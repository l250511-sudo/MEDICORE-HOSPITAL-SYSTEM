
#pragma once

class Person {
protected:
    int id;
    char name[50];
    char password[50];
    char contact[20];

public:
    Person();
    virtual ~Person(); // Virtual destructor for safe inheritance

    // Setters and Getters
    void setID(int newID);
    int getID() const;

    void setName(const char* newName);
    const char* getName() const;

    void setPassword(const char* newPassword);
    const char* getPassword() const;

    // The TWO required pure virtual methods
    virtual void displayProfile() const = 0;
    virtual const char* getRole() const = 0;

    void setContact(const char* newContact);
    const char* getContact() const;
};