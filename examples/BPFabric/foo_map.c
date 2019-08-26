#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "foo_map.h"

struct bpf_map *foo_map_alloc(union bpf_attr *attr)
{
    struct bpf_array *foo_map;
    uint64_t array_size;
    uint32_t elem_size;
   
    /* check sanity of attributes */
    if (attr->max_entries != 0 || attr->key_size != 0 ||
        attr->value_size != 0) {
        errno = EINVAL;
        return NULL;
    }

    /* allocate the mincountmap structure*/
    foo_map = calloc(1*sizeof(int), sizeof(*foo_map));
    
    if (!foo_map) {
        errno = ENOMEM;
        return NULL;
    }

    /* copy mandatory map attributes */
    foo_map->map.map_type = attr->map_type;
    foo_map->map.key_size = sizeof(uint32_t);
    foo_map->map.value_size = sizeof(uint32_t);
    foo_map->map.max_entries = 1;

    foo_map->elem_size = sizeof(int);

    return &foo_map->map;

}

void *foo_map_lookup_elem(struct bpf_map *map, void *key)
{
    uint32_t* ret = calloc(1, sizeof(uint32_t));
    
    struct bpf_array *array = container_of(map, struct bpf_array, map);

    *ret = *((uint32_t*)array->value);
    return ret;
}


int foo_map_update_elem(struct bpf_map *map, void *key, void *value,
                 uint64_t map_flags)
{
    if (map_flags > BPF_EXIST) {
        /* unknown flags */
        errno = EINVAL;
        return -1;
    }

    if (map_flags == BPF_NOEXIST) {
        /* all elements already exist */
        errno = EEXIST;
        return -1;
    }
   
    struct bpf_array *array = container_of(map, struct bpf_array, map);
    *((uint32_t*)array->value) += 1;

    unsigned int d = *((unsigned char*)key);

    return 0;
}

void foo_map_free(struct bpf_map *map)
{
    struct bpf_array *array = container_of(map, struct bpf_array, map);

    free(array);
}

