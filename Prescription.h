#pragma once

class Prescription {
private:
    int id;
    int appointmentID;
    int patientID;
    int doctorID;
    char date[11];
    char medicines[500];
    char notes[300];

public:
    Prescription();
    ~Prescription();

    void setID(int newId);
    int getID() const;

    void setAppointmentID(int aId);
    int getAppointmentID() const;

    void setPatientID(int pId);
    int getPatientID() const;

    void setDoctorID(int dId);
    int getDoctorID() const;

    void setDate(const char* newDate);
    const char* getDate() const;

    void setMedicines(const char* meds);
    const char* getMedicines() const;

    void setNotes(const char* newNotes);
    const char* getNotes() const;
};
