#include "Appointment.h"

int compareStringOffset(const char* str1, const char* str2) {
    int i = 0;
    while (*(str1 + i) != '\0' && *(str1 + i) == *(str2 + i)) {
        i++;
    }
    return *(str1 + i) - *(str2 + i);
}

void copyStringOffset(char* dest, const char* src, int maxLen) {
    int i = 0;
    while (*(src + i) != '\0' && i < maxLen - 1) {
        *(dest + i) = *(src + i);
        i++;
    }
    *(dest + i) = '\0';
}

Appointment::Appointment() {
    id = 0;
    patientID = 0;
    doctorID = 0;
    *(date + 0) = '\0';
    *(timeSlot + 0) = '\0';
    setStatus("pending");
}

Appointment::~Appointment() {}

void Appointment::setID(int newId)
{ id = newId; }
int Appointment::getID() const
{ return id; }

void Appointment::setPatientID(int pId)
{ patientID = pId; }
int Appointment::getPatientID() const 
{ return patientID; }

void Appointment::setDoctorID(int dId) 
{ doctorID = dId; }
int Appointment::getDoctorID() const 
{ return doctorID; }

void Appointment::setDate(const char* newDate)
{ copyStringOffset(date, newDate, 11); }
const char* Appointment::getDate() const
{ return date; }

void Appointment::setTimeSlot(const char* newTimeSlot)
{ copyStringOffset(timeSlot, newTimeSlot, 10); }
const char* Appointment::getTimeSlot() const 
{ return timeSlot; }

void Appointment::setStatus(const char* s)
{ copyStringOffset(status, s, 20); }
const char* Appointment::getStatus() const
{ return status; }

void Appointment::cancelAppointment()
{ setStatus("cancelled"); }

bool Appointment::operator==(const Appointment& other) const 
{
    if (compareStringOffset(this->status, "cancelled") == 0 ||
        compareStringOffset(other.status, "cancelled") == 0) {
        return false;
    }
    return (this->doctorID == other.doctorID) &&
        (compareStringOffset(this->date, other.date) == 0) &&
        (compareStringOffset(this->timeSlot, other.timeSlot) == 0);
}

std::ostream& operator<<(std::ostream& os, const Appointment& appt) 
{
    os << "[Appt ID: " << appt.getID() << "] Doc ID: " << appt.getDoctorID()
        << " | Patient ID: " << appt.getPatientID() << " | Date: " << appt.getDate()
        << " | Slot: " << appt.getTimeSlot() << " | Status: " << appt.getStatus();
    return os;
}