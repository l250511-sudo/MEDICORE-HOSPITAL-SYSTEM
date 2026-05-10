#include "InvalidInputException.h"

InvalidInputException::InvalidInputException()
    : HospitalException("Error: User input failed validation.") {
}