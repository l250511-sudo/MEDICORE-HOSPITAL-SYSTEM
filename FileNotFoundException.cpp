#include "FileNotFoundException.h"

FileNotFoundException::FileNotFoundException()
    : HospitalException("Error: Required data file could not be opened.") {
}