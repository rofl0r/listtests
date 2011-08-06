#include "../../include/binlist.h"
#include "../../include/sblist.h"

#include <string.h>
#include <stdlib.h>

binlist* binlist_new(size_t itemsize, size_t binitems) {
	binlist* ret = (binlist*) malloc(sizeof(binlist));
	binlist_init(ret, itemsize, binitems);
	return ret;
}

static void binlist_clear(binlist* l) {
	l->count = 0;
}

void binlist_init(binlist* l, size_t itemsize, size_t binitems) {
	if(l) {
		l->binitems = binitems;
		l->itemsize = itemsize;
		sptrlist_init(&l->bins, binitems);
		binlist_clear(l);
	}
}

void binlist_free_items(binlist* l) {
	size_t binsused = l->count / l->binitems + ((l->count % l->binitems) ? 1 : 0);
	size_t i;
	
	if(l) {
		for(i = 0; i < binsused; i++)
			sblist_free((sblist*) sptrlist_get(&l->bins, i));
		binlist_clear(l);
	}
}

void binlist_free(binlist* l) {
	binlist_free_items(l);
	free(l);
}

static char* binlist_item_from_index(binlist* l, size_t idx) {
	size_t targetbin = idx / l->binitems;
	size_t binitem = idx % l->binitems;
	sblist* bin = (sblist*) sptrlist_get(&l->bins, targetbin);
	return sblist_get(bin, binitem);
}

void* binlist_get(binlist* l, size_t item) {
	if(item < l->count) return (void*) binlist_item_from_index(l, item);
	return NULL;
}

int binlist_set(binlist* l, void* item, size_t pos) {
	if(pos >= l->count) return 0;
	memcpy(binlist_item_from_index(l, pos), item, l->itemsize);
	return 1;
}

int binlist_add(binlist* l, void* item) {
	sblist* bin;
	size_t targetbin = l->count / l->binitems;
	size_t binitem = l->count % l->binitems;
	if(binitem == 0) {
		bin = sblist_new(l->itemsize, l->binitems);
		if(!bin) return 0;
		sptrlist_add(&l->bins, bin);
	} else
		bin = sptrlist_get(&l->bins, targetbin);
	l->count++;
	return sblist_add(bin, item);
}
