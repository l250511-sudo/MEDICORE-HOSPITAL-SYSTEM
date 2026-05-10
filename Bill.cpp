#include "Bill.h"


void copyStringOffsetBill(char* dest, const char* src, int maxLen) {
    int i = 0;
    while (*(src + i) != '\0' && i < maxLen - 1) {
        *(dest + i) = *(src + i);
        i++;
    }
    *(dest + i) = '\0';
}

Bill::Bill() {
    id = 0; 
    appointmentID = 0;
    patientID = 0;
    amount = 0.0;
    *(date + 0) = '\0';
  
    setStatus("Unpaid");
}

Bill::~Bill() {}


void Bill::setID(int newId) { id = newId; }
int Bill::getID() const { return id; }

void Bill::setAppointmentID(int id) { appointmentID = id; }
int Bill::getAppointmentID() const { return appointmentID; }

void Bill::setPatientID(int id) { patientID = id; }
int Bill::getPatientID() const { return patientID; }

void Bill::setAmount(double amt) { amount = amt; }
double Bill::getAmount() const { return amount; }


void Bill::setStatus(const char* s) {
    int i = 0;
    while (*(s + i) != '\0' && i < 19) {
        *(status + i) = *(s + i);
        i++;
    }
    *(status + i) = '\0';
}

const char* Bill::getStatus() const {
    return status;
}


void Bill::setDate(const char* newDate) {
    copyStringOffsetBill(date, newDate, 11);
}

const char* Bill::getDate() const {
    return date;
}