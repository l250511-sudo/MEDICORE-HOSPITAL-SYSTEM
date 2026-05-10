#pragma once
#include "Person.h"
#include <iostream>

class Doctor : public Person {
private:
    char specialization[50];
    double fee;


public:
    Doctor();
    ~Doctor();

    // Getters and Setters
    void setSpecialization(const char* newSpecialization);
    const char* getSpecialization() const;

    void setFee(double newFee);
    double getFee() const;

    // Pure virtual functions implementations
    void displayProfile() const override;
    const char* getRole() const override;

    // Required Operator Overloads
    bool operator==(const Doctor& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Doctor& doctor);
};