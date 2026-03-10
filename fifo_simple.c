/*
 * fifo_simple.c
 *
 * Minimal FIFO scheduler for the Codexion project.
 * - Single global scheduler mutex protects dongle queues and states
 * - Each request is enqueued on both required dongle queues
 * - Grant happens only when a Request is head on both queues and both
 *   dongles are FREE and cooldown expired
 * - Uses per-request condition variable for the waiting coder
 *
 * NOTE: This is a simple implementation intended for clarity and for
 * n >= 2. The n == 1 case (single dongle) requires special handling and
 * is not fully addressed here.
 */

#include "codexion.h"
#include <string.h>

typedef struct fs_request_s
{
	t_coders		*coder;
	struct fs_request_s	*next_left;   /* link for left queue */
	struct fs_request_s	*next_right;  /* link for right queue */
	pthread_cond_t		cond;
	int				granted;
} fs_request;

typedef struct fs_dongle_s
{
	t_dongle	*orig;       /* pointer to original dongle struct */
	fs_request	*head;
	fs_request	*tail;
	int			state;      /* 0=FREE,1=IN_USE,2=COOLDOWN */
	long long	cooldown_until_ms;
} fs_dongle;

static fs_dongle *fs_dongles = NULL;
static int fs_n = 0;
static int fs_cooldown_ms = 0;
static pthread_mutex_t fs_mtx = PTHREAD_MUTEX_INITIALIZER;

/* helper: find dongle index by pointer; returns -1 if not found */
static int find_dongle_index(t_dongle *d)
{
	for (int i = 0; i < fs_n; ++i)
	{
		if (fs_dongles[i].orig == d)
			return i;
	}
	return -1;
}

/* attempt to grant any eligible head requests. called with fs_mtx held */
static void attempt_grants(void)
{
	int made_progress;
	do {
		made_progress = 0;
		long long now = get_now_in_ms();
		for (int i = 0; i < fs_n; ++i)
		{
			fs_request *r = fs_dongles[i].head;
			if (!r)
				continue;
			/* compute left and right indices for this request */
			int left = find_dongle_index(r->coder->left_dongle);
			int right = find_dongle_index(r->coder->right_dongle);
			if (left < 0 || right < 0)
				continue; /* shouldn't happen */
			/* check that r is head of both queues */
			if (fs_dongles[left].head != r)
				continue;
			if (fs_dongles[right].head != r)
				continue;
			/* check both dongles free and cooldown passed */
			if (fs_dongles[left].state == 0 && fs_dongles[right].state == 0
				&& fs_dongles[left].cooldown_until_ms <= now
				&& fs_dongles[right].cooldown_until_ms <= now)
			{
				/* grant: mark both IN_USE */
				fs_dongles[left].state = 1;
				fs_dongles[right].state = 1;
				/* remove r from left queue */
				fs_dongles[left].head = r->next_left;
				if (fs_dongles[left].head == NULL)
					fs_dongles[left].tail = NULL;
				/* remove r from right queue */
				fs_dongles[right].head = r->next_right;
				if (fs_dongles[right].head == NULL)
					fs_dongles[right].tail = NULL;
				r->granted = 1;
				/* signal the waiting coder */
				pthread_cond_signal(&r->cond);
				made_progress = 1;
			}
		}
	} while (made_progress);
}

/* initialize the simple FIFO scheduler
 * - dongles: pointer to the array of t_dongle (must remain valid)
 * - n: number of dongles
 * - cooldown_ms: dongle cooldown in ms
 */
int fifo_init(t_dongle *dongles, int n, int cooldown_ms)
{
	if (n <= 0 || !dongles)
		return -1;
	fs_dongles = malloc(sizeof *fs_dongles * n);
	if (!fs_dongles)
		return -1;
	fs_n = n;
	fs_cooldown_ms = cooldown_ms;
	for (int i = 0; i < n; ++i)
	{
		fs_dongles[i].orig = &dongles[i];
		fs_dongles[i].head = fs_dongles[i].tail = NULL;
		fs_dongles[i].state = 0; /* FREE */
		fs_dongles[i].cooldown_until_ms = 0;
	}
	return 0;
}

/* enqueue request for coder and wait until granted. returns 0 on success */
int fifo_enqueue_and_wait(t_coders *coder)
{
	if (!coder || fs_n <= 0)
		return -1;

	fs_request *r = malloc(sizeof *r);
	if (!r)
		return -1;
	memset(r, 0, sizeof *r);
	r->coder = coder;
	r->next_left = r->next_right = NULL;
	pthread_cond_init(&r->cond, NULL);
	r->granted = 0;

	pthread_mutex_lock(&fs_mtx);

	int left = find_dongle_index(coder->left_dongle);
	int right = find_dongle_index(coder->right_dongle);
	if (left < 0 || right < 0)
	{
		pthread_mutex_unlock(&fs_mtx);
		pthread_cond_destroy(&r->cond);
		free(r);
		return -1;
	}

	/* enqueue on left queue */
	if (fs_dongles[left].tail)
		fs_dongles[left].tail->next_left = r;
	else
		fs_dongles[left].head = r;
	fs_dongles[left].tail = r;

	/* enqueue on right queue */
	if (fs_dongles[right].tail)
		fs_dongles[right].tail->next_right = r;
	else
		fs_dongles[right].head = r;
	fs_dongles[right].tail = r;

	/* try to grant any eligible requests */
	attempt_grants();

	/* wait until granted */
	while (!r->granted)
		pthread_cond_wait(&r->cond, &fs_mtx);

	/* granted; remove references already handled in attempt_grants */
	pthread_mutex_unlock(&fs_mtx);

	/* cleanup request resources */
	pthread_cond_destroy(&r->cond);
	free(r);
	return 0;
}

/* release the dongles held by coder and start cooldown */
int fifo_release(t_coders *coder)
{
	if (!coder || fs_n <= 0)
		return -1;

	pthread_mutex_lock(&fs_mtx);
	int left = find_dongle_index(coder->left_dongle);
	int right = find_dongle_index(coder->right_dongle);
	if (left < 0 || right < 0)
	{
		pthread_mutex_unlock(&fs_mtx);
		return -1;
	}
	long long now = get_now_in_ms();
	fs_dongles[left].state = 2; /* COOLDOWN */
	fs_dongles[left].cooldown_until_ms = now + fs_cooldown_ms;
	fs_dongles[right].state = 2; /* COOLDOWN */
	fs_dongles[right].cooldown_until_ms = now + fs_cooldown_ms;

	/* after releasing, try to grant other waiting requests */
	attempt_grants();
	pthread_mutex_unlock(&fs_mtx);
	return 0;
}

void fifo_destroy(void)
{
	if (fs_dongles)
	{
		free(fs_dongles);
		fs_dongles = NULL;
	}
	fs_n = 0;
}

/* End of fifo_simple.c */
