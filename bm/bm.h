#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void *bm_compile(uint8_t *patten, uint8_t len);
uint8_t *bm_search(void *tmp, uint8_t *text, int len);
void bm_release(void *handle);
