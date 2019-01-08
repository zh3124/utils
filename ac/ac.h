#ifndef __AC_H__
#define __AC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


void *ac_machine_init();
int ac_machine_insert(void *handle, char *patten);
void ac_machine_compile(void *handle);
int ac_machine_search(void *handle, char *text, uint64_t len, char **result_patten, int *patten_len, int max_size);
void ac_machine_release(void *handle);

#endif