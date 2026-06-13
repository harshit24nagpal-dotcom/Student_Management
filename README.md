# Student Management System Pro

A high-performance, modular, and professional console-based Student Management System built in **C++17**. The system leverages Object-Oriented Programming (OOP) principles, Standard Template Library (STL) containers/algorithms, persistent File Handling (CSV), and includes an administration gate, robust input handling, and detailed statistics analytics.

---

## Key Features

### 🔐 1. Authentication Module
* **Admin Login Access**: Restricts unauthorized system access.
* **Console Masking**: Secure character-by-character password input visual masking (displaying `*`).
* **Attempt Counter**: Allows up to 3 login attempts, terminating execution upon repeated failures.

### 📝 2. Full CRUD Operations
* **Add Student**: Insert new records with validation (prevents duplicate Roll Numbers).
* **View Records**: Formatted tabular view aligning columns dynamically.
* **Search Module**: Choose between **Linear Search** or optimized **Binary Search** (automatically pre-sorting records by Roll Number).
* **Update Records**: Edit individual fields (Name, Age, Course, CGPA, Attendance) with details auto-saved.
* **Delete Records**: Prompts for confirmation before permanent record removal.

### 💾 3. File Handling & History Logging
* **Persistent DB**: Student details automatically saved to and loaded from `data/students.csv`.
* **CSV Exports**: Allows exporting records to custom paths or standard format.
* **Timestamp Logging**: Every data change (updates, modifications) is logged with precise timestamps to `data/updates.log`.

### 📊 4. Analytics & Dashboards
* **Academic Leaderboard**: Instantly view the topper or top 5 ranked students by CGPA.
* **Statistics Dashboard**: 
  * Total student count.
  * Class average, maximum, and minimum CGPA.
  * Class average attendance percentage.
  * Student distribution per major/course.
* **Attendance Tracking**: Individual student reports with warning flags for attendance falling below 75%.

---

## Project Structure

```
Student-Management-System-Pro/
│
├── src/
│   ├── main.cpp          # Application entry point & menu router
│   ├── student.h         # Student & StudentManager declarations
│   ├── student.cpp       # Student & StudentManager implementation
│   ├── auth.h            # Admin authenticator declarations
│   └── auth.cpp          # Admin authenticator implementation
│
├── data/
│   ├── students.csv      # Persistent flat database storage
│   └── updates.log       # Audit log for record changes
│
├── screenshots/          # Application walkthrough visuals
│
├── README.md             # Project documentation
├── LICENSE               # MIT License terms
└── .gitignore            # Git exclusion configuration
```

---

## Technical Highlights
* **Robust Input Validation**: Shielded against infinite stream-loops caused by typing text in numeric fields. It parses and trims lines using strict boundaries.
* **Cross-platform Input Handling**: Employs Windows `_getch()` and POSIX `termios` configuration to capture keystrokes for secure password entry.
* **Standard Library (STL)**: Utilizes `std::vector` for dynamic storage, `std::sort` for custom comparator sorting, `std::max_element` for quick max searches, and `std::map` for course grouping.

---

## Installation & Compilation Guide

### Prerequisites
* A C++ compiler supporting the **C++17** standard (e.g., `g++` 7.0+ or MSVC 2017+).

### Building on Windows (using g++)
Open a command prompt or PowerShell inside the project root and run:
```bash
g++ -std=c++17 -Wall -Wextra -O2 src/main.cpp src/student.cpp src/auth.cpp -o student_system.exe
```

### Building on Linux/macOS
Open a terminal inside the project root and run:
```bash
g++ -std=c++17 -Wall -Wextra -O2 src/main.cpp src/student.cpp src/auth.cpp -o student_system
```

### Running the Application
* **Windows**: `.\student_system.exe`
* **Linux/macOS**: `./student_system`

> **Admin Login Credentials**
> * **Username**: `admin`
> * **Password**: `1234`

---

## Screenshots Section
*(Walkthrough screenshots showcasing the interactive CLI dashboard, lists, and statistics go here)*

---

## Future Enhancements
We plan to expand this CLI prototype into a multi-platform corporate application by implementing:
* **GUI Version using Qt**: Developing a modern desktop interface.
* **SQL Database Integration**: Replacing the flat CSV file with MySQL/PostgreSQL.
* **Multi-user Login**: Adding student/teacher roles and distinct accessibility privileges.
* **Report Cards & PDF Exports**: Generating printable report cards.
* **Web Portal (MERN Stack)**: Transitioning into a cloud web application using React and Node.js.

---

## License
Distributed under the **MIT License**. See the `LICENSE` file for details.

---

## Author
* **Developer**
* Github: [@developer](https://github.com/developer)
