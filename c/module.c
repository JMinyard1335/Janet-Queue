#include <janet.h>
#include <sys/queue.h>

static int queue_gc(void *data, size_t len);
static int queue_gcmark(void *data, size_t len);
static Janet queue_next(void *data, Janet key);
static int queue_get(void *data, Janet key, Janet *out);
static void queue_tostring(void *data, JanetBuffer *buffer);

/***************/
/* C Types     */
/***************/
typedef struct queue_node {
     Janet value;
     TAILQ_ENTRY(queue_node) entries;
} QueueNode;

typedef struct queue {
     TAILQ_HEAD(queue_head, queue_node) head;
     size_t length;
} Queue;


/***************/
/* Janet Types */
/***************/
static const JanetAbstractType queue_type = {
     "queue", queue_gc, queue_gcmark, queue_get, NULL, NULL,
     NULL, queue_tostring, NULL,
     NULL, queue_next, NULL, NULL, NULL
};


/***************/
/* C Functions */
/***************/
static void
queue_tostring(void *data, JanetBuffer *buffer)
{
     Queue *q = (Queue *)data;
     QueueNode *node;
     janet_buffer_push_cstring(buffer, "[");
     node = TAILQ_FIRST(&q->head);
     TAILQ_FOREACH(node, &q->head, entries) {
	  janet_pretty(buffer, 0, 0, node->value);
          if (node != TAILQ_LAST(&q->head, queue_head)) {
	       janet_buffer_push_cstring(buffer, " ");
          }
     }
     janet_buffer_push_cstring(buffer, "]");
}

static int
queue_get(void *data, Janet key, Janet *out)
{     
     Queue *q = (Queue *)data;

     if (janet_checkint(key)) {
	  int32_t index = janet_unwrap_integer(key);

	  if (index < 0 || (size_t)index >= q->length) {
	       janet_panicf("[Error]: queue index %v is out of bounds", key);
	  }

	  QueueNode *node = TAILQ_FIRST(&q->head);
	  for (int32_t i = 0; i < index; i++) {
	       node = TAILQ_NEXT(node, entries);
	  }
	  *out = node->value;
	  return 1;
     }
  
     return 0;
}


static Janet
queue_next(void *data, Janet key)
{
     Queue *q = (Queue *)data;
     QueueNode *node;

     if (janet_checktype(key, JANET_NIL)) {
	  if (TAILQ_EMPTY(&q->head))
	       janet_wrap_nil();
    
	  node = TAILQ_FIRST(&q->head);
	  return janet_wrap_pointer(node);
     }

     if (!janet_checktype(key, JANET_POINTER)) {
	  janet_panicf("[Error]: queue key %v should be type pointer", key);
     }
     node = (QueueNode *)janet_unwrap_pointer(key);
     node = TAILQ_NEXT(node, entries);
     if (node == NULL)
	  return janet_wrap_nil();

     return janet_wrap_pointer(node);
}


static int
queue_gcmark(void *data, size_t len)
{
     (void)len;
     Queue *q = (Queue *)data;
     QueueNode *node;

     TAILQ_FOREACH(node, &q->head, entries) { janet_mark(node->value); }
     return 0;
}


static int
queue_gc(void *data, size_t len)
{
     (void)len;
     Queue *q = (Queue *)data;
     QueueNode *node;

     while (!TAILQ_EMPTY(&q->head)) {
	  node = TAILQ_FIRST(&q->head);
	  TAILQ_REMOVE(&q->head, node, entries);
	  janet_free(node);
     }
     
     return 0;
}


static Janet
cfuns_queue_new(int32_t argc, Janet *argv)
{
     Queue *q;
     int i;
     janet_arity(argc, 0, -1);

     q = janet_abstract(&queue_type, sizeof(Queue));
     TAILQ_INIT(&q->head);
     q->length = 0;
	  
     for (i = 0; i < argc; i++) {
	  QueueNode *node = janet_malloc(sizeof(QueueNode));
          if (!node)
	       janet_panic("[Error]: Out of Memory.");

          node->value = argv[i];
	  TAILQ_INSERT_TAIL(&q->head, node, entries);
	  q->length++;
     }
     
     return janet_wrap_abstract(q);
}


static Janet
cfuns_queue_enqueue(int32_t argc, Janet *argv)
{
     janet_fixarity(argc, 2);
     Queue *q = janet_getabstract(argv, 0, &queue_type);
     QueueNode *node = janet_malloc(sizeof(QueueNode));
     if (!node)
	  janet_panic("[Error]: Out of Memory, unable to allocate queue node.");

     node->value = argv[1];
     TAILQ_INSERT_TAIL(&q->head, node, entries);
     q->length++;
  
     return janet_wrap_abstract(q);
}


static Janet
cfuns_queue_dequeue(int32_t argc, Janet *argv)
{
     janet_fixarity(argc, 1);
     Queue *q = janet_getabstract(argv, 0, &queue_type);

     // TODO: Find better implementation.
     // Currently Return nil if there is nothing to dequeue
     // Other option just panic, crashes program.
     if (TAILQ_EMPTY(&q->head)) 
	  return janet_wrap_nil();

     QueueNode *node = TAILQ_FIRST(&q->head);
     Janet value = node->value;

     TAILQ_REMOVE(&q->head, node, entries);
     janet_free(node);
     q->length--;
     
     return value;
}


static Janet
cfuns_queue_peek_front(int32_t argc, Janet *argv)
{
     janet_fixarity(argc, 1);
     Queue *q = janet_getabstract(argv, 0, &queue_type);

     if (TAILQ_EMPTY(&q->head)) {
	  return janet_wrap_nil();
     }


     QueueNode *node = TAILQ_FIRST(&q->head);
     janet_printf("%v\n", node->value);
     Janet value = node->value;
     return value;
}


static Janet
cfuns_queue_peek_back(int32_t argc, Janet *argv)
{
     janet_fixarity(argc, 1);
     Queue *q = janet_getabstract(argv, 0, &queue_type);

     if (TAILQ_EMPTY(&q->head)) 
	  return janet_wrap_nil();

     QueueNode *node = TAILQ_LAST(&q->head, queue_head);
     janet_printf("%v\n", node->value);
     Janet value = node->value;
     return value;
}


static Janet
cfuns_queue_empty(int32_t argc, Janet *argv)
{
     janet_fixarity(argc, 1);
     Queue *q = janet_getabstract(argv, 0, &queue_type);
     return janet_wrap_boolean(TAILQ_EMPTY(&q->head));
}


static Janet
cfuns_queue_length(int32_t argc, Janet *argv)
{
     janet_fixarity(argc, 1);
     Queue *q = janet_getabstract(argv, 0, &queue_type);
     return janet_wrap_integer(q->length);
}


static Janet
cfuns_queue_to_array(int32_t argc, Janet *argv)
{

     Queue *q = janet_getabstract(argv, 0, &queue_type);
     QueueNode *node;
     JanetArray *a = janet_array(q->length);
	  
     TAILQ_FOREACH(node, &q->head, entries) {
	  janet_array_push(a, node->value);
     }
     return janet_wrap_array(a);
}

/****************/
/* Module Entry */
/****************/

static const JanetReg cfuns[] = {
     {"new", cfuns_queue_new,
      "(queue/new ...args)\n\nCreates a new empty Queue, or creates a queue from the given args"},
     {"enqueue", cfuns_queue_enqueue,
      "(queue/enqueue q value)\n\nAdds 'value' to the back of 'q"},
     {"dequeue", cfuns_queue_dequeue,
      "(queue/dequeue q)\n\nRemoves the front node and returns its value."},
     {"peek-front", cfuns_queue_peek_front,
      "(queue/peek-front q)\n\nReturns the value at the front of the queue."},
     {"peek-back", cfuns_queue_peek_back,
      "(queue/peek-back q)\n\nReturns the value at the back of the queue."},
     {"empty?", cfuns_queue_empty,
      "(queue/empty q)\n\nReturns if 'q' is empty or not."},
     {"length", cfuns_queue_length,
      "(queue/length q)\n\nReturns the length of 'q'."},
     {"to-array", cfuns_queue_to_array,
      "(queue/to-array q)\n\nconverts 'q' to an array."},
     {NULL, NULL, NULL},
};


JANET_MODULE_ENTRY(JanetTable *env) {
     janet_cfuns(env, "queue", cfuns);
}
