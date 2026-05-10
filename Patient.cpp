#include "Patient.h"

void copyContactOffset(char* dest, const char* src, int maxLen) {
    int i = 0;
    while (*(src + i) != '\0' && i < maxLen - 1) {
        *(dest + i) = *(src + i);
        i++;
    }
    *(dest + i) = '\0'; 
}

// Constructor
Patient::Patient() : Person() {
    age = 0;
    gender = 'U'; // U for Unknown
    *(contact + 0) = '\0'; 
    balance = 0.0;
}

Patient::~Patient() {
   
}

void Patient::setAge(int newAge) { age = newAge; }
int Patient::getAge() const { return age; }

void Patient::setGender(char newGender) { gender = newGender; }
char Patient::getGender() const { return gender; }

void Patient::setContact(const char* newContact) {
    copyContactOffset(contact, newContact, 12);
}
const char* Patient::getContact() const { return contact; }

void Patient::setBalance(double newBalance) { balance = newBalance; }
double Patient::getBalance() const { return balance; }


void Patient::displayProfile() const {
    std::cout << "--- Patient Profile ---\n"
        << "ID: " << getID() << "\n"
        << "Name: " << getName() << "\n"
        << "Age: " << age << " | Gender: " << gender << "\n"
        << "Contact: " << contact << "\n"
        << "Balance: $" << balance << "\n"
        << "-----------------------\n";
}

const char* Patient::getRole() const {
    return "Patient";
}

// --- Operator Overloads ---

// Add to balance (Top-up or Refund)
Patient& Patient::operator+=(double amount) {
    this->balance += amount;
    return *this;
}

// Subtract from balance (Paying a bill)
Patient& Patient::operator-=(double amount) {
    this->balance -= amount;
    return *this;
}

// Check if two patients are the exact same person
bool Patient::operator==(const Patient& other) const {
    return this->getID() == other.getID();
}

// Overload << for easy console printing (e.g., cout << myPatient;)
std::ostream& operator<<(std::ostream& os, const Patient& patient) {
    os << "[" << patient.getID() << "] " << patient.getName()
        << " | Balance: $" << patient.balance;
    return os;
}