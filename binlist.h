#ifndef BINLIST_H
#define BINLIST_H

#include "sptrlist.h"

typedef struct {
	size_t count;
	size_t binitems;
	size_t itemsize;
	sptrlist bins;
} binlist;

binlist* binlist_new(size_t itemsize, size_t binitems);
void binlist_init(binlist* l, size_t itemsize, size_t binitems);
void binlist_free_items(binlist* l);
void binlist_free(binlist* l);
void* binlist_get(binlist* l, size_t item);
int binlist_set(binlist* l, void* item, size_t pos);
int binlist_add(binlist* l, void* item);

#endif

//RcB: DEP "../src/binlist/*.c"
