#ifndef TIMER_H
#define TIMER_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif
void init_timer(uint32 frequency);
bool get_seconde();
void set_seconde();
#ifdef __cplusplus
}
#endif



#endif
