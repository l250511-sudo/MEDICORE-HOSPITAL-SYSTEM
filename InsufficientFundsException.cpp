#include "InsufficientFundsException.h"

InsufficientFundsException::InsufficientFundsException()
    : HospitalException("Error: Patient's balance is less than the required amount.") {
}