#include "Admin.h"

Admin::Admin() : Person() {
    // Inheritance 
}

Admin::~Admin() {
}

void Admin::displayProfile() const {
    std::cout << "--- Admin Profile ---\n"
        << "ID: " << getID() << "\n"
        << "Name: " << getName() << "\n"
        << "Role: System Administrator\n"
        << "---------------------\n";
}

const char* Admin::getRole() const {
    return "Admin";
}   