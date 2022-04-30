#ifndef TIMER_H
#define TIMER_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif
void init_timer(uint32 frequency);
void wait(uint32 milliseconds);
uint32 get_ticks();
uint32 get_freq();
#ifdef __cplusplus
}
#endif



#endif
