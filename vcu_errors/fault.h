#ifndef __FAULT_H
#define __FAULT_H

#include "fault_contactors.h"
#include "fault_gates.h"
#include "fault_heartbeats.h"
#include "fault_boards.h"


#include "error.h"

#include "state.h"

const uint8_t FATAL_ERRORS[4] = [ERROR_GATE_FATAL, 
                                ERROR_PRECHARGE, 
                                ERROR_L_CONTACTOR_MISMATCH, 
                                ERROR_R_CONTACTOR_MISMATCH];

const uint8_t RECOVERABLE_ERRORS[7] = [ ERROR_GATE_RECOVERABLE, 
                                        ERROR_BRAKE_ACCEL_CONFLICT, 
                                        ERROR_ACCEL_CONFLICT, 
                                        ERROR_BMS_HEARTBEAT_BAD, 
                                        ERROR_FCN_HEARTBEAT_BAD, 
                                        ERROR_MC_HEARTBEAT_BAD, 
                                        ERROR_CSB_HEARTBEAT_BAD ];

// CONTAINER DECLARATIONS
extern RECOVERABLE_FAULTS_T recoverable_faults;
extern FATAL_FAULTS_T fatal_faults;

// INTERACTION FUNCTIONS
bool update_fatal_faults(void);
bool update_recoverable_faults(void);

void handle_fatal_fault(void);
void handle_recoverable_fault(void);
void handle_test_fault(void);

#endif
