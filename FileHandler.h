#pragma once
#include <exception>
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileNotFoundException.h"




class FileHandler {
private:
    // Private helpers for offset-based string parsing
    void parseField(const char* line, int& i, char* buf, int bufSize) const;
    void parseLastField(const char* line, int& i, char* buf, int bufSize) const;

public:
    FileHandler();
    ~FileHandler();

    // Patients
    void loadPatients(Storage<Patient*>& db, const char* filename);
    void savePatients(Storage<Patient*>& db, const char* filename);
    void appendPatient(Patient* p, const char* filename);

    // Doctors
    void loadDoctors(Storage<Doctor*>& db, const char* filename);
    void saveDoctors(Storage<Doctor*>& db, const char* filename);
    void appendDoctor(Doctor* d, const char* filename);

    // Admin
    void loadAdmin(Admin*& admin, const char* filename);

    // Appointments
    void loadAppointments(Storage<Appointment*>& db, const char* filename);
    void saveAppointments(Storage<Appointment*>& db, const char* filename);
    void appendAppointment(Appointment* a, const char* filename);

    // Bills
    void loadBills(Storage<Bill*>& db, const char* filename);
    void saveBills(Storage<Bill*>& db, const char* filename);
    void appendBill(Bill* b, const char* filename);

    // Prescriptions
    void loadPrescriptions(Storage<Prescription*>& db, const char* filename);
    void savePrescriptions(Storage<Prescription*>& db, const char* filename);
    void appendPrescription(Prescription* pr, const char* filename);

    // Security Log
    void logSecurityEvent(const char* timestamp, const char* role, const char* enteredID, const char* result, const char* filename);
    void displaySecurityLog(const char* filename);

    // Discharge / Archive
    void dischargePatient(int patientID,
        Storage<Patient*>& patients,
        Storage<Appointment*>& appointments,
        Storage<Bill*>& bills,
        Storage<Prescription*>& prescriptions,
        const char* patientFile,
        const char* appointmentFile,
        const char* billFile,
        const char* prescriptionFile,
        const char* dischargedFile);

    // Auto-Increment ID Helpers
    int maxPatientID(const char* f);
    int maxDoctorID(const char* f);
    int maxAppointmentID(const char* f);
    int maxBillID(const char* f);
    int maxPrescriptionID(const char* f);
};
