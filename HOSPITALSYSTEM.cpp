#include "HospitalSystem.h"
#include <iostream>
#include <cstdlib>

// HELPLERS FUNCTIONS 
int HospitalSystem::cmpStr(const char* a, const char* b) {
    int i = 0;
    while (*(a + i) != '\0' && *(a + i) == *(b + i)) i++;
    return *(a + i) - *(b + i);
}

void HospitalSystem::copyMsg(const char* src) {
    int k = 0;
    while (*(src + k) != '\0' && k < 199) { *(statusMsg + k) = *(src + k); k++; }
    *(statusMsg + k) = '\0';
}


HospitalSystem::HospitalSystem() : window(sf::VideoMode(1280, 720), "MediCore Hospital System") {
    window.setFramerateLimit(60);
    admin = nullptr;
    activePatient = nullptr;
    activeDoctor = nullptr;
    appState = 0;
    menuSelection = 0;
    failedAttempts = 0;   accountLocked = false;
    doctorFailedAttempts = 0;   doctorAccountLocked = false;
    adminFailedAttempts = 0;   adminAccountLocked = false;
    bookingStep = 0;   actionSuccess = false;
    *(statusMsg + 0) = '\0';
    resetLoginSystem();
    resetInputBuffers();
    resetLongBuffers();
    setupUI();
}

HospitalSystem::~HospitalSystem() {
    if (admin != nullptr) { delete admin; admin = nullptr; }
    for (int i = 0; i < apptDB.size(); i++) 
        if (*(apptDB.getAll() + i)) 
        {
            delete* (apptDB.getAll() + i);
        }
    for (int i = 0; i < billDB.size(); i++)
        if (*(billDB.getAll() + i))   
        {
            delete* (billDB.getAll() + i);
        }
    for (int i = 0; i < rxDB.size(); i++) 
        if (*(rxDB.getAll() + i))     
        {
            delete* (rxDB.getAll() + i);
        }
    for (int i = 0; i < patientDB.size(); i++)
        if (*(patientDB.getAll() + i))
        {
            delete* (patientDB.getAll() + i);
        }
    for (int i = 0; i < doctorDB.size(); i++)
        if (*(doctorDB.getAll() + i)) 
        {
            delete* (doctorDB.getAll() + i);
        }
}


//RESET HELPERSS  since we are ont allowed to use strings these are used for clearing the buffer 


void HospitalSystem::resetLoginSystem() {
    loginStep = 0; idLen = 0; passLen = 0; loginFailed = false;
    for (int i = 0; i < 20; i++)
        *(loginID + i) = '\0';
    for (int i = 0; i < 50; i++)
        *(loginPass + i) = '\0';
}

void HospitalSystem::resetInputBuffers() {
    inputLen1 = inputLen2 = inputLen3 = inputLen4 = 0;
    bookingStep = 0; actionSuccess = false;
    for (int i = 0; i < 20; i++)
        *(inputBuf1 + i) = '\0';
    for (int i = 0; i < 50; i++)
        *(inputBuf2 + i) = '\0';
    for (int i = 0; i < 15; i++)
        *(inputBuf3 + i) = '\0';
    for (int i = 0; i < 20; i++)
        *(inputBuf4 + i) = '\0';
    for (int i = 0; i < 200; i++)
        *(statusMsg + i) = '\0';
}

void HospitalSystem::resetLongBuffers() {
    longLen1 = longLen2 = 0;
    for (int i = 0; i < 500; i++) *(longBuf1 + i) = '\0';
    for (int i = 0; i < 300; i++) *(longBuf2 + i) = '\0';
}


// SETUP & BOOT


void HospitalSystem::setupUI() {
    if (!font.loadFromFile("ONEDAY.otf")) std::cout << "Font error!\n";
    buttonBase.setSize(sf::Vector2f(400.f, 55.f));
    buttonBase.setOutlineThickness(2.f);
    title.setFont(font);
    title.setFillColor(sf::Color::White);
    title.setCharacterSize(42);
 title.setString("MEDICORE HEALTHCARE");
    title.setPosition(40.f, 22.f);
}      // set up the formant and the template for the text and buttons 
// for loading databases 
void HospitalSystem::bootDatabases() {
    fh.loadPatients(patientDB, "patients.txt");
    fh.loadDoctors(doctorDB, "doctors.txt");
    fh.loadAdmin(admin, "admin.txt");
    fh.loadAppointments(apptDB, "appointments.txt");
    fh.loadBills(billDB, "bills.txt");
    fh.loadPrescriptions(rxDB, "prescription.txt");
}


//  UI HELPERS


void HospitalSystem::drawGradientBackground() {
    int strips = 72; float sh = 720.0f / strips;
    for (int i = 0; i < strips; i++) {
        float t = (float)i / strips;
        sf::RectangleShape s(sf::Vector2f(1280.f, sh + 1.f));
        s.setPosition(0.f, i * sh);
        s.setFillColor(sf::Color((sf::Uint8)(8 + t * 7), (sf::Uint8)(15 + t * 45), (sf::Uint8)(45 + t * 35)));
        window.draw(s);
    }
}

void HospitalSystem::drawCard(float x, float y, float w, float h, sf::Color color) {
    sf::RectangleShape sh(sf::Vector2f(w, h)); 
    sh.setPosition(x + 4, y + 4);
    sh.setFillColor(sf::Color(0, 0, 0, 80));
    window.draw(sh);
    sf::RectangleShape c(sf::Vector2f(w, h));
    c.setPosition(x, y);
    c.setFillColor(color);
    window.draw(c);
}

void HospitalSystem::drawElegantButton(float x, float y, float w, float h, const char* label, bool sel) {
    sf::RectangleShape sh(sf::Vector2f(w, h));   // for the shadow effeect 
    sh.setPosition(x + 3, y + 3);
    sh.setFillColor(sf::Color(0, 0, 0, 100)); 
    window.draw(sh);
    sf::RectangleShape btn(sf::Vector2f(w, h));
    btn.setPosition(x, y);
    btn.setFillColor(sel ? sf::Color(0, 145, 190) : sf::Color(18, 32, 68));  // for the hover effect 
    btn.setOutlineThickness(2.f);
    btn.setOutlineColor(sel ? sf::Color(0, 210, 255) : sf::Color(35, 60, 110));
    window.draw(btn);
    if (sel)        // if the cutton is slected make  a line on the left 
    {
        sf::RectangleShape ac(sf::Vector2f(5.f, h));
        ac.setPosition(x, y); ac.setFillColor(sf::Color(0, 255, 220)); window.draw(ac);
    }
    sf::Text t;   // for centering the text 
    t.setFont(font);
    t.setString(label); 
    t.setCharacterSize(18);
    t.setFillColor(sf::Color::White);
    sf::FloatRect tb = t.getLocalBounds();
    t.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    t.setPosition(x + w / 2.f, y + h / 2.f); window.draw(t);
}

void HospitalSystem::drawButton(float x, float y, const char* label, bool sel) {
    drawElegantButton(x, y, 400.f, 52.f, label, sel);
}
// draws  a label text input and interactive design
void HospitalSystem::drawInputField(float x, float y, float w, const char* label, const char* value, bool active) {
    sf::Text lbl; lbl.setFont(font); lbl.setString(label);
    lbl.setCharacterSize(15);
    lbl.setFillColor(active ? sf::Color(0, 210, 255) : sf::Color(130, 165, 200));
    lbl.setPosition(x, y); 
    window.draw(lbl);
    sf::RectangleShape f(sf::Vector2f(w, 40.f));
    f.setPosition(x, y + 20);
    f.setFillColor(sf::Color(8, 18, 48));
    f.setOutlineThickness(2.f);
    f.setOutlineColor(active ? sf::Color(0, 200, 240) : sf::Color(35, 58, 100));
    window.draw(f);
    char disp[82]; int vl = 0;
    while (*(value + vl) != '\0')
        vl++;
    for (int i = 0; i < vl && i < 80; i++) 
        *(disp + i) = *(value + i);
    if (active) 
    { *(disp + vl) = '|'; *(disp + vl + 1) = '\0'; }
    else *(disp + vl) = '\0';
    sf::Text v;
    v.setFont(font);
    v.setString(disp);
    v.setCharacterSize(18);
    v.setFillColor(sf::Color::White);
    v.setPosition(x + 8, y + 25);
    window.draw(v);
}
// take inputs for the long designs 
void HospitalSystem::drawLongInputField(float x, float y, float w, float h, const char* label, const char* value, bool active) {
    sf::Text lbl; lbl.setFont(font);
    lbl.setString(label);
    lbl.setCharacterSize(15);
    lbl.setFillColor(active ? sf::Color(0, 210, 255) : sf::Color(130, 165, 200));
    lbl.setPosition(x, y);
    window.draw(lbl);
    sf::RectangleShape f(sf::Vector2f(w, h));
    f.setPosition(x, y + 20);
    f.setFillColor(sf::Color(8, 18, 48)); 
    f.setOutlineThickness(2.f);
    f.setOutlineColor(active ? sf::Color(0, 200, 240) : sf::Color(35, 58, 100));
    window.draw(f);
    int vl = 0; while (*(value + vl) != '\0')
        vl++;
    int start = vl > 55 ? vl - 55 : 0;
    char disp[60]; int di = 0;
    for (int i = start; i < vl && di < 58; i++)
        *(disp + di++) = *(value + i);
    if (active) 
    { *(disp + di) = '|'; *(disp + di + 1) = '\0'; }
    else *(disp + di) = '\0';
    sf::Text v;
    v.setFont(font);
    v.setString(disp);
    v.setCharacterSize(16);
    v.setFillColor(sf::Color::White);
    v.setPosition(x + 8, y + 28);
    window.draw(v);
}

void HospitalSystem::drawHeader() {
    drawCard(0, 0, 1280, 100, sf::Color(5, 10, 30));
    sf::RectangleShape al(sf::Vector2f(1280.f, 3.f));
    al.setPosition(0, 97);
    al.setFillColor(sf::Color(0, 200, 240));
    window.draw(al);
    sf::Text sh = title;
    sh.setFillColor(sf::Color(0, 0, 0, 100));
    sh.setPosition(42.f, 24.f);
    window.draw(sh);
    window.draw(title);
    sf::Text sub;
    sub.setFont(font);
    sub.setCharacterSize(15);
    sub.setFillColor(sf::Color(70, 160, 205));
    sub.setString("Your Health, Our Priority");
    sub.setPosition(42.f, 70.f); 
    window.draw(sub);
}

void HospitalSystem::drawPageHeader(const char* pageTitle) {
    drawHeader();
    sf::Text pt; pt.setFont(font);
    pt.setCharacterSize(24);
    pt.setFillColor(sf::Color(0, 200, 240));
    pt.setString(pageTitle);
    pt.setPosition(50, 110);
    window.draw(pt);
    sf::RectangleShape sep(sf::Vector2f(1180.f, 2.f));
    sep.setPosition(50, 148);
    sep.setFillColor(sf::Color(0, 200, 240, 100)); 
    window.draw(sep);
}

void HospitalSystem::drawStatusMessage() {
    if (*(statusMsg + 0) == '\0') return;
    sf::RectangleShape bar(sf::Vector2f(1180.f, 44.f));
    bar.setPosition(50, 646);
    bar.setFillColor(actionSuccess ? sf::Color(0, 88, 52) : sf::Color(108, 16, 16));
    window.draw(bar);
    sf::Text msg;
    msg.setFont(font);
    msg.setString(statusMsg);
    msg.setCharacterSize(16);
    msg.setFillColor(sf::Color::White);
    msg.setPosition(62, 658);
    window.draw(msg);
}

void HospitalSystem::drawBackHint() {
    sf::Text h; h.setFont(font);
    h.setCharacterSize(14);
    h.setFillColor(sf::Color(75, 115, 165));
    h.setString("ESC = Go Back");
    h.setPosition(50, 700); window.draw(h);
}

void HospitalSystem::renderLoginScreen(const char* portalName) {
    drawHeader();
    drawCard(390, 115, 500, 455, sf::Color(12, 22, 55));
    sf::Text lt; lt.setFont(font);
    lt.setCharacterSize(22);
    lt.setFillColor(sf::Color(0, 200, 240));
    lt.setString(portalName);
    sf::FloatRect lr = lt.getLocalBounds();
    lt.setOrigin(lr.left + lr.width / 2.f, 0);
    lt.setPosition(640.f, 130.f);
    window.draw(lt);
    sf::RectangleShape sep(sf::Vector2f(460.f, 2.f));
    sep.setPosition(410, 166);
    sep.setFillColor(sf::Color(0, 200, 240, 100)); 
    window.draw(sep);
    char mask[51];
    for (int i = 0; i < passLen; i++) 
        *(mask + i) = '*';
    *(mask + passLen) = '\0';
    drawInputField(415, 192, 450, "ID", loginID, loginStep == 0);
    drawInputField(415, 290, 450, "Password", mask, loginStep == 1);
    bool locked = (appState == 1 && accountLocked) || (appState == 30 && doctorAccountLocked) || (appState == 40 && adminAccountLocked);
    if (locked || loginFailed) {
        drawCard(390, 408, 500, 46, sf::Color(108, 15, 15));
        sf::Text e; e.setFont(font);
        e.setCharacterSize(15);
        e.setFillColor(sf::Color::White);
        e.setString(locked ? "Account locked. Contact administrator." : "Invalid credentials. Try again.");
        e.setPosition(405, 420);
        window.draw(e);
    }
    sf::Text hint; hint.setFont(font);
    hint.setCharacterSize(13);
    hint.setFillColor(sf::Color(75, 115, 165));
    hint.setString("ENTER to confirm  |  ESC to go back");
    hint.setPosition(415, 482);
    window.draw(hint);
}


//  EVENT HANDLING 


void HospitalSystem::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        
        //  MAIN MENU
       
        if (appState == 0 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up)
            { menuSelection--;
             if (menuSelection < 0) menuSelection = 3;
            }
            if (event.key.code == sf::Keyboard::Down)
            { menuSelection++;
             if (menuSelection > 3) menuSelection = 0;
            }
            if (event.key.code == sf::Keyboard::Enter) {
                resetLoginSystem(); loginFailed = false;
                if (menuSelection == 0) appState = 1;
                else if (menuSelection == 1) appState = 30;
                else if (menuSelection == 2) appState = 40;
                else if (menuSelection == 3) window.close();
            }
        }

        
        // PATIENT PORTAL 
       

        //  LOGIN LOGIC FOR THE PATIENT 
        else if (appState == 1 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) 
            { 
                appState = 0; resetLoginSystem(); 
            }
        }
        else if (appState == 1 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') { if (loginStep == 0 && idLen > 0) loginID[--idLen] = '\0'; else if (loginStep == 1 && passLen > 0) loginPass[--passLen] = '\0'; }
            else if (c == '\r' || c == '\n') {
                if (accountLocked) return;
                if (loginStep == 0 && idLen > 0) loginStep = 1;
                else if (loginStep == 1 && passLen > 0) {
                    Patient* p = patientDB.findByID(std::atoi(loginID));
                    bool match = false;
                    if (p != nullptr) {
                        const char* rp = p->getPassword(); 
                        int i = 0; match = true;
                        while (*(loginPass + i) != '\0')
                        {
                            if (*(rp + i) != *(loginPass + i)) 
                            { match = false; break; }
                            i++; }
                        if (match && *(rp + i) != '\0' && *(rp + i) != '\r' && *(rp + i) != '\n') match = false;
                    }
                    if (p != nullptr && match) {
                        activePatient = p;
                        appState = 11; 
                        menuSelection = 0;
                        failedAttempts = 0;
                        resetInputBuffers();
                        resetLongBuffers();
                    }
                    else {
                        failedAttempts++;
                        if (failedAttempts >= 3) {
                            accountLocked = true;
                            std::ofstream log("security_log.txt", std::ios::app);
                            log << "Patient," << loginID << ",LOCKED\n"; log.close();
                        }
                        else { loginFailed = true; loginStep = 0; idLen = 0; passLen = 0; *(loginID + 0) = '\0'; *(loginPass + 0) = '\0'; }
                    }
                }
            }
            else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                if (loginStep == 0 && idLen < 15 && c >= '0' && c <= '9') { loginID[idLen++] = c; loginID[idLen] = '\0'; }
                else if (loginStep == 1 && passLen < 15) { loginPass[passLen++] = c; loginPass[passLen] = '\0'; }
            }
        }

        // PAITENT DASHBOARD NAVIAGATION 
        else if (appState == 11 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) { menuSelection--; if (menuSelection < 0) menuSelection = 7; }
            if (event.key.code == sf::Keyboard::Down) { menuSelection++; if (menuSelection > 7) menuSelection = 0; }
            if (event.key.code == sf::Keyboard::Enter) {
                resetInputBuffers(); resetLongBuffers();
                if (menuSelection == 0) appState = 20;
                else if (menuSelection == 1) appState = 21;
                else if (menuSelection == 2) appState = 22;
                else if (menuSelection == 3) appState = 23;
                else if (menuSelection == 4) appState = 24;
                else if (menuSelection == 5) appState = 25;
                else if (menuSelection == 6) appState = 26;
                else if (menuSelection == 7) { activePatient = nullptr; resetLoginSystem(); appState = 0; menuSelection = 0; }
            }
        }

        // PATIENT BOOK APPOINTMENT 
        else if (appState == 20 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape)
            { appState = 11; menuSelection = 0; }
        }
        else if (appState == 20 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') {
                if (bookingStep == 0 && inputLen1 > 0) inputBuf1[--inputLen1] = '\0';
                else if (bookingStep == 1 && inputLen2 > 0) inputBuf2[--inputLen2] = '\0';
                else if (bookingStep == 2 && inputLen3 > 0) inputBuf3[--inputLen3] = '\0';
            }
            else if (c == '\r' || c == '\n') {
                if (bookingStep == 0) {
                    if (inputLen1 == 0) return;
                    int did = std::atoi(inputBuf1);
                    if (!Validator::isValidID(did) || doctorDB.findByID(did) == nullptr) { copyMsg("Doctor not found. Enter a valid Doctor ID."); actionSuccess = false; inputLen1 = 0; *(inputBuf1 + 0) = '\0'; }
                    else { *(statusMsg + 0) = '\0'; bookingStep = 1; }
                }
                else if (bookingStep == 1) {
                    if (inputLen2 == 0) return;
                    if (!Validator::isValidDate(inputBuf2))
                    { copyMsg("Invalid date. Use DD-MM-YYYY.");
                    actionSuccess = false;
                    inputLen2 = 0;
                    *(inputBuf2 + 0) = '\0'; }
                    else { *(statusMsg + 0) = '\0'; bookingStep = 2; }
                }
                else if (bookingStep == 2) {
                    if (inputLen3 == 0) return;
                    int did = std::atoi(inputBuf1);
                    Doctor* doc = doctorDB.findByID(did);
                    bool taken = false;
                    for (int i = 0; i < apptDB.size(); i++) {
                        Appointment* a = *(apptDB.getAll() + i); if (!a) continue;
                        if (a->getDoctorID() == did && cmpStr(a->getDate(), inputBuf2) == 0 && cmpStr(a->getTimeSlot(), inputBuf3) == 0 && cmpStr(a->getStatus(), "cancelled") != 0) { taken = true; break; }
                    }
                    try {
                        if (taken) throw SlotUnavailableException();
                        if (doc && activePatient->getBalance() < doc->getFee()) throw InsufficientFundsException();
                        Appointment* na = new Appointment();
                        int nid = fh.maxAppointmentID("appointments.txt") + 1;
                        na->setID(nid);
                        na->setPatientID(activePatient->getID());
                        na->setDoctorID(did);
                        na->setDate(inputBuf2);
                        na->setTimeSlot(inputBuf3);
                        na->setStatus("pending");
                        apptDB.add(na);
                        fh.appendAppointment(na, "appointments.txt");
                        *activePatient -= doc->getFee();
                        fh.savePatients(patientDB, "patients.txt");
                        Bill* nb = new Bill();
                        int bid = fh.maxBillID("bills.txt") + 1;
                        nb->setID(bid);
                        nb->setAppointmentID(nid);
                        nb->setPatientID(activePatient->getID());
                        nb->setAmount(doc->getFee());
                        nb->setDate(inputBuf2);
                        nb->setStatus("Paid");
                        billDB.add(nb);
                        fh.appendBill(nb, "bills.txt");
                        copyMsg("Appointment booked! Fee deducted.");
                        actionSuccess = true; bookingStep = 3;
                    }
                    catch (SlotUnavailableException& e)
                    { copyMsg(e.what()); actionSuccess = false; inputLen3 = 0; *(inputBuf3 + 0) = '\0'; }
                    catch (InsufficientFundsException& e)
                    { copyMsg(e.what()); actionSuccess = false; }
                }
            }
            else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                if (bookingStep == 0 && inputLen1 < 19 && c >= '0' && c <= '9') { inputBuf1[inputLen1++] = c; inputBuf1[inputLen1] = '\0'; }
                else if (bookingStep == 1 && inputLen2 < 10 && (c >= '0' && c <= '9' || c == '-')) { inputBuf2[inputLen2++] = c; inputBuf2[inputLen2] = '\0'; }
                else if (bookingStep == 2 && inputLen3 < 9) { inputBuf3[inputLen3++] = c; inputBuf3[inputLen3] = '\0'; }
            }
        }

        // PATIENT CANCEL APPOINTMENT 
        else if (appState == 21 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) { appState = 11; menuSelection = 0; }
        }
        else if (appState == 21 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') { if (inputLen1 > 0) inputBuf1[--inputLen1] = '\0'; }
            else if (c == '\r' || c == '\n') {
                if (inputLen1 == 0) return;
                Appointment* a = apptDB.findByID(std::atoi(inputBuf1));
                if (!a || a->getPatientID() != activePatient->getID())
                { copyMsg("Appointment not found or not yours.");
                actionSuccess = false; }
                else if (cmpStr(a->getStatus(), "cancelled") == 0) 
                { 
                    copyMsg("Already cancelled.");
                actionSuccess = false;
                }
                else {
                    Doctor* doc = doctorDB.findByID(a->getDoctorID());
                    if (doc) *activePatient += doc->getFee();
                    a->cancelAppointment();
                    fh.saveAppointments(apptDB, "appointments.txt");
                    fh.savePatients(patientDB, "patients.txt");
                    copyMsg("Appointment cancelled. Fee refunded.");
                    actionSuccess = true;
                }
                inputLen1 = 0; *(inputBuf1 + 0) = '\0';
            }
            else if (c >= '0' && c <= '9' && inputLen1 < 19) { inputBuf1[inputLen1++] = c; inputBuf1[inputLen1] = '\0'; }
        }

        // PATIENT VIEW-ONLY SCREENS (22=Appointments, 23=Records, 24=Bills) 
        else if ((appState == 22 || appState == 23 || appState == 24) && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::BackSpace) { appState = 11; menuSelection = 0; }
        }

        // PATIENT PAY BILL 
        else if (appState == 25 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) { appState = 11; menuSelection = 0; }
        }
        else if (appState == 25 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') { if (inputLen1 > 0) inputBuf1[--inputLen1] = '\0'; }
            else if (c == '\r' || c == '\n') {
                if (inputLen1 == 0) return;
                Bill* b = billDB.findByID(std::atoi(inputBuf1));
                if (!b || b->getPatientID() != activePatient->getID()) { copyMsg("Bill not found or not yours."); actionSuccess = false; }
                else if (cmpStr(b->getStatus(), "Paid") == 0) { copyMsg("Bill already paid."); actionSuccess = false; }
                else {
                    try {
                        if (activePatient->getBalance() < b->getAmount()) throw InsufficientFundsException();
                        *activePatient -= b->getAmount();
                        b->setStatus("Paid");
                        fh.saveBills(billDB, "bills.txt");
                        fh.savePatients(patientDB, "patients.txt");
                        copyMsg("Bill paid successfully!");
                        actionSuccess = true;
                    }
                    catch (InsufficientFundsException& e) { copyMsg(e.what()); actionSuccess = false; }
                }
                inputLen1 = 0; 
                *(inputBuf1 + 0) = '\0';
            }
            else if (c >= '0' && c <= '9' && inputLen1 < 19)
            { inputBuf1[inputLen1++] = c;
            inputBuf1[inputLen1] = '\0'; 
            }
        }

        // PATIENT TOP UP BALANCE
        else if (appState == 26 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape)
            { appState = 11; menuSelection = 0; }
        }
        else if (appState == 26 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') { if (inputLen1 > 0) inputBuf1[--inputLen1] = '\0'; }
            else if (c == '\r' || c == '\n') {
                if (inputLen1 == 0) return;
                double amt = std::atof(inputBuf1);
                if (amt <= 0.0)
                { copyMsg("Amount must be greater than zero.");
                actionSuccess = false; }
                else { *activePatient += amt; fh.savePatients(patientDB, "patients.txt"); copyMsg("Balance topped up!"); actionSuccess = true; }
                inputLen1 = 0; *(inputBuf1 + 0) = '\0';
            }
            else if ((c >= '0' && c <= '9' || c == '.') && inputLen1 < 19) { inputBuf1[inputLen1++] = c; inputBuf1[inputLen1] = '\0'; }
        }

        
        // DOCTOR PORTAL
        

        // DOCTOR LOGIN 
        else if (appState == 30 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) 
            { appState = 0; resetLoginSystem();
            loginFailed = false; }
        }
        else if (appState == 30 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') { if (loginStep == 0 && idLen > 0) loginID[--idLen] = '\0'; else if (loginStep == 1 && passLen > 0) loginPass[--passLen] = '\0'; }
            else if (c == '\r' || c == '\n') {
                if (doctorAccountLocked) return;
                if (loginStep == 0 && idLen > 0) loginStep = 1;
                else if (loginStep == 1 && passLen > 0) {
                    Doctor* d = doctorDB.findByID(std::atoi(loginID));
                    bool match = false;
                    if (d != nullptr) {
                        const char* rp = d->getPassword(); int i = 0; match = true;
                        while (*(loginPass + i) != '\0') { if (*(rp + i) != *(loginPass + i)) { match = false; break; } i++; }
                        if (match && *(rp + i) != '\0' && *(rp + i) != '\r' && *(rp + i) != '\n') match = false;
                    }
                    if (d != nullptr && match) {
                        activeDoctor = d; appState = 31; menuSelection = 0; doctorFailedAttempts = 0;
                        resetInputBuffers(); resetLongBuffers();
                    }
                    else {
                        doctorFailedAttempts++;
                        if (doctorFailedAttempts >= 3) {
                            doctorAccountLocked = true;
                            std::ofstream log("security_log.txt", std::ios::app);
                            log << "Doctor," << loginID << ",LOCKED\n"; log.close();
                        }
                        else { loginFailed = true; loginStep = 0; idLen = 0; passLen = 0; *(loginID + 0) = '\0'; *(loginPass + 0) = '\0'; }
                    }
                }
            }
            else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                if (loginStep == 0 && idLen < 15 && c >= '0' && c <= '9') 
                { loginID[idLen++] = c; loginID[idLen] = '\0'; }
                else if (loginStep == 1 && passLen < 15)
                { loginPass[passLen++] = c; loginPass[passLen] = '\0'; }
            }
        }

        // DOCTOR DASHBOARD NAVIGATION 
        else if (appState == 31 && event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Up) { menuSelection--; if (menuSelection < 0) menuSelection = 5; }
            if (event.key.code == sf::Keyboard::Down) { menuSelection++; if (menuSelection > 5) menuSelection = 0; }
            if (event.key.code == sf::Keyboard::Enter) {
                resetInputBuffers(); resetLongBuffers();
                if (menuSelection == 0) appState = 32;
                else if (menuSelection == 1) appState = 33;
                else if (menuSelection == 2) appState = 34;
                else if (menuSelection == 3) appState = 35;
                else if (menuSelection == 4) appState = 36;
                else if (menuSelection == 5) { activeDoctor = nullptr; resetLoginSystem(); loginFailed = false; appState = 0; menuSelection = 0; }
            }
        }

        // DOCTOR VIEW-ONLY SCREENS (32=Appointments, 33=Patients) 
        else if ((appState == 32 || appState == 33) && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) { appState = 31; menuSelection = 0; }
        }

        // DOCTOR WRITE PRESCRIPTION
        else if (appState == 34 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) 
            { appState = 31; menuSelection = 0; }
        }
        else if (appState == 34 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') {
                if (bookingStep == 0 && inputLen1 > 0) inputBuf1[--inputLen1] = '\0';
                else if (bookingStep == 1 && longLen1 > 0)  longBuf1[--longLen1] = '\0';
                else if (bookingStep == 2 && longLen2 > 0)  longBuf2[--longLen2] = '\0';
            }
            else if (c == '\r' || c == '\n') {
                if (bookingStep == 0) {
                    if (inputLen1 == 0) return;
                    Appointment* a = apptDB.findByID(std::atoi(inputBuf1));
                    if (!a || a->getDoctorID() != activeDoctor->getID()) { copyMsg("Appointment not found or not assigned to you."); actionSuccess = false; inputLen1 = 0; *(inputBuf1 + 0) = '\0'; }
                    else { *(statusMsg + 0) = '\0'; bookingStep = 1; }
                }
                else if (bookingStep == 1) { if (longLen1 == 0) return; *(statusMsg + 0) = '\0'; bookingStep = 2; }
                else if (bookingStep == 2) {
                    Appointment* a = apptDB.findByID(std::atoi(inputBuf1));
                    Prescription* rx = new Prescription();
                    int rid = fh.maxPrescriptionID("prescription.txt") + 1;
                    rx->setID(rid);
                    rx->setAppointmentID(std::atoi(inputBuf1));
                    rx->setPatientID(a->getPatientID()); 
                    rx->setDoctorID(activeDoctor->getID());
                    rx->setDate(a->getDate());
                    rx->setMedicines(longBuf1);
                    rx->setNotes(longBuf2);
                    rxDB.add(rx);
                    fh.appendPrescription(rx, "prescription.txt");
                    a->setStatus("completed");
                    fh.saveAppointments(apptDB, "appointments.txt");
                    copyMsg("Prescription written successfully!");
                    actionSuccess = true;
                        bookingStep = 3;
                }
            }
            else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                if (bookingStep == 0 && inputLen1 < 19 && c >= '0' && c <= '9') { inputBuf1[inputLen1++] = c; inputBuf1[inputLen1] = '\0'; }
                else if (bookingStep == 1 && longLen1 < 499) { longBuf1[longLen1++] = c; longBuf1[longLen1] = '\0'; }
                else if (bookingStep == 2 && longLen2 < 299) { longBuf2[longLen2++] = c; longBuf2[longLen2] = '\0'; }
            }
        }

        // DOCTOR MARK APPOINTMENT COMPLETE
        else if (appState == 35 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) { appState = 31; menuSelection = 0; }
        }
        else if (appState == 35 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') { if (inputLen1 > 0) inputBuf1[--inputLen1] = '\0'; }
            else if (c == '\r' || c == '\n') {
                if (inputLen1 == 0) return;
                Appointment* a = apptDB.findByID(std::atoi(inputBuf1));
                if (!a || a->getDoctorID() != activeDoctor->getID()) { copyMsg("Appointment not found or not yours."); actionSuccess = false; }
                else if (cmpStr(a->getStatus(), "completed") == 0) { copyMsg("Already marked completed."); actionSuccess = false; }
                else if (cmpStr(a->getStatus(), "cancelled") == 0) { copyMsg("Cannot complete a cancelled appointment."); actionSuccess = false; }
                else { a->setStatus("completed"); fh.saveAppointments(apptDB, "appointments.txt"); copyMsg("Appointment marked as completed."); actionSuccess = true; }
                inputLen1 = 0; *(inputBuf1 + 0) = '\0';
            }
            else if (c >= '0' && c <= '9' && inputLen1 < 19) { inputBuf1[inputLen1++] = c; inputBuf1[inputLen1] = '\0'; }
        }

        // DOCTOR MARK APPOINTMENT NO-SHOW 
        else if (appState == 36 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) { appState = 31; menuSelection = 0; }
        }
        else if (appState == 36 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') { if (inputLen1 > 0) inputBuf1[--inputLen1] = '\0'; }
            else if (c == '\r' || c == '\n') {
                if (inputLen1 == 0) return;
                Appointment* a = apptDB.findByID(std::atoi(inputBuf1));
                if (!a || a->getDoctorID() != activeDoctor->getID()) 
                { copyMsg("Appointment not found or not yours."); actionSuccess = false; }
                else if (cmpStr(a->getStatus(), "no-show") == 0) 
                { copyMsg("Already marked as no-show."); actionSuccess = false; }
                else if (cmpStr(a->getStatus(), "cancelled") == 0)
                { copyMsg("Cannot mark a cancelled appointment as no-show."); actionSuccess = false; }
                else if (cmpStr(a->getStatus(), "completed") == 0)
                { copyMsg("Cannot mark a completed appointment as no-show."); actionSuccess = false; }
                else { a->setStatus("no-show");
                fh.saveAppointments(apptDB, "appointments.txt");
                copyMsg("Appointment marked as no-show."); actionSuccess = true; }
                inputLen1 = 0; *(inputBuf1 + 0) = '\0';
            }
            else if (c >= '0' && c <= '9' && inputLen1 < 19) { inputBuf1[inputLen1++] = c; inputBuf1[inputLen1] = '\0'; }
        }

        
        //  ADMIN PORTAL 
       

        // ADMIN LOGIN 
        else if (appState == 40 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape)
            { appState = 0; resetLoginSystem();
            loginFailed = false;
            }
        }
        else if (appState == 40 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') { if (loginStep == 0 && idLen > 0) loginID[--idLen] = '\0'; else if (loginStep == 1 && passLen > 0) loginPass[--passLen] = '\0'; }
            else if (c == '\r' || c == '\n') {
                if (adminAccountLocked) return;
                if (loginStep == 0 && idLen > 0) loginStep = 1;
                else if (loginStep == 1 && passLen > 0) {
                    bool match = false;
                    if (admin != nullptr && admin->getID() == std::atoi(loginID)) {
                        const char* rp = admin->getPassword(); int i = 0; match = true;
                        while (*(loginPass + i) != '\0')
                        { if (*(rp + i) != *(loginPass + i)) 
                        { match = false; break; }
                        i++;
                        }
                        if (match && *(rp + i) != '\0' && *(rp + i) != '\r' && *(rp + i) != '\n') match = false;
                    }
                    if (match)
                    { appState = 41;
                    menuSelection = 0;
                    adminFailedAttempts = 0; 
                    
                    resetInputBuffers();
                    resetLongBuffers();
                    }
                    else {
                        adminFailedAttempts++;
                        if (adminFailedAttempts >= 3) {
                            adminAccountLocked = true;
                            std::ofstream log("security_log.txt", std::ios::app);
                            log << "Admin," << loginID << ",LOCKED\n"; log.close();
                        }
                        else 
                        { 
                        loginFailed = true;
                        loginStep = 0; 
                        idLen = 0;
                        passLen = 0;
                        *(loginID + 0) = '\0';
                        *(loginPass + 0) = '\0';
                        }
                    }
                }
            }
            else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                if (loginStep == 0 && idLen < 15 && c >= '0' && c <= '9') { loginID[idLen++] = c; loginID[idLen] = '\0'; }
                else if (loginStep == 1 && passLen < 15) { loginPass[passLen++] = c; loginPass[passLen] = '\0'; }
            }
        }

        // ADMIN DASHBOARD NAVIGATION
        else if (appState == 41 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) { menuSelection--; if (menuSelection < 0) menuSelection = 9; }
            if (event.key.code == sf::Keyboard::Down) { menuSelection++; if (menuSelection > 9) menuSelection = 0; }
            if (event.key.code == sf::Keyboard::Enter) {
                resetInputBuffers(); resetLongBuffers();
                if (menuSelection == 0) appState = 42;   // Add Doctor
                else if (menuSelection == 1) appState = 43;   // Remove Doctor
                else if (menuSelection == 2) appState = 44;   // View All Patients
                else if (menuSelection == 3) appState = 45;   // View All Doctors
                else if (menuSelection == 4) appState = 46;   // View All Appointments
                else if (menuSelection == 5) appState = 47;   // View Unpaid Bills
                else if (menuSelection == 6) appState = 48;   // Discharge Patient
                else if (menuSelection == 7) appState = 49;   // Security Log
                else if (menuSelection == 8) appState = 50;   // Generate Daily Report
                else if (menuSelection == 9) { resetLoginSystem(); loginFailed = false; appState = 0; menuSelection = 0; }
            }
        }

        // ADMIN VIEW-ONLY SCREENS (44,45,46,47,49,50) 
        else if ((appState == 44 || appState == 45 || appState == 46 || appState == 47 || appState == 49 || appState == 50) && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) { appState = 41; menuSelection = 0; }
        }

        // ADMIN ADD DOCTOR
        else if (appState == 42 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) { appState = 41; menuSelection = 0; }
        }
        else if (appState == 42 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') {
                if (bookingStep == 0 && longLen1 > 0)  longBuf1[--longLen1] = '\0';
                else if (bookingStep == 1 && inputLen2 > 0) inputBuf2[--inputLen2] = '\0';
                else if (bookingStep == 2 && inputLen3 > 0) inputBuf3[--inputLen3] = '\0';
                else if (bookingStep == 3 && longLen2 > 0)  longBuf2[--longLen2] = '\0';
                else if (bookingStep == 4 && inputLen1 > 0) inputBuf1[--inputLen1] = '\0';
            }
            else if (c == '\r' || c == '\n') {
                if (bookingStep == 0) { if (longLen1 == 0) return; *(statusMsg + 0) = '\0'; bookingStep = 1; }
                else if (bookingStep == 1) { if (inputLen2 == 0) return; *(statusMsg + 0) = '\0'; bookingStep = 2; }
                else if (bookingStep == 2) {
                    if (inputLen3 == 0) return;
                    if (!Validator::isValidContact(inputBuf3)) { copyMsg("Invalid contact. Must be 11 digits."); actionSuccess = false; inputLen3 = 0; *(inputBuf3 + 0) = '\0'; }
                    else { *(statusMsg + 0) = '\0'; bookingStep = 3; }
                }
                else if (bookingStep == 3) {
                    if (longLen2 == 0) return;
                    if (!Validator::isValidPassword(longBuf2)) { copyMsg("Password must be at least 6 characters."); actionSuccess = false; longLen2 = 0; *(longBuf2 + 0) = '\0'; }
                    else { *(statusMsg + 0) = '\0'; bookingStep = 4; }
                }
                else if (bookingStep == 4) {
                    double fee = std::atof(inputBuf1);
                    if (fee <= 0) { copyMsg("Fee must be greater than zero."); actionSuccess = false; inputLen1 = 0; *(inputBuf1 + 0) = '\0'; }
                    else {
                        Doctor* nd = new Doctor();
                        int nid = fh.maxDoctorID("doctors.txt") + 1;
                        nd->setID(nid); nd->setName(longBuf1); nd->setSpecialization(inputBuf2);
                        nd->setContact(inputBuf3); nd->setPassword(longBuf2); nd->setFee(fee);
                        doctorDB.add(nd); fh.appendDoctor(nd, "doctors.txt");
                        copyMsg("Doctor added successfully!"); actionSuccess = true; bookingStep = 5;
                    }
                }
            }
            else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                if (bookingStep == 0 && longLen1 < 49)
                { longBuf1[longLen1++] = c;  longBuf1[longLen1] = '\0'; }
                else if (bookingStep == 1 && inputLen2 < 49)
                { inputBuf2[inputLen2++] = c; inputBuf2[inputLen2] = '\0'; }
                else if (bookingStep == 2 && inputLen3 < 12 && c >= '0' && c <= '9') 
                { inputBuf3[inputLen3++] = c; inputBuf3[inputLen3] = '\0'; }
                else if (bookingStep == 3 && longLen2 < 49) 
                { longBuf2[longLen2++] = c;  longBuf2[longLen2] = '\0'; }
                else if (bookingStep == 4 && inputLen1 < 12 && (c >= '0' && c <= '9' || c == '.')) 
                { inputBuf1[inputLen1++] = c; inputBuf1[inputLen1] = '\0'; }
            }
        }

        // ADMIN REMOVE DOCTOR
        else if (appState == 43 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) { appState = 41; menuSelection = 0; }
        }
        else if (appState == 43 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') { if (inputLen1 > 0) inputBuf1[--inputLen1] = '\0'; }
            else if (c == '\r' || c == '\n') {
                if (inputLen1 == 0) return;
                int did = std::atoi(inputBuf1);
                Doctor* d = doctorDB.findByID(did);
                if (!d) { copyMsg("Doctor not found. Enter a valid Doctor ID."); actionSuccess = false; }
                else {
                    doctorDB.removeByID(did);
                    fh.saveDoctors(doctorDB, "doctors.txt");
                    copyMsg("Doctor removed successfully."); actionSuccess = true;
                }
                inputLen1 = 0; *(inputBuf1 + 0) = '\0';
            }
            else if (c >= '0' && c <= '9' && inputLen1 < 19) { inputBuf1[inputLen1++] = c; inputBuf1[inputLen1] = '\0'; }
        }

        // ADMIN DISCHARGE PATIENT 
        else if (appState == 48 && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) { appState = 41; menuSelection = 0; }
        }
        else if (appState == 48 && event.type == sf::Event::TextEntered) {
            char c = static_cast<char>(event.text.unicode);
            if (c == '\b') { if (inputLen1 > 0) inputBuf1[--inputLen1] = '\0'; }
            else if (c == '\r' || c == '\n') {
                if (inputLen1 == 0) return;
                int pid = std::atoi(inputBuf1);
                if (!patientDB.findByID(pid)) { copyMsg("Patient not found."); actionSuccess = false; }
                else {
                    fh.dischargePatient(pid, patientDB, apptDB, billDB, rxDB, "patients.txt", "appointments.txt", "bills.txt", "prescription.txt", "discharged.txt");
                    copyMsg("Patient discharged and archived."); actionSuccess = true;
                }
                inputLen1 = 0; *(inputBuf1 + 0) = '\0';
            }
            else if (c >= '0' && c <= '9' && inputLen1 < 19) { inputBuf1[inputLen1++] = c; inputBuf1[inputLen1] = '\0'; }
        }
    }
}


//  RENDER 


void HospitalSystem::render() {
    window.clear();
    drawGradientBackground();

    
    //  MAIN MENU
    
    if (appState == 0) {
        drawHeader();
        drawCard(390, 148, 500, 340, sf::Color(12, 22, 55));
        sf::Text mt;
        mt.setFont(font);
        mt.setCharacterSize(19);
        mt.setFillColor(sf::Color(0, 200, 240));
        mt.setString("SELECT PORTAL");
        sf::FloatRect mr = mt.getLocalBounds();
        mt.setOrigin(mr.left + mr.width / 2.f, 0);
        mt.setPosition(640.f, 160.f);
        window.draw(mt);
        sf::RectangleShape sep(sf::Vector2f(460.f, 2.f));
        sep.setPosition(410, 190);
        sep.setFillColor(sf::Color(0, 200, 240, 100));
        window.draw(sep);
        const char* opts[] = { "Patient Portal","Doctor Portal","Admin Portal","Exit System" };
        for (int i = 0; i < 4; i++) 
            drawElegantButton(410, 202 + i * 68, 460, 52, opts[i], menuSelection == i);
    }

    
    //  LOGIN SCREENS 
    
    else if (appState == 1)  renderLoginScreen("PATIENT LOGIN");
    else if (appState == 30) renderLoginScreen("DOCTOR LOGIN");
    else if (appState == 40) renderLoginScreen("ADMIN LOGIN");

    
    // PATIENT PORTAL RENDER
    

    // PATIENT DASHBOARD 
    else if (appState == 11) {
        drawHeader();
        drawCard(30, 110, 820, 52, sf::Color(10, 28, 68));
        char wb[150];
        snprintf(wb, 150, "Welcome, %s", activePatient->getName());
        sf::Text wt; wt.setFont(font);
        wt.setCharacterSize(20);
        wt.setFillColor(sf::Color(0, 220, 255));
        wt.setString(wb); 
        wt.setPosition(45, 122);
        window.draw(wt);
        drawCard(858, 110, 392, 52, sf::Color(0, 72, 46));
        char bb[80];
        snprintf(bb, 80, "Balance: PKR %.2f", activePatient->getBalance());
        sf::Text bt; 
        bt.setFont(font);
        bt.setCharacterSize(18);
        bt.setFillColor(sf::Color(0, 255, 175));
        bt.setString(bb);
        bt.setPosition(870, 123);
        window.draw(bt);
        const char* po[] = { "1. Book Appointment","2. Cancel Appointment","3. View Appointments","4. Medical Records","5. View Bills","6. Pay Bill","7. Top Up Balance","8. Logout" };
        for (int i = 0; i < 8; i++) drawElegantButton(440, 175 + i * 62, 400, 50, po[i], menuSelection == i);
    }

    // PATIENT BOOK APPOINTMENT
    else if (appState == 20) {
        drawHeader();
        drawCard(330, 110, 620, 515, sf::Color(12, 22, 55));
        sf::Text pt; pt.setFont(font);
        pt.setCharacterSize(22);
        pt.setFillColor(sf::Color(0, 200, 240)); 
        pt.setString("BOOK APPOINTMENT");
        sf::FloatRect pr = pt.getLocalBounds();
        pt.setOrigin(pr.left + pr.width / 2.f, 0);
        pt.setPosition(640.f, 120.f); 
        window.draw(pt);
        sf::RectangleShape sep(sf::Vector2f(580.f, 2.f));
        sep.setPosition(350, 156);
        sep.setFillColor(sf::Color(0, 200, 240, 100));
        window.draw(sep);
        drawInputField(360, 172, 560, "Doctor ID", inputBuf1, bookingStep == 0);
        drawInputField(360, 268, 560, "Date (DD-MM-YYYY)", inputBuf2, bookingStep == 1);
        drawInputField(360, 364, 560, "Time Slot (e.g. 09:00AM)", inputBuf3, bookingStep == 2);
        if (bookingStep == 3) 
        { drawCard(330, 478, 620, 48, sf::Color(0, 88, 52)); sf::Text d; d.setFont(font); d.setCharacterSize(16); d.setFillColor(sf::Color::White); d.setString("Booked! Press ESC to return."); d.setPosition(345, 490); window.draw(d); }
        drawStatusMessage(); drawBackHint();
    }

    // PATIENT CANCEL APPOINTMENT 
    else if (appState == 21) {
        drawHeader();
        drawCard(330, 110, 620, 340, sf::Color(12, 22, 55));
        sf::Text pt;
        pt.setFont(font);
        pt.setCharacterSize(22);
        pt.setFillColor(sf::Color(0, 200, 240));
        pt.setString("CANCEL APPOINTMENT");
        sf::FloatRect pr = pt.getLocalBounds();
        pt.setOrigin(pr.left + pr.width / 2.f, 0);
        pt.setPosition(640.f, 122.f);
        window.draw(pt);
        sf::RectangleShape sep(sf::Vector2f(580.f, 2.f));
        sep.setPosition(350, 156);
        sep.setFillColor(sf::Color(0, 200, 240, 100));
        window.draw(sep);
        drawInputField(360, 175, 560, "Enter Appointment ID to Cancel", inputBuf1, true);
        drawStatusMessage();
        drawBackHint();
    }

    // PATIENT VIEW APPOINTMENTS 
    else if (appState == 22) {
        drawPageHeader("MY APPOINTMENTS");
        int yp = 158; bool found = false;
        for (int i = 0; i < apptDB.size(); i++) {
            Appointment* a = *(apptDB.getAll() + i);
            if (!a || a->getPatientID() != activePatient->getID()) continue;
            bool can = cmpStr(a->getStatus(), "cancelled") == 0;
            drawCard(50, yp, 1180, 46, can ? sf::Color(38, 10, 10) : sf::Color(14, 28, 62));
            char buf[300];
            snprintf(buf, 300, "ID:%-4d | Dr ID:%-4d | Date:%-12s | Slot:%-10s | %s", a->getID(), a->getDoctorID(), a->getDate(), a->getTimeSlot(), a->getStatus());
            sf::Text t;
            t.setFont(font);
            t.setString(buf);
            t.setCharacterSize(16);
            t.setFillColor(can ? sf::Color(255, 80, 80) : sf::Color(0, 215, 145));
            t.setPosition(65, yp + 12);
            window.draw(t);
            yp += 54; found = true; if (yp > 625) break;
        }
        if (!found) { sf::Text n; n.setFont(font); n.setCharacterSize(22); n.setFillColor(sf::Color(90, 130, 175)); n.setString("No appointments found."); n.setPosition(50, 200); window.draw(n); }
        drawBackHint();
    }

    // PATIENT MEDICAL RECORDS 
    else if (appState == 23) {
        drawPageHeader("MY MEDICAL RECORDS");
        int yp = 158; bool found = false;
        for (int i = 0; i < rxDB.size(); i++) {
            Prescription* rx = *(rxDB.getAll() + i);
            if (!rx || rx->getPatientID() != activePatient->getID()) continue;
            drawCard(50, yp, 1180, 76, sf::Color(14, 28, 62));
            char b1[250], b2[200];
            snprintf(b1, 250, "Rx#%-4d | Appt#%-4d | Dr#%-4d | Date: %s", rx->getID(), rx->getAppointmentID(), rx->getDoctorID(), rx->getDate());
            snprintf(b2, 200, "Medicines: %s", rx->getMedicines());

            sf::Text t1; t1.setFont(font);
            t1.setString(b1); t1.setCharacterSize(15); t1.setFillColor(sf::Color(0, 195, 255)); t1.setPosition(65, yp + 7); window.draw(t1);
            sf::Text t2; t2.setFont(font); t2.setString(b2); t2.setCharacterSize(14); t2.setFillColor(sf::Color(185, 210, 235)); t2.setPosition(65, yp + 32); window.draw(t2);
            yp += 84; found = true; if (yp > 598) break;
        }
        if (!found) { sf::Text n; n.setFont(font); n.setCharacterSize(22); n.setFillColor(sf::Color(90, 130, 175)); n.setString("No medical records found."); n.setPosition(50, 200); window.draw(n); }
        drawBackHint();
    }

    // PATIENT VIEW BILLS 
    else if (appState == 24) {
        drawPageHeader("MY BILLS");
        int yp = 158; bool found = false;
        for (int i = 0; i < billDB.size(); i++) {
            Bill* b = *(billDB.getAll() + i);
            if (!b || b->getPatientID() != activePatient->getID()) continue;
            bool paid = cmpStr(b->getStatus(), "Paid") == 0;
            drawCard(50, yp, 1180, 48, paid ? sf::Color(10, 36, 20) : sf::Color(36, 20, 10));
            char buf[300]; snprintf(buf, 300, "Bill#%-4d | Appt#%-4d | PKR %-8.2f | Date:%-12s | %s", b->getID(), b->getAppointmentID(), b->getAmount(), b->getDate(), b->getStatus());
           
            sf::Text t; t.setFont(font); t.setString(buf); t.setCharacterSize(16);
            t.setFillColor(paid ? sf::Color(0, 215, 145) : sf::Color(255, 178, 0)); t.setPosition(65, yp + 13); window.draw(t);
            yp += 56; found = true; if (yp > 625) break;
        }
        if (!found) { sf::Text n; n.setFont(font); n.setCharacterSize(22); n.setFillColor(sf::Color(90, 130, 175)); n.setString("No bills found."); n.setPosition(50, 200); window.draw(n); }
        drawBackHint();
    }

    // PATIENT PAY BILL 
    else if (appState == 25) {
        drawHeader(); 
        drawCard(330, 110, 620, 360, sf::Color(12, 22, 55));
        sf::Text pt; 
        pt.setFont(font);
        pt.setCharacterSize(22);
        pt.setFillColor(sf::Color(0, 200, 240));
        pt.setString("PAY BILL");
        sf::FloatRect pr = pt.getLocalBounds();
        pt.setOrigin(pr.left + pr.width / 2.f, 0);
        pt.setPosition(640.f, 122.f); 
        window.draw(pt);
        sf::RectangleShape sep(sf::Vector2f(580.f, 2.f));
        sep.setPosition(350, 156);
        sep.setFillColor(sf::Color(0, 200, 240, 100));
        window.draw(sep);
        char bb[80];
        snprintf(bb, 80, "Balance: PKR %.2f", activePatient->getBalance());
        sf::Text bt; bt.setFont(font);
        bt.setCharacterSize(17);
        bt.setFillColor(sf::Color(0, 215, 145));
        bt.setString(bb);
        bt.setPosition(360, 165);
        window.draw(bt);
        drawInputField(360, 205, 560, "Bill ID to Pay", inputBuf1, true);
        drawStatusMessage();
        drawBackHint();
    }

    // PATIENT TOP UP BALANCE 
    else if (appState == 26) {
        drawHeader(); 
        drawCard(330, 110, 620, 360, sf::Color(12, 22, 55));
        sf::Text pt; 
        pt.setFont(font);
        pt.setCharacterSize(22);
        pt.setFillColor(sf::Color(0, 200, 240)); 
        pt.setString("TOP UP BALANCE");
        sf::FloatRect pr = pt.getLocalBounds();
        pt.setOrigin(pr.left + pr.width / 2.f, 0);
        pt.setPosition(640.f, 122.f);
        window.draw(pt);
        sf::RectangleShape sep(sf::Vector2f(580.f, 2.f));
        sep.setPosition(350, 156);
        sep.setFillColor(sf::Color(0, 200, 240, 100));
        window.draw(sep);
        char bb[80];
        snprintf(bb, 80, "Balance: PKR %.2f", activePatient->getBalance());
        sf::Text bt; bt.setFont(font); bt.setCharacterSize(17); bt.setFillColor(sf::Color(0, 215, 145)); bt.setString(bb); bt.setPosition(360, 165); window.draw(bt);
        drawInputField(360, 205, 560, "Amount to Add (PKR)", inputBuf1, true);
        drawStatusMessage(); drawBackHint();
    }

    
    //  DOCTOR PORTAL RENDER
    

    // DOCTOR DASHBOARD 
    else if (appState == 31) {
        drawHeader();
        drawCard(30, 110, 820, 52, sf::Color(10, 28, 68));
        char wb[150]; snprintf(wb, 150, "Dr. %s  |  %s", activeDoctor->getName(), activeDoctor->getSpecialization());
        sf::Text wt; wt.setFont(font); wt.setCharacterSize(19); wt.setFillColor(sf::Color(0, 220, 255)); wt.setString(wb); wt.setPosition(45, 122); window.draw(wt);
        drawCard(858, 110, 392, 52, sf::Color(0, 72, 46));
        char fb[80]; snprintf(fb, 80, "Fee: PKR %.2f", activeDoctor->getFee());
        sf::Text ft; ft.setFont(font); ft.setCharacterSize(18); ft.setFillColor(sf::Color(0, 255, 175)); ft.setString(fb); ft.setPosition(870, 123); window.draw(ft);
        const char* dOpts[] = { "1. View My Appointments","2. View My Patients","3. Write Prescription","4. Mark Appointment Complete","5. Mark Appointment No-Show","6. Logout" };
        for (int i = 0; i < 6; i++) drawElegantButton(390, 185 + i * 72, 500, 56, dOpts[i], menuSelection == i);
    }

    // DOCTOR VIEW APPOINTMENTS
    else if (appState == 32) {
        drawPageHeader("MY APPOINTMENTS");
        int yp = 158; bool found = false;
        for (int i = 0; i < apptDB.size(); i++) {
            Appointment* a = *(apptDB.getAll() + i);
            if (!a || a->getDoctorID() != activeDoctor->getID()) continue;
            bool comp = cmpStr(a->getStatus(), "completed") == 0;
            bool can = cmpStr(a->getStatus(), "cancelled") == 0;
            bool nos = cmpStr(a->getStatus(), "no-show") == 0;
            sf::Color rowC = can ? sf::Color(38, 10, 10) : (nos ? sf::Color(38, 25, 0) : (comp ? sf::Color(10, 36, 20) : sf::Color(14, 28, 62)));
            drawCard(50, yp, 1180, 46, rowC);
            char buf[300]; snprintf(buf, 300, "ID:%-4d | Patient#%-4d | Date:%-12s | Slot:%-10s | %s", a->getID(), a->getPatientID(), a->getDate(), a->getTimeSlot(), a->getStatus());
            sf::Color tc = can ? sf::Color(255, 80, 80) : (nos ? sf::Color(255, 178, 0) : (comp ? sf::Color(0, 215, 145) : sf::Color(0, 195, 255)));
            sf::Text t; t.setFont(font); t.setString(buf); t.setCharacterSize(16); t.setFillColor(tc); t.setPosition(65, yp + 12); window.draw(t);
            yp += 54; found = true; if (yp > 625) break;
        }
        if (!found) { sf::Text n; n.setFont(font); n.setCharacterSize(22); n.setFillColor(sf::Color(90, 130, 175)); n.setString("No appointments found."); n.setPosition(50, 200); window.draw(n); }
        drawBackHint();
    }

    // DOCTOR VIEW PATIENTS 
    else if (appState == 33) {
        drawPageHeader("MY PATIENTS");
        int seen[100]; int sc = 0; int yp = 158; bool found = false;
        for (int i = 0; i < apptDB.size(); i++) {
            Appointment* a = *(apptDB.getAll() + i);
            if (!a || a->getDoctorID() != activeDoctor->getID()) continue;
            int pid = a->getPatientID();
            bool already = false;
            for (int j = 0; j < sc; j++)
                if (*(seen + j) == pid) { already = true; break; }
            if (already) continue;
            *(seen + sc++) = pid;
            Patient* p = patientDB.findByID(pid); if (!p) continue;
            drawCard(50, yp, 1180, 46, sf::Color(14, 28, 62));
            char buf[300]; snprintf(buf, 300, "ID:%-4d | %-20s | Age:%-3d | Gender: %c | Contact: %s", p->getID(), p->getName(), p->getAge(), p->getGender(), p->getContact());
            sf::Text t; t.setFont(font); t.setString(buf); t.setCharacterSize(16); t.setFillColor(sf::Color(0, 215, 145)); t.setPosition(65, yp + 12); window.draw(t);
            yp += 54; found = true; if (yp > 625) break;
        }
        if (!found) { sf::Text n; n.setFont(font); n.setCharacterSize(22); n.setFillColor(sf::Color(90, 130, 175)); n.setString("No patients found."); n.setPosition(50, 200); window.draw(n); }
        drawBackHint();
    }

    // DOCTOR WRITE PRESCRIPTION
    else if (appState == 34) {
        drawHeader(); 
        drawCard(280, 110, 720, 520, sf::Color(12, 22, 55));
        sf::Text pt;
        pt.setFont(font);
        pt.setCharacterSize(22); 
        pt.setFillColor(sf::Color(0, 200, 240));
        pt.setString("WRITE PRESCRIPTION");

        sf::FloatRect pr = pt.getLocalBounds(); pt.setOrigin(pr.left + pr.width / 2.f, 0); pt.setPosition(640.f, 122.f); window.draw(pt);
        sf::RectangleShape sep(sf::Vector2f(680.f, 2.f)); sep.setPosition(300, 156); sep.setFillColor(sf::Color(0, 200, 240, 100)); window.draw(sep);
        
        drawInputField(300, 170, 680, "Appointment ID", inputBuf1, bookingStep == 0);
        drawLongInputField(300, 258, 680, 60, "Medicines (press ENTER when done)", longBuf1, bookingStep == 1);
        drawLongInputField(300, 368, 680, 60, "Notes / Instructions", longBuf2, bookingStep == 2);
        
        if (bookingStep == 3)
        { drawCard(280, 468, 720, 48, sf::Color(0, 88, 52)); sf::Text d; d.setFont(font); d.setCharacterSize(16); d.setFillColor(sf::Color::White); d.setString("Prescription written! Press ESC to return."); d.setPosition(295, 480); window.draw(d); }
        drawStatusMessage(); drawBackHint();
    }

    // DOCTOR MARK APPOINTMENT COMPLETE
    else if (appState == 35) {
        drawHeader();
        drawCard(330, 110, 620, 340, sf::Color(12, 22, 55));
        
        sf::Text pt; pt.setFont(font); pt.setCharacterSize(22); pt.setFillColor(sf::Color(0, 200, 240)); pt.setString("MARK AS COMPLETED");
        
        sf::FloatRect pr = pt.getLocalBounds(); pt.setOrigin(pr.left + pr.width / 2.f, 0); pt.setPosition(640.f, 122.f); window.draw(pt);
        
        sf::RectangleShape sep(sf::Vector2f(580.f, 2.f)); sep.setPosition(350, 156); sep.setFillColor(sf::Color(0, 200, 240, 100)); window.draw(sep);
        
        drawInputField(360, 175, 560, "Appointment ID", inputBuf1, true);
        drawStatusMessage(); drawBackHint();
    }

    // DOCTOR MARK APPOINTMENT NO-SHOW
    else if (appState == 36) {
        drawHeader();
        drawCard(330, 110, 620, 340, sf::Color(12, 22, 55));
        
        
        sf::Text pt; pt.setFont(font); pt.setCharacterSize(22); pt.setFillColor(sf::Color(255, 160, 30)); pt.setString("MARK AS NO-SHOW");
        
        sf::FloatRect pr = pt.getLocalBounds(); pt.setOrigin(pr.left + pr.width / 2.f, 0); pt.setPosition(640.f, 122.f); window.draw(pt);
        
        sf::RectangleShape sep(sf::Vector2f(580.f, 2.f)); sep.setPosition(350, 156); sep.setFillColor(sf::Color(255, 160, 30, 120)); window.draw(sep);
        
        sf::Text warn; warn.setFont(font); warn.setCharacterSize(14); warn.setFillColor(sf::Color(255, 178, 0));
        
        warn.setString("Patient did not attend. This will mark the appointment as no-show."); warn.setPosition(360, 163); window.draw(warn);
        drawInputField(360, 195, 560, "Appointment ID", inputBuf1, true);
        drawStatusMessage(); drawBackHint();
    }

    
    // ADMIN PORTAL RENDER 
    // 

    // ADMIN DASHBOARD 
    else if (appState == 41) {
        drawHeader();

        drawCard(30, 110, 820, 52, sf::Color(10, 28, 68));
        
        sf::Text wt; wt.setFont(font); wt.setCharacterSize(20); wt.setFillColor(sf::Color(0, 220, 255)); wt.setString("System Administrator"); wt.setPosition(45, 122); window.draw(wt);
        
        drawCard(858, 110, 392, 52, sf::Color(60, 30, 5));
        
        sf::Text rt; rt.setFont(font); rt.setCharacterSize(17); rt.setFillColor(sf::Color(255, 178, 0)); rt.setString("ADMIN PANEL"); rt.setPosition(910, 123); window.draw(rt);
        // Left column (options 0-4), Right column (options 5-9)
        
        const char* ao[] = { "1. Add Doctor","2. Remove Doctor","3. View All Patients","4. View All Doctors","5. View All Appointments","6. View Unpaid Bills","7. Discharge Patient","8. Security Log","9. Daily Report","10. Logout" };
        for (int i = 0; i < 5; i++)
            drawElegantButton(100, 175 + i * 98, 480, 78, ao[i], menuSelection == i);
        for (int i = 5; i < 10; i++)
            drawElegantButton(700, 175 + (i - 5) * 98, 480, 78, ao[i], menuSelection == i);
    }

    // ADMIN ADD DOCTOR 
    else if (appState == 42) {
        drawHeader(); drawCard(250, 110, 780, 500, sf::Color(12, 22, 55));
        
        sf::Text pt; pt.setFont(font); pt.setCharacterSize(22); pt.setFillColor(sf::Color(0, 200, 240)); pt.setString("ADD NEW DOCTOR");
        sf::FloatRect pr = pt.getLocalBounds(); pt.setOrigin(pr.left + pr.width / 2.f, 0); pt.setPosition(640.f, 120.f); window.draw(pt);
        sf::RectangleShape sep(sf::Vector2f(740.f, 2.f)); sep.setPosition(270, 154); sep.setFillColor(sf::Color(0, 200, 240, 100)); window.draw(sep);
        
        drawInputField(270, 162, 360, "Full Name", longBuf1, bookingStep == 0);
        drawInputField(650, 162, 360, "Specialization", inputBuf2, bookingStep == 1);
        drawInputField(270, 252, 360, "Contact (11 digits)", inputBuf3, bookingStep == 2);
        drawInputField(650, 252, 360, "Password", longBuf2, bookingStep == 3);
        drawInputField(270, 342, 360, "Consultation Fee (PKR)", inputBuf1, bookingStep == 4);
        if (bookingStep == 5) { drawCard(250, 430, 780, 48, sf::Color(0, 88, 52)); sf::Text d; d.setFont(font); d.setCharacterSize(16); d.setFillColor(sf::Color::White); d.setString("Doctor added successfully! Press ESC."); d.setPosition(265, 442); window.draw(d); }
        drawStatusMessage(); drawBackHint();
    }

    // ADMIN REMOVE DOCTOR 
    else if (appState == 43) {
        drawHeader(); drawCard(330, 110, 620, 340, sf::Color(12, 22, 55));
       
        sf::Text pt; pt.setFont(font); pt.setCharacterSize(22); pt.setFillColor(sf::Color(255, 80, 80)); pt.setString("REMOVE DOCTOR");
        sf::FloatRect pr = pt.getLocalBounds(); pt.setOrigin(pr.left + pr.width / 2.f, 0); pt.setPosition(640.f, 122.f); window.draw(pt);
        sf::RectangleShape sep(sf::Vector2f(580.f, 2.f)); sep.setPosition(350, 156); sep.setFillColor(sf::Color(255, 80, 80, 100)); window.draw(sep);
        sf::Text warn; warn.setFont(font); warn.setCharacterSize(14); warn.setFillColor(sf::Color(255, 178, 0));
        warn.setString("Warning: This will permanently remove the doctor from the system."); warn.setPosition(352, 163); window.draw(warn);
        
        drawInputField(360, 195, 560, "Doctor ID to Remove", inputBuf1, true);
        drawStatusMessage(); drawBackHint();
    }

    // ADMIN VIEW ALL PATIENTS 
    else if (appState == 44) {
        drawPageHeader("ALL PATIENTS");
        int yp = 158; bool found = false;
        for (int i = 0; i < patientDB.size(); i++) {
            Patient* p = *(patientDB.getAll() + i); if (!p) continue;
            drawCard(50, yp, 1180, 46, sf::Color(14, 28, 62));
            char buf[300]; snprintf(buf, 300, "ID:%-4d | %-20s | Age:%-3d | %c | Contact:%-12s | PKR %.2f", p->getID(), p->getName(), p->getAge(), p->getGender(), p->getContact(), p->getBalance());
            sf::Text t; t.setFont(font); t.setString(buf); t.setCharacterSize(15); t.setFillColor(sf::Color(0, 215, 145)); t.setPosition(65, yp + 12); window.draw(t);
            yp += 54; found = true; if (yp > 625) break;
        }
        if (!found) { sf::Text n; n.setFont(font); n.setCharacterSize(22); n.setFillColor(sf::Color(90, 130, 175)); n.setString("No patients registered."); n.setPosition(50, 200); window.draw(n); }
        drawBackHint();
    }

    // ADMIN VIEW ALL DOCTORS 
    else if (appState == 45) {
        drawPageHeader("ALL DOCTORS");
        int yp = 158; bool found = false;
        for (int i = 0; i < doctorDB.size(); i++) {
            Doctor* d = *(doctorDB.getAll() + i); if (!d) continue;
            drawCard(50, yp, 1180, 46, sf::Color(14, 28, 62));
            char buf[300]; snprintf(buf, 300, "ID:%-4d | %-22s | Spec:%-20s | Contact:%-12s | Fee: PKR %.2f", d->getID(), d->getName(), d->getSpecialization(), d->getContact(), d->getFee());
            sf::Text t; t.setFont(font); t.setString(buf); t.setCharacterSize(15); t.setFillColor(sf::Color(0, 195, 255)); t.setPosition(65, yp + 12); window.draw(t);
            yp += 54; found = true; if (yp > 625) break;
        }
        if (!found) { sf::Text n; n.setFont(font); n.setCharacterSize(22); n.setFillColor(sf::Color(90, 130, 175)); n.setString("No doctors registered."); n.setPosition(50, 200); window.draw(n); }
        drawBackHint();
    }

    // ADMIN VIEW ALL APPOINTMENTS 
    else if (appState == 46) {
        drawPageHeader("ALL APPOINTMENTS");
        int yp = 158; bool found = false;
        for (int i = 0; i < apptDB.size(); i++) {
            Appointment* a = *(apptDB.getAll() + i); if (!a) continue;
            bool can = cmpStr(a->getStatus(), "cancelled") == 0;
            bool comp = cmpStr(a->getStatus(), "completed") == 0;
            bool nos = cmpStr(a->getStatus(), "no-show") == 0;
            sf::Color rc = can ? sf::Color(38, 10, 10) : (nos ? sf::Color(38, 25, 0) : (comp ? sf::Color(10, 36, 20) : sf::Color(14, 28, 62)));
            drawCard(50, yp, 1180, 46, rc);
            char buf[300]; snprintf(buf, 300, "ID:%-4d | P#%-4d | Dr#%-4d | Date:%-12s | Slot:%-10s | %s", a->getID(), a->getPatientID(), a->getDoctorID(), a->getDate(), a->getTimeSlot(), a->getStatus());
            sf::Color tc = can ? sf::Color(255, 80, 80) : (nos ? sf::Color(255, 178, 0) : (comp ? sf::Color(0, 215, 145) : sf::Color(0, 195, 255)));
            sf::Text t; t.setFont(font); t.setString(buf); t.setCharacterSize(15); t.setFillColor(tc); t.setPosition(65, yp + 12); window.draw(t);
            yp += 54; found = true; if (yp > 625) break;
        }
        if (!found) { sf::Text n; n.setFont(font); n.setCharacterSize(22); n.setFillColor(sf::Color(90, 130, 175)); n.setString("No appointments found."); n.setPosition(50, 200); window.draw(n); }
        drawBackHint();
    }

    // ADMIN VIEW UNPAID BILLS 
    else if (appState == 47) {
        drawPageHeader("UNPAID BILLS");
        int yp = 158; bool found = false; double total = 0.0;
        for (int i = 0; i < billDB.size(); i++) {
            Bill* b = *(billDB.getAll() + i); if (!b) continue;
            if (cmpStr(b->getStatus(), "Paid") == 0) continue; // skip paid bills
            drawCard(50, yp, 1180, 46, sf::Color(36, 20, 10));
            char buf[300]; snprintf(buf, 300, "Bill#%-4d | P#%-4d | Appt#%-4d | PKR %-8.2f | Date:%-12s | %s", b->getID(), b->getPatientID(), b->getAppointmentID(), b->getAmount(), b->getDate(), b->getStatus());
            sf::Text t; t.setFont(font); t.setString(buf); t.setCharacterSize(15); t.setFillColor(sf::Color(255, 178, 0)); t.setPosition(65, yp + 12); window.draw(t);
            total += b->getAmount(); yp += 54; found = true; if (yp > 580) break;
        }
        if (!found) { sf::Text n; n.setFont(font); n.setCharacterSize(22); n.setFillColor(sf::Color(90, 130, 175)); n.setString("No unpaid bills. All clear!"); n.setPosition(50, 200); window.draw(n); }
        else {
            drawCard(50, 610, 1180, 44, sf::Color(60, 35, 5));
            char tot[100]; snprintf(tot, 100, "Total Outstanding Amount: PKR %.2f", total);
            sf::Text tt; tt.setFont(font); tt.setString(tot); tt.setCharacterSize(18); tt.setFillColor(sf::Color(255, 200, 80)); tt.setPosition(65, 622); window.draw(tt);
        }
        drawBackHint();
    }

    // ADMIN DISCHARGE PATIENT 
    else if (appState == 48) {
        drawHeader(); drawCard(330, 110, 620, 340, sf::Color(12, 22, 55));
        
        sf::Text pt; pt.setFont(font); pt.setCharacterSize(22); pt.setFillColor(sf::Color(255, 100, 80)); pt.setString("DISCHARGE PATIENT");
        sf::FloatRect pr = pt.getLocalBounds(); pt.setOrigin(pr.left + pr.width / 2.f, 0); pt.setPosition(640.f, 122.f); window.draw(pt);
        sf::RectangleShape sep(sf::Vector2f(580.f, 2.f)); sep.setPosition(350, 156); sep.setFillColor(sf::Color(255, 100, 80, 100)); window.draw(sep);
        sf::Text warn; warn.setFont(font); warn.setCharacterSize(14); warn.setFillColor(sf::Color(255, 178, 0));
        warn.setString("Warning: This will archive all patient data permanently."); warn.setPosition(355, 163); window.draw(warn);
        
        drawInputField(360, 195, 560, "Patient ID to Discharge", inputBuf1, true);
        drawStatusMessage(); drawBackHint();
    }

    // ADMIN SECURITY LOG 
    else if (appState == 49) {
        drawPageHeader("SECURITY LOG");
        std::ifstream logf("security_log.txt");
        int yp = 158; bool found = false;
        char line[250];
        while (logf.getline(line, 250) && yp < 635) {
            drawCard(50, yp, 1180, 38, sf::Color(38, 18, 10));
            sf::Text t; t.setFont(font); t.setString(line); t.setCharacterSize(15); t.setFillColor(sf::Color(255, 155, 80)); t.setPosition(65, yp + 9); window.draw(t);
            yp += 45; found = true;
        }
        logf.close();
        if (!found) { sf::Text n; n.setFont(font); n.setCharacterSize(22); n.setFillColor(sf::Color(90, 130, 175)); n.setString("Security log is empty."); n.setPosition(50, 200); window.draw(n); }
        drawBackHint();
    }


   // ADMIN GENERATE DAILY REPORT 
    else if (appState == 50) {
       drawPageHeader("DAILY REPORT");

       // Count statistics
       int totalPatients = patientDB.size();
       int totalDoctors = doctorDB.size();
       int totalAppts = 0, pending = 0, completed = 0, cancelled = 0, noShow = 0;
       for (int i = 0; i < apptDB.size(); i++) {
           Appointment* a = *(apptDB.getAll() + i); if (!a) continue;
           totalAppts++;
           if (cmpStr(a->getStatus(), "pending") == 0)   pending++;
           else if (cmpStr(a->getStatus(), "completed") == 0) completed++;
           else if (cmpStr(a->getStatus(), "cancelled") == 0) cancelled++;
           else if (cmpStr(a->getStatus(), "no-show") == 0)   noShow++;
       }
       int totalBills = 0, paidBills = 0, unpaidBills = 0;
       double paidAmt = 0.0, unpaidAmt = 0.0;
       for (int i = 0; i < billDB.size(); i++) {
           Bill* b = *(billDB.getAll() + i); if (!b) continue;
           totalBills++;
           if (cmpStr(b->getStatus(), "Paid") == 0) { paidBills++; paidAmt += b->getAmount(); }
           else { unpaidBills++; unpaidAmt += b->getAmount(); }
       }
       int totalRx = 0;
       for (int i = 0; i < rxDB.size(); i++) if (*(rxDB.getAll() + i)) totalRx++;

       // Summary Cards (top section) 
       drawCard(50, 158, 560, 80, sf::Color(10, 28, 68));
       drawCard(670, 158, 560, 80, sf::Color(10, 28, 68));
       drawCard(50, 248, 560, 80, sf::Color(14, 28, 40));
       drawCard(670, 248, 560, 80, sf::Color(14, 28, 40));
       drawCard(50, 338, 1180, 60, sf::Color(14, 28, 40));
       drawCard(50, 408, 1180, 60, sf::Color(10, 36, 20));
       drawCard(50, 478, 1180, 40, sf::Color(36, 20, 10));

       char l1[100], l2[100], l3[100], l4[100], l5[160], l6[160], l7[150];
       snprintf(l1, 100, "Registered Patients  : %d", totalPatients);
       snprintf(l2, 100, "Registered Doctors   : %d", totalDoctors);
       snprintf(l3, 100, "Total Appointments   : %d", totalAppts);
       snprintf(l4, 100, "Prescriptions Issued : %d", totalRx);
       snprintf(l5, 160, "Pending: %d   Completed: %d   Cancelled: %d   No-Show: %d", pending, completed, cancelled, noShow);
       snprintf(l6, 160, "Total Bills: %d   Paid: %d (PKR %.2f)   Unpaid: %d (PKR %.2f)", totalBills, paidBills, paidAmt, unpaidBills, unpaidAmt);
       snprintf(l7, 150, "Outstanding Revenue: PKR %.2f", unpaidAmt);

       auto drawRLine = [&](float x, float y, const char* s, sf::Color col, int sz = 17) {
           sf::Text t; t.setFont(font); t.setString(s); t.setCharacterSize(sz); t.setFillColor(col); t.setPosition(x, y); window.draw(t);
           };

       drawRLine(68, 165, l1, sf::Color(0, 215, 145));
       drawRLine(688, 165, l2, sf::Color(0, 195, 255));
       drawRLine(68, 255, l3, sf::Color(0, 195, 255));
       drawRLine(688, 255, l4, sf::Color(185, 150, 255));
       drawRLine(68, 346, "Appointment Breakdown:", sf::Color(130, 165, 200), 14);
       drawRLine(68, 362, l5, sf::Color(255, 220, 100), 15);
       drawRLine(68, 416, "Billing Summary:", sf::Color(130, 165, 200), 14);
       drawRLine(68, 432, l6, sf::Color(0, 215, 145), 14);
       drawRLine(68, 482, l7, sf::Color(255, 178, 0), 16);

       //  Doctor Summary Table 
       drawCard(50, 528, 1180, 28, sf::Color(5, 15, 40));
       sf::Text hdr; hdr.setFont(font); hdr.setCharacterSize(14);
       hdr.setFillColor(sf::Color(0, 200, 240));
       hdr.setString("  Dr#    Name                          Specialization                  Total Appointments");
       hdr.setPosition(58, 533); window.draw(hdr);

       int yp = 558;
       for (int i = 0; i < doctorDB.size(); i++) {
           Doctor* d = *(doctorDB.getAll() + i); if (!d) continue;

           // Count this doctor's appointments
           int docAppts = 0;
           for (int j = 0; j < apptDB.size(); j++) {
               Appointment* a = *(apptDB.getAll() + j);
               if (a && a->getDoctorID() == d->getID()) docAppts++;
           }

           drawCard(50, yp, 1180, 34, i % 2 == 0 ? sf::Color(14, 28, 55) : sf::Color(10, 22, 45));

           char row[300];
           snprintf(row, 300, "  %-6d %-30s %-32s %d",
               d->getID(), d->getName(), d->getSpecialization(), docAppts);

           sf::Text t; t.setFont(font); t.setString(row);
           t.setCharacterSize(14); t.setFillColor(sf::Color(185, 215, 245));
           t.setPosition(58, yp + 9); window.draw(t);

           yp += 36;
           if (yp > 700) break;
       }

       drawBackHint();
       }

    window.display();
}

void HospitalSystem::runUI() {
    while (window.isOpen()) { handleEvents(); render(); }
}