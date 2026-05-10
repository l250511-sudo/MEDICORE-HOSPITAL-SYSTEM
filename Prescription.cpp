#include "Prescription.h"

void copyRxString(char* dest, const char* src, int maxLen) {
    int i = 0;
    while (*(src + i) != '\0' && i < maxLen - 1) {
        *(dest + i) = *(src + i);
        i++;
    }
    *(dest + i) = '\0';
}

Prescription::Prescription() {
    id = 0;
    appointmentID = 0;
    patientID = 0;
    doctorID = 0;
    *(date + 0) = '\0';
    *(medicines + 0) = '\0';
    *(notes + 0) = '\0';
}

Prescription::~Prescription() {}

void Prescription::setID(int newId)
{ id = newId; }
int Prescription::getID() const
{ return id; }

void Prescription::setAppointmentID(int aId) 
{ appointmentID = aId; }
int Prescription::getAppointmentID() const 
{ return appointmentID; }

void Prescription::setPatientID(int pId) 
{ patientID = pId; }
int Prescription::getPatientID() const 
{ return patientID; }

void Prescription::setDoctorID(int dId) 
{ doctorID = dId; }
int Prescription::getDoctorID() const
{ return doctorID; }

void Prescription::setDate(const char* newDate)
{ copyRxString(date, newDate, 11); }
const char* Prescription::getDate() const 
{ return date; }

void Prescription::setMedicines(const char* meds)
{ copyRxString(medicines, meds, 500); }
const char* Prescription::getMedicines() const 
{ return medicines; }

void Prescription::setNotes(const char* newNotes)
{ copyRxString(notes, newNotes, 300); }
const char* Prescription::getNotes() const 
{ return notes; }