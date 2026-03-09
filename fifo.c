typedef struct queue_node_s {
    int coder_id;              
    struct queue_node_s* next; 
} queue_node_t;

typedef struct fifo_queue_s {
    queue_node_t* head; // The front of the queue (the first one to ask)
    queue_node_t* tail; // The back of the queue (the last one to ask)
} fifo_queue_t;

void append(fifo_queue_t* queue, int coder_id){
	queue_node_t* new_node = malloc(sizeof(queue_node_t));
	if (!new_node)
		exit(-1);
	new_node->coder_id = coder_id;
	new_node->next = NULL;

	if (queue->tail == NULL)
	{
		queue->head = new_node;
		queue->tail = new_node;
	}
	else 
	{
		queue->tail->next = new_node;
		queue->tail = new_node;
	}
}

int peek(fifo_queue_t* queue)
{
	if (queue->head == NULL)
	{
		return (-1);
	}
	return queue->head->coder_id;
}

