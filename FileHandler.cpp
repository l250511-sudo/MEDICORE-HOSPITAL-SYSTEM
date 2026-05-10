#include "FileHandler.h"
#include <fstream>
#include <iostream>
#include <cstdlib>   // for the use of atoi(), atof()


FileHandler::FileHandler() {}
FileHandler::~FileHandler() {}


// Reads up to the next ',' (or end-of-line / \r) from line[i],
// writes it into buf, null-terminates, then skips past the comma.
void FileHandler::parseField(const char* line, int& i,
    char* buf, int bufSize) const {
    int t = 0;
    while (*(line + i) != ',' &&
        *(line + i) != '\0' &&
        *(line + i) != '\r' &&
        *(line + i) != '\n') {
        if (t < bufSize - 1) {
            *(buf + t) = *(line + i);
            t++;
        }
        i++;
    }
    *(buf + t) = '\0';
    if (*(line + i) == ',') i++;   // skip the comma
}

// For the LAST field on a line: read until \0 / \r / \n.
void FileHandler::parseLastField(const char* line, int& i,
    char* buf, int bufSize) const {
    int t = 0;
    while (*(line + i) != '\0' &&
        *(line + i) != '\r' &&
        *(line + i) != '\n') {
        if (t < bufSize - 1) {
            *(buf + t) = *(line + i);
            t++;
        }
        i++;
    }
    *(buf + t) = '\0';
}


// to load patients we use this one        Format: patient_id,name,age,gender,contact,password,balance

void FileHandler::loadPatients(Storage<Patient*>& db,
    const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw FileNotFoundException();

    char line[512];
    while (file.getline(line, 512)) {
        if (*line == '\0' || *line == '\r') continue;  // skip blank lines

        Patient* p = new Patient();
        char buf[256];
        int i = 0;

        parseField(line, i, buf, 256);   p->setID(std::atoi(buf));
        parseField(line, i, buf, 256);   p->setName(buf);
        parseField(line, i, buf, 256);   p->setAge(std::atoi(buf));
        parseField(line, i, buf, 256);   p->setGender(*buf);      // single char
        parseField(line, i, buf, 256);   p->setContact(buf);
        parseField(line, i, buf, 256);   p->setPassword(buf);
        parseLastField(line, i, buf, 256); p->setBalance(std::atof(buf));

        db.add(p);
    }
    file.close();
}


void FileHandler::savePatients(Storage<Patient*>& db,
    const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    Patient** all = db.getAll();
    int sz = db.size();
    for (int i = 0; i < sz; i++) {
        Patient* p = *(all + i);
        if (!p) continue;
        file << p->getID() << ","
            << p->getName() << ","
            << p->getAge() << ","
            << p->getGender() << ","
            << p->getContact() << ","
            << p->getPassword() << ","
            << p->getBalance() << "\n";
    }
    file.close();
}


//  APPEND PATIENT  (add only one new line)

void FileHandler::appendPatient(Patient* p, const char* filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) return;
    file << p->getID() << ","
        << p->getName() << ","
        << p->getAge() << ","
        << p->getGender() << ","
        << p->getContact() << ","
        << p->getPassword() << ","
        << p->getBalance() << "\n";
    file.close();
}



// for loading doctors we use this  Format: doctor_id,name,specialization,contact,password,fee
void FileHandler::loadDoctors(Storage<Doctor*>& db,
    const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw FileNotFoundException();

    char line[512];
    while (file.getline(line, 512)) {
        if (*line == '\0' || *line == '\r') continue;

        Doctor* d = new Doctor();
        char buf[256];
        int i = 0;

        parseField(line, i, buf, 256);   d->setID(std::atoi(buf));
        parseField(line, i, buf, 256);   d->setName(buf);
        parseField(line, i, buf, 256);   d->setSpecialization(buf);
        parseField(line, i, buf, 256);   d->setContact(buf);
        parseField(line, i, buf, 256);   d->setPassword(buf);
        parseLastField(line, i, buf, 256); d->setFee(std::atof(buf));

        db.add(d);
    }
    file.close();
}

//  SAVE DOCTORS  (full rewrite)

void FileHandler::saveDoctors(Storage<Doctor*>& db,
    const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    Doctor** all = db.getAll();
    int sz = db.size();
    for (int i = 0; i < sz; i++) {
        Doctor* d = *(all + i);
        if (!d) continue;
        file << d->getID() << ","
            << d->getName() << ","
            << d->getSpecialization() << ","
            << d->getContact() << ","
            << d->getPassword() << ","
            << d->getFee() << "\n";
    }
    file.close();
}


//  APPEND DOCTOR

void FileHandler::appendDoctor(Doctor* d, const char* filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) return;
    file << d->getID() << ","
        << d->getName() << ","
        << d->getSpecialization() << ","
        << d->getContact() << ","
        << d->getPassword() << ","
        << d->getFee() << "\n";
    file.close();
}



// for loading admin  Format: admin_id,name,password

void FileHandler::loadAdmin(Admin*& admin, const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw FileNotFoundException();

    char line[256];
    if (file.getline(line, 256)) {         // only one admin record
        admin = new Admin();
        char buf[128];
        int i = 0;

        parseField(line, i, buf, 128);   admin->setID(std::atoi(buf));
        parseField(line, i, buf, 128);   admin->setName(buf);
        parseLastField(line, i, buf, 128); admin->setPassword(buf);
    }
    file.close();
}



// for loading appointments   Format: appointment_id,patient_id,doctor_id,date,time_slot,status

void FileHandler::loadAppointments(Storage<Appointment*>& db,
    const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw FileNotFoundException();

    char line[512];
    while (file.getline(line, 512)) {
        if (*line == '\0' || *line == '\r') continue;

        Appointment* a = new Appointment();
        char buf[128];
        int i = 0;

        parseField(line, i, buf, 128);   a->setID(std::atoi(buf));
        parseField(line, i, buf, 128);   a->setPatientID(std::atoi(buf));
        parseField(line, i, buf, 128);   a->setDoctorID(std::atoi(buf));
        parseField(line, i, buf, 128);   a->setDate(buf);
        parseField(line, i, buf, 128);   a->setTimeSlot(buf);
        parseLastField(line, i, buf, 128); a->setStatus(buf);

        db.add(a);
    }
    file.close();
}
//  SAVE APPOINTMENTS  (full rewrite)

void FileHandler::saveAppointments(Storage<Appointment*>& db,
    const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    Appointment** all = db.getAll();
    int sz = db.size();
    for (int i = 0; i < sz; i++) {
        Appointment* a = *(all + i);
        if (!a) continue;
        file << a->getID() << ","
            << a->getPatientID() << ","
            << a->getDoctorID() << ","
            << a->getDate() << ","
            << a->getTimeSlot() << ","
            << a->getStatus() << "\n";
    }
    file.close();
}


//  APPEND APPOINTMENT

void FileHandler::appendAppointment(Appointment* a, const char* filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) return;
    file << a->getID() << ","
        << a->getPatientID() << ","
        << a->getDoctorID() << ","
        << a->getDate() << ","
        << a->getTimeSlot() << ","
        << a->getStatus() << "\n";
    file.close();
}



// for loading bills   Format: bill_id,patient_id,appointment_id,amount,status,date

void FileHandler::loadBills(Storage<Bill*>& db, const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw FileNotFoundException();

    char line[512];
    while (file.getline(line, 512)) {
        if (*line == '\0' || *line == '\r') continue;

        Bill* b = new Bill();
        char buf[128];
        int i = 0;

        parseField(line, i, buf, 128);   b->setID(std::atoi(buf));
        parseField(line, i, buf, 128);   b->setPatientID(std::atoi(buf));
        parseField(line, i, buf, 128);   b->setAppointmentID(std::atoi(buf));
        parseField(line, i, buf, 128);   b->setAmount(std::atof(buf));
        parseField(line, i, buf, 128);   b->setStatus(buf);
        parseLastField(line, i, buf, 128); b->setDate(buf);

        db.add(b);
    }
    file.close();
}


//  SAVE BILLS  (full rewrite)

void FileHandler::saveBills(Storage<Bill*>& db, const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    Bill** all = db.getAll();
    int sz = db.size();
    for (int i = 0; i < sz; i++) {
        Bill* b = *(all + i);
        if (!b) continue;
        file << b->getID() << ","
            << b->getPatientID() << ","
            << b->getAppointmentID() << ","
            << b->getAmount() << ","
            << b->getStatus() << ","
            << b->getDate() << "\n";
    }
    file.close();
}


//  APPEND BILL

void FileHandler::appendBill(Bill* b, const char* filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) return;
    file << b->getID() << ","
        << b->getPatientID() << ","
        << b->getAppointmentID() << ","
        << b->getAmount() << ","
        << b->getStatus() << ","
        << b->getDate() << "\n";
    file.close();
}


//  LOADING  PRESCRIPTIONS
//  Format: prescription_id,appointment_id,patient_id,
//          doctor_id,date,medicines,notes over here ; used as a seperator 

void FileHandler::loadPrescriptions(Storage<Prescription*>& db,
    const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw FileNotFoundException();

    char line[1024];
    while (file.getline(line, 1024)) {
        if (*line == '\0' || *line == '\r') continue;

        Prescription* pr = new Prescription();
        char buf[512];
        int i = 0;

        parseField(line, i, buf, 512);   pr->setID(std::atoi(buf));
        parseField(line, i, buf, 512);   pr->setAppointmentID(std::atoi(buf));
        parseField(line, i, buf, 512);   pr->setPatientID(std::atoi(buf));
        parseField(line, i, buf, 512);   pr->setDoctorID(std::atoi(buf));
        parseField(line, i, buf, 512);   pr->setDate(buf);
        parseField(line, i, buf, 512);   pr->setMedicines(buf);
        parseLastField(line, i, buf, 512); pr->setNotes(buf);

        db.add(pr);
    }
    file.close();
}


//  SAVE PRESCRIPTIONS  (full rewrite)

void FileHandler::savePrescriptions(Storage<Prescription*>& db,
    const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    Prescription** all = db.getAll();
    int sz = db.size();
    for (int i = 0; i < sz; i++) {
        Prescription* pr = *(all + i);
        if (!pr) continue;
        file << pr->getID() << ","
            << pr->getAppointmentID() << ","
            << pr->getPatientID() << ","
            << pr->getDoctorID() << ","
            << pr->getDate() << ","
            << pr->getMedicines() << ","
            << pr->getNotes() << "\n";
    }
    file.close();
}


//  APPEND PRESCRIPTION

void FileHandler::appendPrescription(Prescription* pr,
    const char* filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) return;
    file << pr->getID() << ","
        << pr->getAppointmentID() << ","
        << pr->getPatientID() << ","
        << pr->getDoctorID() << ","
        << pr->getDate() << ","
        << pr->getMedicines() << ","
        << pr->getNotes() << "\n";
    file.close();
}


//  SECURITY LOG


// Appends: timestamp,role,entered_id,result
void FileHandler::logSecurityEvent(const char* timestamp,
    const char* role,
    const char* enteredID,
    const char* result,
    const char* filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) return;
    file << timestamp << "," << role << "," << enteredID << "," << result << "\n";
    file.close();
}

// Reads and prints every line of the security log.
void FileHandler::displaySecurityLog(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "No security events logged.\n";
        return;
    }
    char line[512];
    bool empty = true;
    while (file.getline(line, 512)) {
        if (*line == '\0' || *line == '\r') continue;
        std::cout << line << "\n";
        empty = false;
    }
    if (empty) std::cout << "No security events logged.\n";
    file.close();
}


//  DISCHARGE PATIENT
// reomves the data from the current file and moves to other file for info 

void FileHandler::dischargePatient(int patientID,
    Storage<Patient*>& patients,
    Storage<Appointment*>& appointments,
    Storage<Bill*>& bills,
    Storage<Prescription*>& prescriptions,
    const char* patientFile,
    const char* appointmentFile,
    const char* billFile,
    const char* prescriptionFile,
    const char* dischargedFile)
{
    // 
    std::ofstream arch(dischargedFile, std::ios::app);
    if (arch.is_open()) {
       // Find the patient in the database and write their profile details to the archive
        Patient** allP = patients.getAll();
        for (int i = 0; i < patients.size(); i++) {
            Patient* p = *(allP + i);
            if (p && p->getID() == patientID) {
                arch << p->getID() << ","
                    << p->getName() << ","
                    << p->getAge() << ","
                    << p->getGender() << ","
                    << p->getContact() << ","
                    << p->getPassword() << ","
                    << p->getBalance() << "\n";
                break;
            }
        }

        // search all appointments and archeive that belong to this  pesron 
        Appointment** allA = appointments.getAll();
        for (int i = 0; i < appointments.size(); i++) {
            Appointment* a = *(allA + i);
            if (a && a->getPatientID() == patientID) {
                arch << a->getID() << ","
                    << a->getPatientID() << ","
                    << a->getDoctorID() << ","
                    << a->getDate() << ","
                    << a->getTimeSlot() << ","
                    << a->getStatus() << "\n";
            }
        }

        // serching for the bills of thei patient 
        Bill** allB = bills.getAll();
        for (int i = 0; i < bills.size(); i++) {
            Bill* b = *(allB + i);
            if (b && b->getPatientID() == patientID) {
                arch << b->getID() << ","
                    << b->getPatientID() << ","
                    << b->getAppointmentID() << ","
                    << b->getAmount() << ","
                    << b->getStatus() << ","
                    << b->getDate() << "\n";
            }
        }

        // search all prescriptions and s a ve those that belong to this patient 
        Prescription** allPr = prescriptions.getAll();
        for (int i = 0; i < prescriptions.size(); i++) {
            Prescription* pr = *(allPr + i);
            if (pr && pr->getPatientID() == patientID) {
                arch << pr->getID() << ","
                    << pr->getAppointmentID() << ","
                    << pr->getPatientID() << ","
                    << pr->getDoctorID() << ","
                    << pr->getDate() << ","
                    << pr->getMedicines() << ","
                    << pr->getNotes() << "\n";
            }
        }
        arch.close();
    }

    // remove data from the storage and since it has been stored ew delte it from the active array 
    patients.removeByID(patientID);

    // Remove appointments belonging to this patient
    Appointment** allA = appointments.getAll();
    int asz = appointments.size();
    for (int i = 0; i < asz; i++) {
        Appointment* a = *(allA + i);
        if (a && a->getPatientID() == patientID)
            appointments.removeByID(a->getID());
    }

    // Remove bills belonging to this patient
    Bill** allB = bills.getAll();
    int bsz = bills.size();
    for (int i = 0; i < bsz; i++) {
        Bill* b = *(allB + i);
        if (b && b->getPatientID() == patientID)
            bills.removeByID(b->getID());
    }

    // Remove prescriptions belonging to this patient
    Prescription** allPr = prescriptions.getAll();
    int prsz = prescriptions.size();
    for (int i = 0; i < prsz; i++) {
        Prescription* pr = *(allPr + i);
        if (pr && pr->getPatientID() == patientID)
            prescriptions.removeByID(pr->getID());
    }

    // now we rewrite all the files without this patient 
    savePatients(patients, patientFile);
    saveAppointments(appointments, appointmentFile);
    saveBills(bills, billFile);
    savePrescriptions(prescriptions, prescriptionFile);
}

// ════════════════════════════════════════════════════════════
//  MAX-ID HELPERS
//  Each one opens the file, reads every first field (ID),
//  tracks the maximum, and returns it.  Returns 0 if empty.
// ════════════════════════════════════════════════════════════

// Reusable inner logic — reads the first CSV field of each line as an int.
static int findMaxID(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return 0;

    int maxID = 0;
    char line[512];
    while (file.getline(line, 512)) {
        if (*line == '\0' || *line == '\r') continue;
        char buf[32];
        int i = 0, t = 0;
        while (*(line + i) != ',' && *(line + i) != '\0') {
            *(buf + t) = *(line + i);
            t++; i++;
        }
        *(buf + t) = '\0';
        int id = std::atoi(buf);
        if (id > maxID) maxID = id;
    }
    file.close();
    return maxID;
}

int FileHandler::maxPatientID(const char* f) { return findMaxID(f); }
int FileHandler::maxDoctorID(const char* f) { return findMaxID(f); }
int FileHandler::maxAppointmentID(const char* f) { return findMaxID(f); }
int FileHandler::maxBillID(const char* f) { return findMaxID(f); }
int FileHandler::maxPrescriptionID(const char* f) { return findMaxID(f); }