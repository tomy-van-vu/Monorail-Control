#pragma once
#include "BTInterface.h"

typedef enum {
  O_NONE,
  O_EAST,
  O_WEST,
  O_START,
  O_STOP,
  O_OPEN,
  O_CLOSE,
  O_EMERGENCY,
} operator_instructions;

// Deprecated
// No longer in use
typedef enum{
  T_NONE,
  T_EAST,
  T_WEST,
  T_START,
  T_STOP,
  T_OPEN,
  T_CLOSE,
  T_EMERGENCY,
} control_box_tx;


typedef struct {
  operator_instructions current_instruction;
  operator_instructions next_instruction;
  bool has_new;
} SM_instructions;


message read_msg();
message testing_mode_read_message();
