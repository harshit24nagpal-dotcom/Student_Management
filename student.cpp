#include "student.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <ctime>

// ==========================================
// STUDENT CLASS IMPLEMENTATION
// ==========================================

Student::Student() 
    : rollNumber(0), name(""), age(0), course(""), cgpa(0.0), attendancePercentage(0.0) {}

Student::Student(int roll, const std::string& studentName, int studentAge,
                 const std::string& studentCourse, double studentCgpa, double studentAttendance)
    : rollNumber(roll), name(studentName), age(studentAge), 
      course(studentCourse), cgpa(studentCgpa), attendancePercentage(studentAttendance) {}

int Student::getRollNumber() const { return rollNumber; }
std::string Student::getName() const { return name; }
int Student::getAge() const { return age; }
std::string Student::getCourse() const { return course; }
double Student::getCGPA() const { return cgpa; }
double Student::getAttendancePercentage() const { return attendancePercentage; }

void Student::setRollNumber(int roll) { rollNumber = roll; }
void Student::setName(const std::string& studentName) { name = studentName; }
void Student::setAge(int studentAge) { age = studentAge; }
void Student::setCourse(const std::string& studentCourse) { course = studentCourse; }
void Student::setCGPA(double studentCgpa) { cgpa = studentCgpa; }
void Student::setAttendancePercentage(double studentAttendance) { attendancePercentage = studentAttendance; }

std::string Student::toCSV() const {
    std::stringstream ss;
    // Format double values to 2 decimal places in CSV representation
    ss << rollNumber << ","
       << name << ","
       << age << ","
       << course << ","
       << std::fixed << std::setprecision(2) << cgpa << ","
       << std::fixed << std::setprecision(2) << attendancePercentage;
    return ss.str();
}

Student Student::fromCSV(const std::string& csvLine, bool& success) {
    std::stringstream ss(csvLine);
    std::string rollStr, nameStr, ageStr, courseStr, cgpaStr, attStr;

    if (std::getline(ss, rollStr, ',') &&
        std::getline(ss, nameStr, ',') &&
        std::getline(ss, ageStr, ',') &&
        std::getline(ss, courseStr, ',') &&
        std::getline(ss, cgpaStr, ',') &&
        std::getline(ss, attStr, ',')) {
        
        try {
            int roll = std::stoi(rollStr);
            int age = std::stoi(ageStr);
            double cgpa = std::stod(cgpaStr);
            double att = std::stod(attStr);
            success = true;
            return Student(roll, nameStr, age, courseStr, cgpa, att);
        } catch (...) {
            // Parsing failure (e.g. invalid formatting)
        }
    }
    success = false;
    return Student();
}


// ==========================================
// STUDENT MANAGER CLASS IMPLEMENTATION
// ==========================================

StudentManager::StudentManager(const std::string& dataPath, const std::string& logPath)
    : dataFilePath(dataPath), logFilePath(logPath) {}

std::string StudentManager::getCurrentTimestamp() const {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localTime);
    return std::string(buf);
}

void StudentManager::logUpdate(int roll, const std::string& field, const std::string& oldVal, const std::string& newVal) const {
    std::ofstream logFile(logFilePath, std::ios::app);
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTimestamp() << "] Roll " << roll 
                << ": Updated " << field << " from \"" << oldVal 
                << "\" to \"" << newVal << "\"\n";
        logFile.close();
    }
}

bool StudentManager::rollNumberExists(int roll) const {
    return findIndexLinear(roll) != -1;
}

int StudentManager::findIndexLinear(int roll) const {
    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].getRollNumber() == roll) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int StudentManager::findIndexBinary(int roll) {
    // Binary Search requires records to be sorted by Roll Number first
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.getRollNumber() < b.getRollNumber();
    });

    int low = 0;
    int high = static_cast<int>(students.size()) - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (students[mid].getRollNumber() == roll) {
            return mid;
        } else if (students[mid].getRollNumber() < roll) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

bool StudentManager::loadFromFile() {
    students.clear();
    std::ifstream file(dataFilePath);
    if (!file.is_open()) {
        // If file doesn't exist, we will create it automatically upon saving
        return true; 
    }

    std::string line;
    bool isHeader = true;
    while (std::getline(file, line)) {
        if (isHeader) {
            isHeader = false; // Skip the CSV header line
            continue;
        }
        if (trim(line).empty()) continue;
        
        bool success = false;
        Student s = Student::fromCSV(line, success);
        if (success) {
            students.push_back(s);
        }
    }
    file.close();
    return true;
}

bool StudentManager::saveToFile() const {
    // Ensure data directory exists
    // (In C++17 we could use std::filesystem, but to avoid compiler issues on older systems, 
    // we assume the data/ directory exists. We'll pre-create it or it is already in the workspace).
    std::ofstream file(dataFilePath);
    if (!file.is_open()) {
        return false;
    }

    file << "Roll,Name,Age,Course,CGPA,Attendance\n";
    for (const auto& s : students) {
        file << s.toCSV() << "\n";
    }
    file.close();
    return true;
}

// 1. Add Student
void StudentManager::addStudent() {
    std::cout << "\n=====================================\n";
    std::cout << "          ADD NEW STUDENT            \n";
    std::cout << "=====================================\n";

    int roll = readInteger("Enter Roll Number: ", 1, 999999);
    if (rollNumberExists(roll)) {
        std::cout << "\n>>> [ERROR] Duplicate Roll Number! Student with Roll " << roll << " already exists.\n";
        return;
    }

    std::string name = readLine("Enter Full Name: ");
    int age = readInteger("Enter Age: ", 5, 120);
    std::string course = readLine("Enter Course/Major: ");
    double cgpa = readDouble("Enter CGPA (0.00 - 10.00): ", 0.0, 10.0);
    double att = readDouble("Enter Attendance % (0.00 - 100.00): ", 0.0, 100.0);

    students.emplace_back(roll, name, age, course, cgpa, att);

    if (saveToFile()) {
        std::cout << "\n>>> [SUCCESS] Student record added and saved successfully!\n";
    } else {
        std::cout << "\n>>> [WARNING] Student record added, but failed to write to database file!\n";
    }
}

// 2. View All Students
void StudentManager::viewAllStudents() const {
    std::cout << "\n========================================================================================\n";
    std::cout << "                                   STUDENT REGISTRY                                     \n";
    std::cout << "========================================================================================\n";
    
    if (students.empty()) {
        std::cout << "                    No student records found in the database.                           \n";
        std::cout << "========================================================================================\n";
        return;
    }

    std::cout << std::left 
              << std::setw(10) << "Roll No"
              << std::setw(25) << "Name"
              << std::setw(8)  << "Age"
              << std::setw(20) << "Course"
              << std::setw(10) << "CGPA"
              << std::setw(12) << "Attendance" << "\n";
    std::cout << "----------------------------------------------------------------------------------------\n";

    for (const auto& s : students) {
        std::cout << std::left 
                  << std::setw(10) << s.getRollNumber()
                  << std::setw(25) << (s.getName().length() > 22 ? s.getName().substr(0, 20) + ".." : s.getName())
                  << std::setw(8)  << s.getAge()
                  << std::setw(20) << (s.getCourse().length() > 18 ? s.getCourse().substr(0, 16) + ".." : s.getCourse())
                  << std::fixed << std::setprecision(2)
                  << std::setw(10) << s.getCGPA()
                  << std::setw(12) << (std::to_string(static_cast<int>(s.getAttendancePercentage())) + "%") << "\n";
    }
    std::cout << "========================================================================================\n";
    std::cout << "Total Students: " << students.size() << "\n\n";
}

// 3. Search Student (Linear & Binary options)
void StudentManager::searchStudent() {
    if (students.empty()) {
        std::cout << "\n>>> [ERROR] Database is empty. Add records first.\n";
        return;
    }

    std::cout << "\n=====================================\n";
    std::cout << "           SEARCH STUDENT            \n";
    std::cout << "=====================================\n";
    std::cout << "1. Linear Search (unsorted/direct)\n";
    std::cout << "2. Binary Search (sorts by Roll first)\n";
    std::cout << "3. Back\n";
    
    int choice = readInteger("Select search method (1-3): ", 1, 3);
    if (choice == 3) return;

    int roll = readInteger("Enter Roll Number to search: ", 1, 999999);
    int index = -1;

    if (choice == 1) {
        std::cout << "Executing Linear Search...\n";
        index = findIndexLinear(roll);
    } else {
        std::cout << "Executing Binary Search (auto-sorting database by Roll Number first)...\n";
        index = findIndexBinary(roll);
    }

    if (index != -1) {
        const auto& s = students[index];
        std::cout << "\n=====================================\n";
        std::cout << "        STUDENT RECORD FOUND         \n";
        std::cout << "=====================================\n";
        std::cout << " Roll Number : " << s.getRollNumber() << "\n";
        std::cout << " Name        : " << s.getName() << "\n";
        std::cout << " Age         : " << s.getAge() << "\n";
        std::cout << " Course      : " << s.getCourse() << "\n";
        std::cout << " CGPA        : " << std::fixed << std::setprecision(2) << s.getCGPA() << " / 10.00\n";
        std::cout << " Attendance  : " << std::fixed << std::setprecision(2) << s.getAttendancePercentage() << "%\n";
        std::cout << "=====================================\n\n";
    } else {
        std::cout << "\n>>> [ERROR] Student with Roll Number " << roll << " was not found in the registry.\n\n";
    }
}

// 4. Update Student Details
void StudentManager::updateStudent() {
    if (students.empty()) {
        std::cout << "\n>>> [ERROR] Database is empty. Add records first.\n";
        return;
    }

    std::cout << "\n=====================================\n";
    std::cout << "           UPDATE STUDENT            \n";
    std::cout << "=====================================\n";
    int roll = readInteger("Enter Roll Number to update: ", 1, 999999);

    int index = findIndexLinear(roll);
    if (index == -1) {
        std::cout << "\n>>> [ERROR] Student with Roll Number " << roll << " not found.\n";
        return;
    }

    Student& s = students[index];
    bool updating = true;

    while (updating) {
        std::cout << "\n--- Current Record for " << s.getName() << " (Roll " << s.getRollNumber() << ") ---\n";
        std::cout << "1. Name       : " << s.getName() << "\n";
        std::cout << "2. Age        : " << s.getAge() << "\n";
        std::cout << "3. Course     : " << s.getCourse() << "\n";
        std::cout << "4. CGPA       : " << s.getCGPA() << "\n";
        std::cout << "5. Attendance : " << s.getAttendancePercentage() << "%\n";
        std::cout << "6. Finish & Save Updates\n";
        
        int choice = readInteger("Select field to update (1-6): ", 1, 6);
        switch (choice) {
            case 1: {
                std::string oldVal = s.getName();
                std::string newVal = readLine("Enter New Name: ");
                s.setName(newVal);
                logUpdate(s.getRollNumber(), "Name", oldVal, newVal);
                std::cout << ">>> Name updated temporarily in memory.\n";
                break;
            }
            case 2: {
                std::string oldVal = std::to_string(s.getAge());
                int newVal = readInteger("Enter New Age: ", 5, 120);
                s.setAge(newVal);
                logUpdate(s.getRollNumber(), "Age", oldVal, std::to_string(newVal));
                std::cout << ">>> Age updated temporarily in memory.\n";
                break;
            }
            case 3: {
                std::string oldVal = s.getCourse();
                std::string newVal = readLine("Enter New Course: ");
                s.setCourse(newVal);
                logUpdate(s.getRollNumber(), "Course", oldVal, newVal);
                std::cout << ">>> Course updated temporarily in memory.\n";
                break;
            }
            case 4: {
                std::string oldVal = std::to_string(s.getCGPA());
                double newVal = readDouble("Enter New CGPA (0.00-10.00): ", 0.0, 10.0);
                s.setCGPA(newVal);
                logUpdate(s.getRollNumber(), "CGPA", oldVal, std::to_string(newVal));
                std::cout << ">>> CGPA updated temporarily in memory.\n";
                break;
            }
            case 5: {
                std::string oldVal = std::to_string(s.getAttendancePercentage());
                double newVal = readDouble("Enter New Attendance (0.00-100.00): ", 0.0, 100.0);
                s.setAttendancePercentage(newVal);
                logUpdate(s.getRollNumber(), "Attendance", oldVal, std::to_string(newVal));
                std::cout << ">>> Attendance updated temporarily in memory.\n";
                break;
            }
            case 6:
                updating = false;
                break;
        }
    }

    if (saveToFile()) {
        std::cout << "\n>>> [SUCCESS] Student updates saved persistently and logged to history file!\n";
    } else {
        std::cout << "\n>>> [WARNING] Updates applied in-memory, but saving to database file failed!\n";
    }
}

// 5. Delete Student Record (with confirmation)
void StudentManager::deleteStudent() {
    if (students.empty()) {
        std::cout << "\n>>> [ERROR] Database is empty. Add records first.\n";
        return;
    }

    std::cout << "\n=====================================\n";
    std::cout << "          DELETE STUDENT             \n";
    std::cout << "=====================================\n";
    int roll = readInteger("Enter Roll Number to delete: ", 1, 999999);

    int index = findIndexLinear(roll);
    if (index == -1) {
        std::cout << "\n>>> [ERROR] Student with Roll Number " << roll << " not found.\n";
        return;
    }

    const auto& s = students[index];
    std::cout << "\n--- Student Record Found ---\n";
    std::cout << "Name  : " << s.getName() << "\n";
    std::cout << "Roll  : " << s.getRollNumber() << "\n";
    std::cout << "Course: " << s.getCourse() << "\n";
    std::cout << "----------------------------\n";

    std::string confirm = readLine("Are you absolutely sure you want to delete this record? (yes/no): ");
    if (toUpper(confirm) == "YES" || toUpper(confirm) == "Y") {
        students.erase(students.begin() + index);
        if (saveToFile()) {
            std::cout << "\n>>> [SUCCESS] Student record has been deleted and database saved.\n";
        } else {
            std::cout << "\n>>> [WARNING] Student removed from memory, but database write failed.\n";
        }
    } else {
        std::cout << "\n>>> [INFO] Deletion cancelled.\n";
    }
}

// 6. Sort Records Menu
void StudentManager::sortRecords() {
    if (students.empty()) {
        std::cout << "\n>>> [ERROR] Database is empty. Add records first.\n";
        return;
    }

    std::cout << "\n=====================================\n";
    std::cout << "            SORT RECORDS             \n";
    std::cout << "=====================================\n";
    std::cout << "1. Sort by Roll Number (Ascending)\n";
    std::cout << "2. Sort by Name (Alphabetical)\n";
    std::cout << "3. Sort by CGPA (Highest First)\n";
    std::cout << "4. Sort by Attendance (Highest First)\n";
    std::cout << "5. Cancel\n";

    int choice = readInteger("Enter sort option (1-5): ", 1, 5);
    if (choice == 5) return;

    switch (choice) {
        case 1:
            std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
                return a.getRollNumber() < b.getRollNumber();
            });
            std::cout << "\n>>> Database sorted by Roll Number (Ascending).\n";
            break;
        case 2:
            std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
                return toUpper(a.getName()) < toUpper(b.getName());
            });
            std::cout << "\n>>> Database sorted by Name (Alphabetical).\n";
            break;
        case 3:
            std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
                return a.getCGPA() > b.getCGPA(); // Descending (highest first)
            });
            std::cout << "\n>>> Database sorted by CGPA (Highest First).\n";
            break;
        case 4:
            std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
                return a.getAttendancePercentage() > b.getAttendancePercentage(); // Descending
            });
            std::cout << "\n>>> Database sorted by Attendance (Highest First).\n";
            break;
    }

    // Auto-save the sorted database representation
    saveToFile();
    viewAllStudents();
}

// 7. Attendance Management Module
void StudentManager::manageAttendance() {
    bool running = true;
    while (running) {
        std::cout << "\n=====================================\n";
        std::cout << "       ATTENDANCE MANAGEMENT         \n";
        std::cout << "=====================================\n";
        std::cout << "1. Add/Update Attendance Percentage\n";
        std::cout << "2. View Attendance of Individual Student\n";
        std::cout << "3. Back to Main Menu\n";

        int choice = readInteger("Enter choice (1-3): ", 1, 3);
        if (choice == 3) {
            running = false;
            break;
        }

        if (students.empty()) {
            std::cout << "\n>>> [ERROR] Database is empty. Add records first.\n";
            continue;
        }

        int roll = readInteger("Enter Roll Number: ", 1, 999999);
        int index = findIndexLinear(roll);
        if (index == -1) {
            std::cout << "\n>>> [ERROR] Student not found.\n";
            continue;
        }

        Student& s = students[index];

        if (choice == 1) {
            std::string oldVal = std::to_string(s.getAttendancePercentage()) + "%";
            double att = readDouble("Enter New Attendance % (0.00-100.00): ", 0.0, 100.0);
            s.setAttendancePercentage(att);
            logUpdate(s.getRollNumber(), "Attendance (Sub-module)", oldVal, std::to_string(att) + "%");
            saveToFile();
            std::cout << "\n>>> [SUCCESS] Attendance updated successfully!\n";
        } else {
            std::cout << "\n=====================================\n";
            std::cout << "        STUDENT ATTENDANCE REPORT     \n";
            std::cout << "=====================================\n";
            std::cout << " Roll Number : " << s.getRollNumber() << "\n";
            std::cout << " Name        : " << s.getName() << "\n";
            std::cout << " Attendance  : " << std::fixed << std::setprecision(2) << s.getAttendancePercentage() << "%\n";
            std::cout << " Status      : " << (s.getAttendancePercentage() >= 75.0 ? "Satisfactory (Safe)" : "Low Attendance (Critical)") << "\n";
            std::cout << "=====================================\n\n";
        }
    }
}

// 8. CGPA Management Module
void StudentManager::manageCGPA() {
    bool running = true;
    while (running) {
        std::cout << "\n=====================================\n";
        std::cout << "           CGPA MANAGEMENT           \n";
        std::cout << "=====================================\n";
        std::cout << "1. Add/Update CGPA\n";
        std::cout << "2. View Topper\n";
        std::cout << "3. Display Top 5 Students by CGPA\n";
        std::cout << "4. Back to Main Menu\n";

        int choice = readInteger("Enter choice (1-4): ", 1, 4);
        if (choice == 4) {
            running = false;
            break;
        }

        if (students.empty()) {
            std::cout << "\n>>> [ERROR] Database is empty. Add records first.\n";
            continue;
        }

        if (choice == 1) {
            int roll = readInteger("Enter Roll Number: ", 1, 999999);
            int index = findIndexLinear(roll);
            if (index == -1) {
                std::cout << "\n>>> [ERROR] Student not found.\n";
                continue;
            }
            Student& s = students[index];
            std::string oldVal = std::to_string(s.getCGPA());
            double cgpa = readDouble("Enter New CGPA (0.00-10.00): ", 0.0, 10.0);
            s.setCGPA(cgpa);
            logUpdate(s.getRollNumber(), "CGPA (Sub-module)", oldVal, std::to_string(cgpa));
            saveToFile();
            std::cout << "\n>>> [SUCCESS] CGPA updated successfully!\n";
        } 
        else if (choice == 2) {
            auto topperIt = std::max_element(students.begin(), students.end(), [](const Student& a, const Student& b) {
                return a.getCGPA() < b.getCGPA();
            });
            if (topperIt != students.end()) {
                std::cout << "\n=====================================\n";
                std::cout << "          ACADEMIC TOPPER            \n";
                std::cout << "=====================================\n";
                std::cout << " Name        : " << topperIt->getName() << "\n";
                std::cout << " Roll Number : " << topperIt->getRollNumber() << "\n";
                std::cout << " Course      : " << topperIt->getCourse() << "\n";
                std::cout << " CGPA        : " << std::fixed << std::setprecision(2) << topperIt->getCGPA() << " / 10.00\n";
                std::cout << " Attendance  : " << topperIt->getAttendancePercentage() << "%\n";
                std::cout << "=====================================\n\n";
            }
        } 
        else if (choice == 3) {
            std::vector<Student> sortedList = students;
            std::sort(sortedList.begin(), sortedList.end(), [](const Student& a, const Student& b) {
                return a.getCGPA() > b.getCGPA();
            });

            std::cout << "\n=================================================================\n";
            std::cout << "                   TOP 5 ACADEMIC RANKINGS                       \n";
            std::cout << "=================================================================\n";
            std::cout << std::left 
                      << std::setw(8)  << "Rank"
                      << std::setw(10) << "Roll No"
                      << std::setw(25) << "Name"
                      << std::setw(12) << "CGPA"
                      << std::setw(15) << "Course" << "\n";
            std::cout << "-----------------------------------------------------------------\n";

            size_t displayCount = std::min(static_cast<size_t>(5), sortedList.size());
            for (size_t i = 0; i < displayCount; ++i) {
                std::cout << std::left
                          << std::setw(8)  << (i + 1)
                          << std::setw(10) << sortedList[i].getRollNumber()
                          << std::setw(25) << (sortedList[i].getName().length() > 22 ? sortedList[i].getName().substr(0, 20) + ".." : sortedList[i].getName())
                          << std::fixed << std::setprecision(2)
                          << std::setw(12) << sortedList[i].getCGPA()
                          << std::setw(15) << (sortedList[i].getCourse().length() > 13 ? sortedList[i].getCourse().substr(0, 11) + ".." : sortedList[i].getCourse()) << "\n";
            }
            std::cout << "=================================================================\n\n";
        }
    }
}

// 9. Dashboard Analytics
void StudentManager::showDashboardAnalytics() const {
    if (students.empty()) {
        std::cout << "\n>>> [ERROR] Database is empty. Dashboard has no data.\n";
        return;
    }

    double totalCGPA = 0;
    double totalAttendance = 0;
    double highestCGPA = students[0].getCGPA();
    double lowestCGPA = students[0].getCGPA();
    const Student* topper = &students[0];

    std::map<std::string, int> courseDistribution;

    for (const auto& s : students) {
        totalCGPA += s.getCGPA();
        totalAttendance += s.getAttendancePercentage();

        if (s.getCGPA() > highestCGPA) {
            highestCGPA = s.getCGPA();
            topper = &s;
        }
        if (s.getCGPA() < lowestCGPA) {
            lowestCGPA = s.getCGPA();
        }

        // course standardisation: convert to uppercase to group correctly
        std::string courseNorm = toUpper(trim(s.getCourse()));
        courseDistribution[courseNorm]++;
    }

    double avgCGPA = totalCGPA / students.size();
    double avgAttendance = totalAttendance / students.size();

    std::cout << "\n=======================================================\n";
    std::cout << "               ANALYTICS DASHBOARD                     \n";
    std::cout << "=======================================================\n";
    std::cout << " [1] Enrollment Statistics:\n";
    std::cout << "     - Total Enrolled Students   : " << students.size() << "\n";
    std::cout << "\n [2] Academic CGPA Performance:\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "     - Average Class CGPA        : " << avgCGPA << " / 10.00\n";
    std::cout << "     - Highest CGPA Achieved     : " << highestCGPA << "\n";
    std::cout << "     - Lowest CGPA Recorded      : " << lowestCGPA << "\n";
    std::cout << "\n [3] Attendance Performance:\n";
    std::cout << "     - Average Class Attendance  : " << avgAttendance << "%\n";
    std::cout << "\n [4] Star Performer:\n";
    std::cout << "     - Student Name              : " << topper->getName() << "\n";
    std::cout << "     - Roll Number               : " << topper->getRollNumber() << "\n";
    std::cout << "     - CGPA                      : " << topper->getCGPA() << "\n";
    std::cout << "\n [5] Enrollment Distribution by Course:\n";
    for (const auto& pair : courseDistribution) {
        std::cout << "     - " << std::left << std::setw(25) << pair.first << ": " << pair.second << " Student(s)\n";
    }
    std::cout << "=======================================================\n\n";
}

// 10. CSV Export Option (Allows specifying custom paths)
bool StudentManager::exportCSV() const {
    if (students.empty()) {
        std::cout << "\n>>> [ERROR] Database is empty. Nothing to export.\n";
        return false;
    }

    std::cout << "\n=====================================\n";
    std::cout << "             CSV EXPORT              \n";
    std::cout << "=====================================\n";
    std::cout << "Default export directory: data/students.csv\n";
    
    std::string pathInput = readLine("Enter export filepath (Press Enter to use default 'data/students.csv'): ", true);
    if (pathInput.empty()) {
        pathInput = "data/students.csv";
    }

    std::ofstream file(pathInput);
    if (!file.is_open()) {
        std::cout << "\n>>> [ERROR] Could not open filepath '" << pathInput << "' for writing.\n";
        return false;
    }

    file << "Roll,Name,Age,Course,CGPA,Attendance\n";
    for (const auto& s : students) {
        file << s.toCSV() << "\n";
    }
    file.close();

    std::cout << "\n>>> [SUCCESS] Database exported to '" << pathInput << "' successfully.\n";
    return true;
}


// ==========================================
// STATIC HELPERS IMPLEMENTATION
// ==========================================

int StudentManager::readInteger(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input)) {
            std::cin.clear();
            return minVal;
        }
        input = trim(input);
        try {
            size_t idx;
            int val = std::stoi(input, &idx);
            if (idx == input.length() && val >= minVal && val <= maxVal) {
                return val;
            }
        } catch (...) {
            // Silence exceptions, output error below
        }
        std::cout << ">>> [ERROR] Invalid input. Please enter a whole number between " 
                  << minVal << " and " << maxVal << ".\n";
    }
}

double StudentManager::readDouble(const std::string& prompt, double minVal, double maxVal) {
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input)) {
            std::cin.clear();
            return minVal;
        }
        input = trim(input);
        try {
            size_t idx;
            double val = std::stod(input, &idx);
            if (idx == input.length() && val >= minVal && val <= maxVal) {
                return val;
            }
        } catch (...) {
            // Silence exceptions, output error below
        }
        std::cout << ">>> [ERROR] Invalid input. Please enter a number between " 
                  << minVal << " and " << maxVal << ".\n";
    }
}

std::string StudentManager::readLine(const std::string& prompt, bool allowEmpty) {
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input)) {
            std::cin.clear();
            return "";
        }
        input = trim(input);
        if (input.empty() && !allowEmpty) {
            std::cout << ">>> [ERROR] Input cannot be empty. Please enter a valid text.\n";
            continue;
        }
        return input;
    }
}

std::string StudentManager::toUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string StudentManager::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}
