#include "fault_pedalbox.h"

Conflicts_T conflicts        = {};
volatile Pedalbox_T pedalbox = {};


void update_conflicts() {
  if (Check_Error(ERROR_BRAKE_ACCEL_CONFLICT, false)) {
    // Clear conflict if accel was released.
    // printf("[FAULT : PEDALBOX : BRAKE_ACCEL] Conflict.\r\n");
    if ((pedalbox_min(accel) < PEDALBOX_ACCEL_RELEASE) &&
        (pedalbox.brake_2 < PEDALBOX_BRAKE_BEGIN)) {
      printf("[FAULT : PEDALBOX : BRAKE_ACCEL] Potential release.\r\n");
    
      Error_Clear(ERROR_BRAKE_ACCEL_CONFLICT);
    }
    else {
      // If conflict not cleared, keep it as is.
      return;
    }
  }

  bool brake_engaged = pedalbox.brake_2 > PEDALBOX_BRAKE_BEGIN;
  bool accel_engaged = pedalbox_min(accel) > PEDALBOX_ACCEL_BEGIN;

  if(brake_engaged && accel_engaged) {
    Error_Present(ERROR_BRAKE_ACCEL_CONFLICT);
  } else {
    Error_Clear(ERROR_BRAKE_ACCEL_CONFLICT);
  }
}

void update_implausibility() {
  conflicts.any_errs = conflicts.fcn.accel_1_over  ||
                      conflicts.fcn.accel_1_under ||
                      conflicts.fcn.accel_2_over  ||
                      conflicts.fcn.accel_2_under ||
                      conflicts.fcn.brake_1_over  ||
                      conflicts.fcn.brake_1_under ||
                      conflicts.fcn.brake_2_over  ||
                      conflicts.fcn.brake_2_under;

  bool implausibility = ((pedalbox_max(accel) - pedalbox_min(accel)) >
                        PEDALBOX_ACCEL_IMPLAUSIBLE) ||
                        conflicts.any_errs;

  // There is an implausibility if the range of accel meaasurements is above the
  // implausibility threshold.



  if (implausibility) {
    if (conflicts.observed_implausibility) {
      Error_Present(ERROR_ACCEL_CONFLICT);
    }
  }
  else {
    if (conflicts.observed_implausibility) {
    }
    else {
      if (HAL_GetTick() - Error_GetStatus(ERROR_ACCEL_CONFLICT)->last_time_stamp > 1000) {
        Error_Clear(ERROR_ACCEL_CONFLICT);
      }
    }
  }

  conflicts.observed_implausibility = implausibility;
}

int32_t get_pascals(int16_t brake_reading) {
  // Same conversion as the dash
  int32_t psi = (2019 * brake_reading) / 1000 - 188;

  // Convert to pascals
  int32_t pascals = 689476 * psi / 100;

  return pascals;
}
