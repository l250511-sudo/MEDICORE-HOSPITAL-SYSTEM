#pragma once
#include "Person.h"
#include <iostream>

class Admin : public Person {
public:
    Admin();
    ~Admin();

    // Pure virtual functions implementations
    void displayProfile() const override;
    const char* getRole() const override;
};