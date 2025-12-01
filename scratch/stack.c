#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define T int
#define STACK_CHUNK_SIZE 100

typedef enum  {
    STACKRESULT_OK = 0,
    STACKRESULT_OOM,
    STACKRESULT_OVERFLOW,
} StackResult;

typedef struct Stack {
	T *data;
	size_t count;
    size_t capacity;
} Stack;

StackResult Stack_init(Stack *s, size_t capacity){
    assert(s != NULL && capacity > 0);
    s->data = NULL; s->count = 0; s->capacity = 0;
    if(capacity > SIZE_MAX / sizeof(*s->data)) return STACKRESULT_OVERFLOW;
    s->data = malloc(capacity * sizeof(*s->data));
    if (!s->data) return STACKRESULT_OOM;
    s->capacity = capacity;
    return STACKRESULT_OK;
}

bool Stack_is_full(const Stack *s){
    assert(s != NULL);
    return s->capacity <= s->count;
}

void Stack_push(Stack *s, T new_elem){ // Unchecked version, faster, won't allocate
    assert(s != NULL);
    assert(!Stack_is_full(s));
    s->data[s->count++] = new_elem;
}

StackResult Stack_push_maybe_grow(Stack *s, T new_elem){ // Will allocate as needed
    assert(s != NULL);
    if (Stack_is_full(s)){
        if(s->capacity > SIZE_MAX - (size_t)STACK_CHUNK_SIZE) return STACKRESULT_OVERFLOW;

        size_t new_count = s->capacity + (size_t)STACK_CHUNK_SIZE;
        if(new_count > SIZE_MAX / sizeof(*s->data)) return STACKRESULT_OVERFLOW;

        T *p = realloc(s->data, new_count * sizeof(*s->data));
        if (!p) return STACKRESULT_OOM;

        s->data = p;
        s->capacity = new_count;
    }

    Stack_push(s, new_elem);
    return STACKRESULT_OK;
}

T Stack_pop(Stack *s){
    assert(s != NULL);
    assert(s->count > 0);
    return s->data[--s->count];
}

void Stack_free(Stack *s){
    if(!s) return;
    free(s->data);
    s->capacity = 0;
    s->count = 0;
    s->data = NULL; // Somewhat helpful to catch dangling pointers
}

