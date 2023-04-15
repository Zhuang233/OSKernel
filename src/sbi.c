#include "sbi.h"

uint64 sbi_set_timer(uint64 stime_value)
{
    return SET_TIMER(stime_value);
}