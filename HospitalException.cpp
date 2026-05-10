#include "HospitalException.h"


void copyExceptionString(char* dest, const char* src, int maxLen) {
    int i = 0;
    while (*(src + i) != '\0' && i < maxLen - 1) {
        *(dest + i) = *(src + i);
        i++;
    }
    *(dest + i) = '\0';
}

HospitalException::HospitalException(const char* msg) {
    copyExceptionString(message, msg, 200);
}

const char* HospitalException::what() const {
    return message;
}