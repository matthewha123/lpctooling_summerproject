#include "fault.h"

FATAL_FAULTS_T fatal_faults             = {};
RECOVERABLE_FAULTS_T recoverable_faults = {};

// TODO: Move the fatal and recoverable fault questions to fault.c/h
#define VS_EQ(name) (get_vcu_state() == VCU_STATE_ ## name)
#define VS_NEQ(name) !VS_EQ(name)

bool update_fatal_faults(void) {

  //Run these update functions to check if an error needs to be acknowledged as present or cleared
  update_fatal_contactor_faults();
  update_gate_status();

  //Check all fatal errors against their timeout conditions
  return VS_NEQ(ROOT) && Check_Error_Group(FATAL_ERRORS);
}

bool update_recoverable_faults(void) {
  //Run these update functions to check if an error needs to be acknowledged as present or cleared
  update_recoverable_contactor_faults();
  update_gate_status();
  update_conflicts();
  update_implausibility();

  //Check all recoverable errors against their timeout conditions
  return VS_NEQ(ROOT) && Check_Error_Group(RECOVERABLE_ERRORS);
}

void handle_fatal_fault(void) {
  // Print the status of all monitored faults
  // TODO/HACK: Reenable discharge sequence.
  // // Just in case we lose a CAN msg, do this multiple times
  sendMotorOffCmdMsg();
  sendMotorOffCmdMsg();
  sendMotorOffCmdMsg();
  sendMotorOffCmdMsg();

  // Make sure the driver can avoid death.
  set_brake_valve(false);
  lock_brake_valve();
  disable_controls();

  openLowSideContactor();
  openHighSideContactor();

  printf("[FAULT : HANDLER : FATAL] Need external recovery.\r\n");
}

void handle_recoverable_fault(void) {
  if (Check_Error(ERROR_GATE_RECOVERABLE, false) || get_vcu_state() != VCU_STATE_DRIVING) {
    sendMotorOffCmdMsg();
  }
  else {
    sendTorqueCmdMsg(0);
  }

  // Make sure the driver can avoid death.
  set_brake_valve(false);
  lock_brake_valve();
  disable_controls();
}

void handle_test_fault(void) {
  const Time_T reset_timeout = 5000;
  Time_T in_time             = HAL_GetTick();

  printf("[TEST] Reached end of test. Resetting in %dms...\r\n", reset_timeout);

  while (HAL_GetTick() - in_time < reset_timeout) {
    print_gate_faults(false);
  }

  NVIC_SystemReset();
}
