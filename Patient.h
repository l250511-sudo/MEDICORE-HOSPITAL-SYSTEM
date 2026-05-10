#pragma once
#include "Person.h"
#include <iostream>

// Patient inherits from the abstract Person class
class Patient : public Person {
private:
    int age;
    char gender;
    char contact[12]; // 11 digits + '\0' null terminator
    double balance;

public:
    Patient();
    ~Patient();

    // Getters and Setters
    void setAge(int newAge);
    int getAge() const;

    void setGender(char newGender);
    char getGender() const;

    void setContact(const char* newContact);
    const char* getContact() const;

    void setBalance(double newBalance);
    double getBalance() const;

    // Must implement pure virtual functions from Person
    void displayProfile() const override;
    const char* getRole() const override;

    // Required Operator Overloads
    Patient& operator+=(double amount);  // For adding funds/refunds
    Patient& operator-=(double amount);  // For paying bills
    bool operator==(const Patient& other) const; // To compare if two patients are the same

    // Friend function for console output
    friend std::ostream& operator<<(std::ostream& os, const Patient& patient);
};