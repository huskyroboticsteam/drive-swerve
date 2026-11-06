#include "hall_encoders.h"
#include "commutation.h"

// Tracks the last read value of the hall sensors
// volatile to avoid issues with using interrupts
extern volatile HallState_t Hall_States[HALL_COUNT];
