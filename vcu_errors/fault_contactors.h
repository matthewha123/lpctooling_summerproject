#ifndef __FAULT_CONTACTORS_H
#define __FAULT_CONTACTORS_H

#include "stdio.h"
#include <stdbool.h>

#include "gpio.h"
#include "extern.h"
#include "error.h"

typedef struct {
  bool L_contactor_closed;
  bool H_contactor_closed;
  bool L_contactor_welded;
  bool H_contactor_welded;
} Contactors_T;


extern volatile Contactors_T contactors;

bool update_fatal_contactor_faults(void);
bool update_recoverable_contactor_faults(void);

bool update_fatal_contactor_mismatch_faults(void);

void print_contactor_state(bool force);

#endif // ifndef __FAULT_CONTACTORS_H
