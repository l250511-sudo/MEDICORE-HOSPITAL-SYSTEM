#include "Doctor.h"

void copySpecializationOffset(char* dest, const char* src, int maxLen) {
    int i = 0;
    while (*(src + i) != '\0' && i < maxLen - 1) {
        *(dest + i) = *(src + i);
        i++;
    }
    *(dest + i) = '\0'; 
}

Doctor::Doctor() : Person() {
    *(specialization + 0) = '\0';
    fee = 0.0;
}

Doctor::~Doctor() {
}

void Doctor::setSpecialization(const char* newSpecialization) {
    copySpecializationOffset(specialization, newSpecialization, 50);
}

const char* Doctor::getSpecialization() const {
    return specialization;
}

void Doctor::setFee(double newFee) {
    fee = newFee;
}

double Doctor::getFee() const {
    return fee;
}

void Doctor::displayProfile() const {
    std::cout << "--- Doctor Profile ---\n"
        << "ID: " << getID() << "\n"
        << "Name: " << getName() << "\n"
        << "Specialization: " << specialization << "\n"
        << "Consultation Fee: $" << fee << "\n"
        << "----------------------\n";
}

const char* Doctor::getRole() const {
    return "Doctor";
}

bool Doctor::operator==(const Doctor& other) const {
    return this->getID() == other.getID();
}

std::ostream& operator<<(std::ostream& os, const Doctor& doctor) {
    os << "[Dr. " << doctor.getName() << " - ID: " << doctor.getID()
        << "] Specialization: " << doctor.specialization << " | Fee: $" << doctor.fee;
    return os;
}