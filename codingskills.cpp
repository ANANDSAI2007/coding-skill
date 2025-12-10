// Student Management System - Class-Based Implementation
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cstdio>     // for remove, rename
using namespace std;

using namespace std;

const string STUDENT_DATABASE = "students.txt";
const string AUTH_FILE = "credentials.txt";
const string TEMP_DATABASE = "temp_db.txt";

class StudentManager {
private:
    string loggedInUser;
    string userRole;
    
    bool isValidMarks(float marks) {
        return marks >= 0 && marks <= 100;
    }
    
    bool isValidRollNumber(int roll) {
        return roll > 0;
    }
    
    bool readStudentFromFile(ifstream& file, int& id, string& name, float& marks) {
        return (file >> id >> name >> marks).good();
    }

public:
    bool authenticate() {
        string username, password;
        
        cout << "\n USER AUTHENTICATION " << endl;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        ifstream authFile(AUTH_FILE);
        if (!authFile) {
            cerr << "Error: Authentication database not found!" << endl;
            return false;
        }

        string u, p, role;
        while (authFile >> u >> p >> role) {
            if (username == u && password == p) {
                loggedInUser = username;
                userRole = role;
                authFile.close();
                cout << "Authentication successful!" << endl;
                return true;
            }
        }

        authFile.close();
        cerr << "Error: Authentication failed - Invalid credentials!" << endl;
        return false;
    }

    void insertStudent() {
        int studentRoll;
        string studentName;
        float studentMarks;

        cout << "\n--- Add New Student ---" << endl;
        
        while (true) {
            cout << "Student Roll Number: ";
            if (!(cin >> studentRoll) || !isValidRollNumber(studentRoll)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Error: Enter a valid positive roll number!" << endl;
                continue;
            }
            break;
        }
        
        cout << "Student Name: ";
        cin >> studentName;
        
        while (true) {
            cout << "Student Marks (0-100): ";
            if (!(cin >> studentMarks) || !isValidMarks(studentMarks)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Error: Marks must be between 0 and 100!" << endl;
                continue;
            }
            break;
        }

        ofstream database(STUDENT_DATABASE, ios::app);
        if (!database) {
            cerr << "Error: Cannot write to database!" << endl;
            return;
        }
        
        database << studentRoll << " " << studentName << " " << fixed << setprecision(2) << studentMarks << endl;
        database.close();
        cout << "Record inserted successfully!" << endl;
    }

    void viewAllStudents() {
        cout << "\n Student Records " << endl;
        ifstream database(STUDENT_DATABASE);
        if (!database) {
            cerr << "Error: Database file not found!" << endl;
            return;
        }

        int id;
        string sname;
        float marks;
        int count = 0;

        cout << left << setw(10) << "Roll" << setw(20) << "Name" << setw(10) << "Marks" << endl;
        cout << string(40, '-') << endl;
        
        while (database >> id >> sname >> marks) {
            cout << left << setw(10) << id << setw(20) << sname << fixed << setprecision(2) << marks << endl;
            count++;
        }

        database.close();
        
        if (count == 0) {
            cout << "No student records found." << endl;
        } else {
            cout << "Total records: " << count << endl;
        }
    }

    void queryStudent() {
        int rollToFind;
        cout << "\n Search Student " << endl;
        cout << "Enter Roll Number to search: ";
        
        if (!(cin >> rollToFind) || !isValidRollNumber(rollToFind)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Enter a valid positive roll number!" << endl;
            return;
        }

        ifstream database(STUDENT_DATABASE);
        if (!database) {
            cerr << "Error: Database file not found!" << endl;
            return;
        }

        int id;
        string sname;
        float marks;
        bool exists = false;

        while (database >> id >> sname >> marks) {
            if (id == rollToFind) {
                cout << "\nRecord Found:" << endl;
                cout << "Roll: " << id << endl;
                cout << "Name: " << sname << endl;
                cout << "Marks: " << fixed << setprecision(2) << marks << endl;
                exists = true;
                break;
            }
        }

        if (!exists) {
            cout << "No record found for Roll: " << rollToFind << endl;
        }
        database.close();
    }

    void removeStudent() {
        int rollToDelete;
        cout << "\n Delete Student " << endl;
        cout << "Enter Roll Number to delete: ";
        cin >> rollToDelete;

        ifstream inDB(STUDENT_DATABASE);
        if (!inDB) {
            cerr << "Error: Database file not found!" << endl;
            return;
        }

        ofstream tempDB(TEMP_DATABASE);
        if (!tempDB) {
            cerr << "Error: Cannot create temporary file!" << endl;
            inDB.close();
            return;
        }

        int id;
        string sname;
        float marks;
        bool deleted = false;

        while (inDB >> id >> sname >> marks) {
            if (id != rollToDelete) {
                tempDB << id << " " << sname << " " << fixed << setprecision(2) << marks << endl;
            } else {
                deleted = true;
            }
        }

        inDB.close();
        tempDB.close();

        if (remove(STUDENT_DATABASE.c_str()) != 0) {
            cerr << "Error: Cannot delete old database file!" << endl;
            return;
        }
        
        if (rename(TEMP_DATABASE.c_str(), STUDENT_DATABASE.c_str()) != 0) {
            cerr << "Error: Cannot rename temporary file!" << endl;
            return;
        }

        if (deleted) {
            cout << "Student record deleted successfully!" << endl;
        } else {
            cout << "Roll number not found!" << endl;
        }
    }

    void modifyStudent() {
        int rollToModify;
        cout << "\n Modify Student Record " << endl;
        cout << "Enter Roll Number to modify: ";
        cin >> rollToModify;

        ifstream inDB(STUDENT_DATABASE);
        if (!inDB) {
            cerr << "Error: Database file not found!" << endl;
            return;
        }

        ofstream tempDB(TEMP_DATABASE);
        if (!tempDB) {
            cerr << "Error: Cannot create temporary file!" << endl;
            inDB.close();
            return;
        }

        int id;
        string sname;
        float marks;
        bool modified = false;

        while (inDB >> id >> sname >> marks) {
            if (id == rollToModify) {
                modified = true;
                string newName;
                float newMarks;

                cout << "Current Name: " << sname << endl;
                cout << "Enter New Name: ";
                cin >> newName;
                
                // Validate new marks
                while (true) {
                    cout << "Current Marks: " << fixed << setprecision(2) << marks << endl;
                    cout << "Enter New Marks (0-100): ";
                    if (!(cin >> newMarks) || !isValidMarks(newMarks)) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Error: Marks must be between 0 and 100!" << endl;
                        continue;
                    }
                    break;
                }

                tempDB << id << " " << newName << " " << fixed << setprecision(2) << newMarks << endl;
            } else {
                tempDB << id << " " << sname << " " << fixed << setprecision(2) << marks << endl;
            }
        }

        inDB.close();
        tempDB.close();

        if (remove(STUDENT_DATABASE.c_str()) != 0) {
            cerr << "Error: Cannot delete old database file!" << endl;
            return;
        }
        
        if (rename(TEMP_DATABASE.c_str(), STUDENT_DATABASE.c_str()) != 0) {
            cerr << "Error: Cannot rename temporary file!" << endl;
            return;
        }

        if (modified) {
            cout << "Record updated successfully!" << endl;
        } else {
            cout << "Roll number not found!" << endl;
        }
    }

    void adminPanel() {
        int choice;
        while (true) {
            cout << "\n ADMINISTRATOR PANEL " << endl;
            cout << "1. Add Student Record" << endl;
            cout << "2. View All Records" << endl;
            cout << "3. Search by Roll" << endl;
            cout << "4. Update Record" << endl;
            cout << "5. Delete Record" << endl;
            cout << "6. Exit to Main Menu" << endl;
            cout << "Select Option: ";
            cin >> choice;

            switch(choice) {
                case 1: insertStudent(); break;
                case 2: viewAllStudents(); break;
                case 3: queryStudent(); break;
                case 4: modifyStudent(); break;
                case 5: removeStudent(); break;
                case 6: return;
                default: cout << "Invalid choice!" << endl;
            }
        }
    }

    void staffPanel() {
        int choice;
        while (true) {
            cout << "\n STAFF PANEL " << endl;
            cout << "1. Add New Student" << endl;
            cout << "2. View Records" << endl;
            cout << "3. Search Student" << endl;
            cout << "4. Update Student Info" << endl;
            cout << "5. Logout" << endl;
            cout << "Select Option: ";
            cin >> choice;

            switch(choice) {
                case 1: insertStudent(); break;
                case 2: viewAllStudents(); break;
                case 3: queryStudent(); break;
                case 4: modifyStudent(); break;
                case 5: return;
                default: cout << "Invalid choice!" << endl;
            }
        }
    }

    void guestPanel() {
        int choice;
        while (true) {
            cout << "\n GUEST ACCESS " << endl;
            cout << "1. View All Students" << endl;
            cout << "2. Search Student" << endl;
            cout << "3. Exit" << endl;
            cout << "Select Option: ";
            cin >> choice;

            switch(choice) {
                case 1: viewAllStudents(); break;
                case 2: queryStudent(); break;
                case 3: return;
                default: cout << "Invalid choice!" << endl;
            }
        }
    }

    void launchApplication() {
        cout << "Logged in as: " << userRole << " (" << loggedInUser << ")" << endl;

        string role = userRole;
        transform(role.begin(), role.end(), role.begin(), ::tolower);

        if (role == "admin") {
            adminPanel();
        } else if (role == "staff") {
            staffPanel();
        } else {
            guestPanel();
        }
    }
};

int main() {
    StudentManager system;
    
    cout << "\n" << endl;
    cout << "   STUDENT MANAGEMENT SYSTEM v2.0       " << endl;
    cout << "\n" << endl;

    if (system.authenticate()) {
        system.launchApplication();
        cout << "\nThank you for using the system. Goodbye!" << endl;
    } else {
        cout << "\nAccess Denied. Program will exit." << endl;
        return 1;
    }

    return 0;

}
