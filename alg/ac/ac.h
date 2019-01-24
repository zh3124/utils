#ifndef __AC_H__
#define __AC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


void *ac_machine_init();
int ac_machine_insert(void *handle, uint8_t *patten, uint8_t len);
void ac_machine_compile(void *handle);
int ac_machine_search(void *handle, uint8_t *text, uint64_t len, uint8_t **result_pattens, uint8_t *patten_lens, int max_size);
void ac_machine_release(void *handle);

#endif