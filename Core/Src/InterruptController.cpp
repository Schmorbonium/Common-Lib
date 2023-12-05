#include "zHal.h"
#include "InterruptController.h"

// Static variable to keep track of the interrupt count
static volatile uint16_t count = 0;

static void blockInterrupts() {
    __disable_irq();
}

static void unblockInterrupts() {
    __enable_irq();
}

// Enter sensitive area
void InterruptController_enter() {
    if (count == 0) {
        blockInterrupts();
    }
    count++;
}

// Leave sensitive area
void InterruptController_leave() {
    if (count > 0) {
        count--;
        if (count == 0) {
            unblockInterrupts();
        }
    }
}