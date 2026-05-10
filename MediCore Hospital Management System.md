MediCore Hospital Management System



Overview

MediCore is a comprehensive, fully functional Hospital Management System built entirely in C++. It features a custom Graphical User Interface (GUI) powered by SFML, moving beyond standard console applications to provide an interactive, real-time user experience. 



The system handles patient bookings, doctor schedules, financial billing, and administrative reporting across three distinct security portals.



Technical Achievements \& Constraints

This project was built under strict academic constraints to demonstrate a deep understanding of low-level C++ mechanics and memory management:

No Standard String Libraries:** Built entirely without `<cstring>` or `std::string`. All string comparisons, concatenations, and formatting (`strcmp`, `strcpy`, etc.) were manually made and implemented though the names are the  same as the original functions.

Strict Pointer Arithmetic:** Array indexing (e.g., `array\[i]`) was strictly forbidden. All array and buffer traversals were handled using pure pointer offset notation (e.g., `\*(array + i)`).

Custom Exception Handling:** Utilizes user-defined exceptions (e.g., `SlotUnavailableException`, `InsufficientFundsException`) to manage edge cases and errors safely to make sure the smooth working of the system and avoiding crashes.

File I/O Persistence:** Real-time reading and writing to local `.txt` databases (`patients.txt`, `doctors.txt`, `appointments.txt`, etc.) to ensure data persistence between sessions.



Features


Patient Portal


1  Book Appointment (with slot conflict check and balance deduction) 

2  Cancel Appointment (with automatic fee refund) 

3  View My Appointments 

4  View Medical Records / Prescriptions 

5  View My Bills 

6  Pay Bill 

7  Top Up Balance 

8  Logout 



Doctor Portal


1  View My Appointments (color-coded by status) 

2  View My Patients (unique patients only) 

3  Write Prescription (auto-marks appointment as completed) 

4 Mark Appointment as Completed 

5  Mark Appointment as No-Show 

6  Logout 



Admin Portal

1  Add Doctor 

2  Remove Doctor 

3  View All Patients 

4  View All Doctors 

5  View All Appointments 

6  View Unpaid Bills (with total outstanding amount) 

7  Discharge Patient (archives all related data) 

8  View Security Log 

9  Generate Daily Report (patients, doctors, appointments, billing, doctor-wise summary) 

10  Logout 


Core Modules

1. Patient Portal

\* Secure login with a 3-strike lockout system.

\* Search for doctors by specialization (case-insensitive).

\* Book, view, and cancel appointments.

\* View and pay hospital bills (automatically updates balance).


2. Doctor Portal
 
\* Secure login interface.

\* View assigned appointments sorted dynamically by date.

\* Access patient medical history.

\* Finalize appointments and write multi-line medical prescriptions.



3. Administrator Portal**

\* View live, hospital-wide statistics (Daily Report).

\* Track total revenue, outstanding bills, and appointment statuses (Pending/Completed/No-Show).



Prerequisites

To compile and run this project, you will need:

1\. A standard C++ Compiler (MinGW/GCC or MSVC).

2\. \*\*SFML (Simple and Fast Multimedia Library)\*\* version 2.5 or higher.



How to Run

1\. Ensure SFML is correctly linked in your IDE (Visual Studio, Code::Blocks, etc.) or via your Makefile.

2\. Verify that all database text files (`patients.txt`, `doctors.txt`, etc.) and the `Arial.ttf` font file are located in the same directory as the executable.          

3\. Compile and build the project.

4\. Run the generated `.exe`.



Default Test Credentials

To evaluate the system, you can use the following default logins (assuming they exist in your local `.txt` files):



Patient Login: ID: `1`

Password: pass123 (Check patients.txt for exact details)

Doctor Login: ID: `101`

Password: `doc456` (Check doctors.txt for exact details)

Admin Login: ID: `1`

Password: `admin`

link for the project 
https://github.com/l250511-sudo/MEDICORE-HOSPITAL-SYSTEM
Developed as an Object-Oriented Programming (OOP) Final Project.

