#include "Validator.h"

Validator::Validator() {}
Validator::~Validator() {}

int Validator::getLength(const char* str) {
    int len = 0;
    while (*(str + len) != '\0') {
        len++;
    }
    return len;
}

bool Validator::isDigit(char c) {
    return (c >= '0' && c <= '9');
}

bool Validator::isValidID(int id) {
    return id > 0;
}

// Format: DD-MM-YYYY (Length must be exactly 10)
bool Validator::isValidDate(const char* date) {
    // 1. Basic length and format check (DD-MM-YYYY)
    if (getLength(date) != 10) return false;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (*(date + i) != '-') return false;
        }
        else {
            if (!isDigit(*(date + i))) return false;
        }
    }

    // 2. Extract values using pointer arithmetic (e.g., '1' '2' -> 1*10 + 2 = 12)
    int day = (*(date + 0) - '0') * 10 + (*(date + 1) - '0');
    int month = (*(date + 3) - '0') * 10 + (*(date + 4) - '0');
    int year = (*(date + 6) - '0') * 1000 + (*(date + 7) - '0') * 100 +
        (*(date + 8) - '0') * 10 + (*(date + 9) - '0');

    // 3. Basic Month and Year Range
    if (year < 2024 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    // 4. Specific Month Length Checks
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    }

    // 5. February & Leap Year Logic
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (isLeap) {
            if (day > 29) return false;
        }
        else {
            if (day > 28) return false;
        }
    }

    return true;
}
//  Exactly 11 digits, all numeric
bool Validator::isValidContact(const char* contact) {
    if (getLength(contact) != 11) return false;

    for (int i = 0; i < 11; i++) {
        if (!isDigit(*(contact + i))) return false;
    }
    return true;
}

// Minimum 6 characters
bool Validator::isValidPassword(const char* password) {
    return getLength(password) >= 6;
}

bool Validator::isPositiveFloat(double value) {
    return value >= 0.0;
}

bool Validator::isValidMenuChoice(int choice, int min, int max) {
    return (choice >= min && choice <= max);
}