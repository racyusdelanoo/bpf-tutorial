#ifndef __EBPF_FOOMAP_H
#define __EBPF_FOOMAP_H

#include "bpfmap.h"

struct bpf_map *foo_map_alloc(union bpf_attr *attr);
void foo_map_free(struct bpf_map *map);
void *foo_map_lookup_elem(struct bpf_map *map, void *key);
int foo_map_update_elem(struct bpf_map *map, void *key, void *value, uint64_t map_flags);


#endif
