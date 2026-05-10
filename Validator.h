#pragma once

class Validator {
private:
    // Helper function to get length without using strlen()
    static int getLength(const char* str);
    // Helper function to check if a character is a digit
    static bool isDigit(char c);

public:
    Validator();
    ~Validator();

    // Required Validations
    static bool isValidID(int id);
    static bool isValidDate(const char* date); // DD-MM-YYYY
    static bool isValidContact(const char* contact); // 11 digits
    static bool isValidPassword(const char* password); // Min 6 chars
    static bool isPositiveFloat(double value);
    static bool isValidMenuChoice(int choice, int min, int max);
};