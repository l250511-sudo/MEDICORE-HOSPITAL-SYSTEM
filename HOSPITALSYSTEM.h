#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include "FileHandler.h"
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Validator.h"
#include "SlotUnavailableException.h"
#include "InsufficientFundsException.h"
#include "InvalidInputException.h"

class HospitalSystem {
private:
    sf::RenderWindow   window;
    sf::Font           font;
    sf::RectangleShape headerBar;
    sf::RectangleShape buttonBase;
    sf::Text           title;

    // ── DATABASES ──
    Storage<Patient*>      patientDB;
    Storage<Doctor*>       doctorDB;
    Storage<Appointment*>  apptDB;
    Storage<Bill*>         billDB;
    Storage<Prescription*> rxDB;

    Admin* admin;
    FileHandler fh;

    int      appState;
    int      menuSelection;

    // ── ACTIVE SESSIONS ──
    Patient* activePatient;   // logged-in patient
    Doctor* activeDoctor;    // logged-in doctor

    // ── SHARED LOGIN BUFFERS (reused across all portals) ──
    char loginID[20];
    char loginPass[50];
    int  idLen, passLen, loginStep;
    bool loginFailed;

    // ── PER-PORTAL SECURITY ──
    bool accountLocked;        int failedAttempts;        // Patient
    bool doctorAccountLocked;  int doctorFailedAttempts;  // Doctor
    bool adminAccountLocked;   int adminFailedAttempts;   // Admin

    // ── SHORT INPUT BUFFERS (IDs, age, fee, gender) ──
    char inputBuf1[20];
    char inputBuf2[50];
    char inputBuf3[15];
    char inputBuf4[20];
    int  inputLen1, inputLen2, inputLen3, inputLen4;

    // ── LONG INPUT BUFFERS (names, medicines, notes) ──
    char longBuf1[500];
    char longBuf2[300];
    int  longLen1, longLen2;

    int  bookingStep;
    char statusMsg[200];
    bool actionSuccess;

    // ── PRIVATE HELPERS ──
    void resetLoginSystem();
    void resetInputBuffers();
    void resetLongBuffers();
    void copyMsg(const char* src);

    // ── UI DRAW HELPERS ──
    void drawGradientBackground();
    void drawCard(float x, float y, float w, float h, sf::Color color);
    void drawElegantButton(float x, float y, float w, float h, const char* label, bool isSelected);
    void drawButton(float x, float y, const char* label, bool isSelected);
    void drawInputField(float x, float y, float w, const char* label, const char* value, bool active);
    void drawLongInputField(float x, float y, float w, float h, const char* label, const char* value, bool active);
    void drawHeader();
    void drawPageHeader(const char* pageTitle);
    void drawStatusMessage();
    void drawBackHint();
    void renderLoginScreen(const char* portalName);

    static int cmpStr(const char* a, const char* b);

public:
    HospitalSystem();
    ~HospitalSystem();
    void bootDatabases();
    void setupUI();
    void runUI();
    void handleEvents();
    void render();
};