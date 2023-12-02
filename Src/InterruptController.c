#include "InterruptController.h"
#include "zHal.h"

// Static variable to keep track of the interrupt count
static uint16_t count = 0;

static inline void blockInterrupts() {
    __disable_irq();
}

static inline void unblockInterrupts() {
    __enable_irq();
}

// Enter sensitive area
void inline InterruptController_enter() {
    if (count == 0) {
        blockInterrupts();
    }
    count++;
}

// Leave sensitive area
void inline InterruptController_leave() {
    if (count > 0) {
        count--;
        if (count == 0) {
            unblockInterrupts();
        }
    }
}