#pragma once

class Bill {
private:
    int id;             // Renamed from billID to match FileHandler
    int appointmentID;
    int patientID;
    double amount;
    char status[20];    // Replaced bool isPaid with string array
    char date[11];      // Added to track the billing date

public:
    Bill();
    ~Bill();

    void setID(int newId);
    int getID() const;

    void setAppointmentID(int id);
    int getAppointmentID() const;

    void setPatientID(int id);
    int getPatientID() const;

    void setAmount(double amt);
    double getAmount() const;

    void setStatus(const char* s);
    const char* getStatus() const;

    void setDate(const char* newDate);
    const char* getDate() const;
};