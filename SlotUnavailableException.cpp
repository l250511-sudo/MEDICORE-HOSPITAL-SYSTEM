#include "SlotUnavailableException.h"

SlotUnavailableException::SlotUnavailableException()
    : HospitalException("Error: Attempted to book an already occupied time slot.") {
}