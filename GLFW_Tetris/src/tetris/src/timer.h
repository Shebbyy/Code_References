#pragma once

#include "./primitives.h"

void     drive_timer      (void);
double * get_interval     (void);
void     initialize_timer (on_timer_type * on_timer);
void     reset_timer      (void);
void     set_interval     (double interval);
