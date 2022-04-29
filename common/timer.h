#ifndef TIMER_H
#define TIMER_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif
void init_timer(uint32 frequency);
uint32 get_ticks();
#ifdef __cplusplus
}
#endif



#endif
