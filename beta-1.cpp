#include "HospitalSystem.h"

int main() {
    HospitalSystem medicoreApp; // Creates the main system controller

    medicoreApp.bootDatabases(); // Loads all data from text files
    medicoreApp.runUI();         // Launches the SFML window

    return 0;
}