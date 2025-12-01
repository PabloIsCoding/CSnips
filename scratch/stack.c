// TODO: Add a parser script that will do the transformations in the comments below (first @ means "transform into", second one marks the end)
//__StackResult__@{1}@
//__STACKRESULT__@${1/(.*)/${1:/upcase}/}@
//__Stack__@{2}@
//__STACK__@${2/(.*)/${1:/upcase}/}@
//__T__@{3}@
//#define __T__ int@@


#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define __T__ int
#define __STACK___CHUNK_SIZE 100

typedef enum  {
    STACKRESULT_OK = 0,
    STACKRESULT_OOM,
    STACKRESULT_OVERFLOW,
} __StackResult__;

typedef struct __Stack__  {
	__T__ *data;
	size_t count;
    size_t capacity;
} __Stack__ ;

__StackResult__ __Stack___init(__Stack____ *s, size_t capacity){
    assert(s != NULL && capacity > 0);
    s->data = NULL; s->count = 0; s->capacity = 0;
    if(capacity > SIZE_MAX / sizeof(*s->data)) return STACKRESULT_OVERFLOW;
    s->data = malloc(capacity * sizeof(*s->data));
    if (!s->data) return STACKRESULT_OOM;
    s->capacity = capacity;
    return STACKRESULT_OK;
}

bool __Stack___is_full(const __Stack__ *s){
    assert(s != NULL);
    return s->capacity <= s->count;
}

void __Stack___push(__Stack__ *s, __T__ new_elem){ // Unchecked version, faster, won't allocate
    assert(s != NULL);
    assert(!__Stack___is_full(s));
    s->data[s->count++] = new_elem;
}

__StackResult__ __Stack___push_maybe_grow(__Stack__  *s, __T__ new_elem){ // Will allocate as needed
    assert(s != NULL);
    if (__Stack___is_full(s)){
        if(s->capacity > SIZE_MAX - (size_t)STACK_CHUNK_SIZE) return STACKRESULT_OVERFLOW;

        size_t new_count = s->capacity + (size_t)STACK_CHUNK_SIZE;
        if(new_count > SIZE_MAX / sizeof(*s->data)) return STACKRESULT_OVERFLOW;

        __T__ *p = realloc(s->data, new_count * sizeof(*s->data));
        if (!p) return STACKRESULT_OOM;

        s->data = p;
        s->capacity = new_count;
    }

    __Stack___push(s, new_elem);
    return STACKRESULT_OK;
}

__T__ __Stack___pop(__Stack__  *s){
    assert(s != NULL);
    assert(s->count > 0);
    return s->data[--s->count];
}

void __Stack___free(__Stack__  *s){
    if(!s) return;
    free(s->data);
    s->capacity = 0;
    s->count = 0;
    s->data = NULL; // Somewhat helpful to catch dangling pointers
}

