#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>

// Class representing a single student record with encapsulated fields.
class Student {
private:
    int rollNumber;
    std::string name;
    int age;
    std::string course;
    double cgpa;
    double attendancePercentage;

public:
    Student();
    Student(int roll, const std::string& studentName, int studentAge,
            const std::string& studentCourse, double studentCgpa, double studentAttendance);

    // Getters (Encapsulation)
    int getRollNumber() const;
    std::string getName() const;
    int getAge() const;
    std::string getCourse() const;
    double getCGPA() const;
    double getAttendancePercentage() const;

    // Setters
    void setRollNumber(int roll);
    void setName(const std::string& studentName);
    void setAge(int studentAge);
    void setCourse(const std::string& studentCourse);
    void setCGPA(double studentCgpa);
    void setAttendancePercentage(double studentAttendance);

    // Serialization helper: Convert student to a single CSV-formatted line
    std::string toCSV() const;

    // Deserialization helper: Create a student from a CSV-formatted line
    static Student fromCSV(const std::string& csvLine, bool& success);
};

// Manager class handling student records collection, file storage, CLI menu interfaces
class StudentManager {
private:
    std::vector<Student> students;
    std::string dataFilePath;
    std::string logFilePath;

    // Utility methods
    void logUpdate(int roll, const std::string& field, const std::string& oldVal, const std::string& newVal) const;
    std::string getCurrentTimestamp() const;
    bool rollNumberExists(int roll) const;

public:
    StudentManager(const std::string& dataPath = "data/students.csv",
                   const std::string& logPath = "data/updates.log");

    // File Operations
    bool loadFromFile();
    bool saveToFile() const;
    bool exportCSV() const;

    // Core CRUD
    void addStudent();
    void viewAllStudents() const;
    void searchStudent();
    void updateStudent();
    void deleteStudent();

    // Sorting Procedures
    void sortRecords();

    // Sub-Modules
    void manageAttendance();
    void manageCGPA();

    // Analytics Dashboard
    void showDashboardAnalytics() const;

    // Core Helpers & Input Verification
    int findIndexLinear(int roll) const;
    int findIndexBinary(int roll); // Sorts list by Roll first to guarantee binary search correctness

    // Clean Input Helpers (protects against infinite loops / invalid types)
    static int readInteger(const std::string& prompt, int minVal = 1, int maxVal = 2147483647);
    static double readDouble(const std::string& prompt, double minVal = 0.0, double maxVal = 100.0);
    static std::string readLine(const std::string& prompt, bool allowEmpty = false);
    static std::string toUpper(std::string str);
    static std::string trim(const std::string& str);
};

#endif // STUDENT_H
