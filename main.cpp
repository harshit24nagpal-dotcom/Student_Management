#include <iostream>
#include "auth.h"
#include "student.h"

int main() {
    // 1. Admin Authentication Gate
    if (!Auth::authenticateAdmin()) {
        std::cout << "[SYSTEM] Exiting application due to authentication failure.\n";
        return 1;
    }

    // 2. Initialize Database and Load Records
    StudentManager manager;
    if (!manager.loadFromFile()) {
        std::cout << "[SYSTEM WARNING] Failed to read database from data/students.csv. Starting with a blank record.\n";
    } else {
        std::cout << "[SYSTEM] Loaded existing student records successfully.\n\n";
    }

    // 3. Application Main Menu Loop
    bool running = true;
    while (running) {
        std::cout << "=====================================\n";
        std::cout << "     STUDENT MANAGEMENT SYSTEM PRO   \n";
        std::cout << "=====================================\n";
        std::cout << "1. Add Student\n";
        std::cout << "2. View Students\n";
        std::cout << "3. Search Student\n";
        std::cout << "4. Update Student\n";
        std::cout << "5. Delete Student\n";
        std::cout << "6. Sort Records\n";
        std::cout << "7. Attendance Management\n";
        std::cout << "8. CGPA Management\n";
        std::cout << "9. Dashboard Analytics\n";
        std::cout << "10. Export CSV\n";
        std::cout << "11. Exit\n";
        std::cout << "=====================================\n";

        int choice = StudentManager::readInteger("Enter your choice (1-11): ", 1, 11);

        switch (choice) {
            case 1:
                manager.addStudent();
                break;
            case 2:
                manager.viewAllStudents();
                break;
            case 3:
                manager.searchStudent();
                break;
            case 4:
                manager.updateStudent();
                break;
            case 5:
                manager.deleteStudent();
                break;
            case 6:
                manager.sortRecords();
                break;
            case 7:
                manager.manageAttendance();
                break;
            case 8:
                manager.manageCGPA();
                break;
            case 9:
                manager.showDashboardAnalytics();
                break;
            case 10:
                manager.exportCSV();
                break;
            case 11:
                std::cout << "\n[SYSTEM] Thank you for using Student Management System Pro. Saving data and exiting...\n";
                manager.saveToFile();
                running = false;
                break;
            default:
                std::cout << "\n>>> [ERROR] Unexpected choice error. Please try again.\n";
                break;
        }
    }

    return 0;
}
