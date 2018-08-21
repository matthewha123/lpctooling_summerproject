#include "fault_contactors.h"

volatile Contactors_T contactors = {};

// TODO: Implement requested vs measured mismatch contactor fault. Possibly on
// BMS too.

bool update_fatal_contactor_faults(void) {
  return update_fatal_contactor_mismatch_faults();
}

bool update_recoverable_contactor_faults(void) {
  
}


bool update_fatal_contactor_mismatch_faults(void) {
  static Time_T last_L_mismatch_time = 0;
  static bool   last_L_mismatch      = false;

  bool L_mismatch = (READ_PIN(L_CONTACTOR) != READ_PIN(L_CONTACTOR_STATUS));

  if(L_mismatch) {
    Error_Present(ERROR_L_CONTACTOR_MISMATCH);
  }
  else {
    Error_Clear(ERROR_L_CONTACTOR_MISMATCH);
  }

}

void print_contactor_state(bool force) {
  printf("L SIDE MONITOR %d\r\n", contactors.L_contactor_closed);
}
