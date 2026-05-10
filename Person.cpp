#include "Person.h"

Person::Person() {
    id = 0;
    *(name + 0) = '\0';
    *(password + 0) = '\0';
    *(contact + 0) = '\0';
}

Person::~Person() {}

void Person::setID(int newID) { id = newID; }
int Person::getID() const { return id; }

void Person::setName(const char* newName) {
    int i = 0;
    while (*(newName + i) != '\0' && i < 99) {
        *(name + i) = *(newName + i);
        i++;
    }
    *(name + i) = '\0';
}
const char* Person::getName() const { return name; }

void Person::setPassword(const char* newPassword) {
    int i = 0;
    while (*(newPassword + i) != '\0' && i < 49) {
        *(password + i) = *(newPassword + i);
        i++;
    }
    *(password + i) = '\0';
}
const char* Person::getPassword() const { return password; }


void Person::setContact(const char* newContact) {
    int i = 0;
    while (*(newContact + i) != '\0' && i < 19) {
        *(contact + i) = *(newContact + i);
        i++;
    }
    *(contact + i) = '\0';
}
const char* Person::getContact() const { return contact; }