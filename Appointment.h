#pragma once
#include <iostream>

class Appointment {
private:
    int id;
    int patientID;
    int doctorID;
    char date[11];
    char timeSlot[10];
    char status[20];

public:
    Appointment();
    ~Appointment();

    void setID(int newId);
    int getID() const;

    void setPatientID(int id);
    int getPatientID() const;

    void setDoctorID(int id);
    int getDoctorID() const;

    void setDate(const char* newDate);
    const char* getDate() const;

    void setTimeSlot(const char* newTimeSlot);
    const char* getTimeSlot() const;

    void setStatus(const char* s);
    const char* getStatus() const;

    void cancelAppointment();
    bool operator==(const Appointment& other) const;
};

std::ostream& operator<<(std::ostream& os, const Appointment& appt);

