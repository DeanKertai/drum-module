#include "utils.h"

/**
 * Checks if micros() is still in the time period defined by the start
 * timestamp and a duration. Also handles instances where micros() has
 * overflowed.
 */
bool Utils::isInPeriod(unsigned long start, unsigned long duration) {
    // Since elapsed is unsigned, it will properly handle micros() overflowing
    unsigned long elapsed = micros() - start;
    return elapsed < duration;
}
