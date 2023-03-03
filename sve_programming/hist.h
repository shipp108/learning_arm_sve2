#ifndef __HIST_H__
#define __HIST_H__

#include <stdint.h>
#include <stdlib.h>

void histogram_c(uint32_t *hist, uint8_t *data, size_t sz);
void histogram_sve_gather(uint32_t *hist, uint8_t *data, size_t sz);

void histogram_sve_common(uint32_t *hist, uint8_t *data, size_t sz);

#endif // __HIST_H__