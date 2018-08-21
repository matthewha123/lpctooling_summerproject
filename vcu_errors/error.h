/*
  This is a standardized error handling system, for all of the car's boards. It is based on the bms error handling
  This folder contains vcu code replaced by this error handler.

  Before, all faults were checked by their individual functions.
  Now, the "update_fatal_faults()" and "upadte_recoverable_faults()" functions call various update functions, like update_recoverable_contactor_faults(), for example
  These functions will determine whether to call Error_Clear or Error_Present on their specific errors

  Then, at the end of the "update_fatal_faults()" and "upadte_recoverable_faults()" functions,
  Check_Errors_Group() is called, with an array of the recoverable and fatal faults respectively
  This Function will return true if any of those errors constitute a fault

  Benefits: Gets rid of a lot of repetive timeout code
            error handling is abstracted away from takes place in a centralized place

  Downsides: some error logic for the heartbeats has to take place in the can_manager
*/

#ifndef _ERROR_HANDLER_H
#define _ERROR_HANDLER_H

#include <stdint.h>
#include <stdbool.h>

#include "board.h"

#define NO_CONDITION 0

#define GATE_RECOVERABLE_TIMEOUT NO_CONDITION
#define GATE_FATAL_TIMEOUT 5
#define PRECHARGE_TIMEOUT 1
#define L_CONTACTOR_MISMATCH_TIMEOUT 250
#define R_CONTACOTR_MISMATCH_TIMEOUT 250
#define ERROR_BRAKE_ACCEL_CONFLICT_TIMEOUT NO_CONDITION
#define ACCEL_CONFLICT_TIMEOUT 100
#define BMS_HEARTBEAT_BAD_TIMEOUT 1000
#define FCN_HEARTBEAT_BAD_TIMEOUT 10000
#define MC_HEARTBEAT_BAD_TIMEOUT 1000
#define CSB_HEARTBEAT_BAD_TIMEOUT 1000


typedef enum {
  ERROR_GATE_RECOVERABLE,
  ERROR_GATE_FATAL,
  ERROR_PRECHARGE,
  ERROR_L_CONTACTOR_MISMATCH,
  ERROR_R_CONTACTOR_MISMATCH,
  ERROR_BRAKE_ACCEL_CONFLICT,
  ERROR_ACCEL_CONFLICT,
  ERROR_BMS_HEARTBEAT_BAD,
  ERROR_FCN_HEARTBEAT_BAD,
  ERROR_MC_HEARTBEAT_BAD,
  ERROR_CSB_HEARTBEAT_BAD,
  ERROR_NUM_ERRORS
} ERROR_T;

static const char *const ERROR_NAMES[ERROR_NUM_ERRORS] = {
  "ERROR_GATE_RECOVERABLE",
  "ERROR_GATE_FATAL",
  "ERROR_PRECHARGE",
  "ERROR_L_CONTACTOR_MISMATCH",
  "ERROR_R_CONTACTOR_MISMATCH",
  "ERROR_BRAKE_ACCEL_CONFLICT",
  "ERROR_ACCEL_CONFLICT",
  "ERROR_HEARTBEAT_BMS_BAD",
  "ERROR_HEARTBEAT_FCN_BAD",
  "ERROR_MC_HEARTBEAT_BAD",
  "ERROR_CSB_HEARTBEAT_BAD"
};


typedef struct {
  bool     handling;
  bool     error;
  uint16_t count;
  uint32_t time_stamp;
} ERROR_STATUS_T;

typedef enum error_handler_status {
    FINE,
    FAULT
} ERROR_HANDLER_STATUS_T;

typedef ERROR_HANDLER_STATUS_T (*ERROR_HANDLER_FUNC)(ERROR_STATUS_T *,
                                                     const uint32_t);
typedef struct {
  ERROR_HANDLER_FUNC handler;
  const uint32_t     condition;
} ERROR_HANDLER;


//called when an error is present
void Error_Present(ERROR_T error);

//check if any of the errors, fatal or recoverable, return a FAULT
bool Check_All_Errors(void);

//Whether a specific error should lead to a fault
bool Check_Error(ERROR_T er_t, bool Force_Check);

//Whether any of the errors in an array lead to a fault
bool Check_Error_Group(ERROR_T* er_t);

//returns whether an error is ignored or not
bool Error_Care(ERROR_T er_t);

#endif // ifndef _ERROR_HANDLER_H
