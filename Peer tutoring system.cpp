#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct User {
    string id;
    string name;
    string contact;
    string password;
};

struct Session {
    string student_id;
    string tutor_id;
    string  date;
    string time;
    string status;
};

bool loginUser(ifstream& in, string& userID) {
    if (!in) {
        cout << "Error: Unable to open file" << "\n";
        return false;
    }

    string id, name, contact, password, enteredID, enteredPassword;
    cout << "Enter ID: ";
    cin >> enteredID;
    cout << "Enter Password: ";
    cin >> enteredPassword;

    while (in >> id) {
        in.ignore();
        getline(in, name, ' ');
        getline(in, contact, ' ');
        getline(in, password);

        if (id == enteredID && password == enteredPassword) {
            userID = id;
            return true;
        }
    }
    return false;
}

void registerUser(ofstream& out, const string& userType) {
    if (!out) {
        cout << "Error: Unable to open file" << "\n";
        return;
    }

    User user;
    cout << "Enter " << userType << " ID: ";
    cin >> user.id;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, user.name);
    cout << "Enter Contact Info: ";
    getline(cin, user.contact);
    cout << "Enter Password: ";
    cin >> user.password;

    out << user.id << " " << user.name << " " << user.contact << " " << user.password << endl;
    out.close();
    cout << userType << " registered successfully!\n";
}

void scheduleSession() {
    string studentID;
    ifstream in("students.txt");
    if (!loginUser(in, studentID)) {
        cout << "Invalid login!\n";
        return;
    }
    in.close();

    in.open("tutoring.txt");
    if (!in) {
        cout << "Error: No tutors available.\n";
        return;
    }

    ofstream out("sessions.txt", ios::app);
    string id, name, contact, password;

    cout << "Available Tutors:\n";
    while (in >> id) {
        in.ignore();
        getline(in, name, ' ');
        getline(in, contact, ' ');
        getline(in, password);
        cout << "ID: " << id << ", Name: " << name << ", Contact: " << contact << endl;
    }
    in.close();

    string tutorID, date, time;
    cout << "Enter Tutor ID to schedule session: ";
    cin >> tutorID;
    cout << "Enter Session Date (YYYY-MM-DD): ";
    cin >> date;
    cout << "Enter Session Time (HH:MM) using 24 hr format: ";
    cin >> time;

    out << studentID << " " << tutorID << " " << date << " " << time << " pending" << endl;
    out.close();
    cout << "Session requested successfully!\n";
}

void approveSessions() {
    string tutorID;
    ifstream in("tutoring.txt");
    if (!loginUser(in, tutorID)) {
        cout << "Invalid login!\n";
        return;
    }
    in.close();

    in.open("sessions.txt");
    if (!in) {
        cout << "Error: No sessions available.\n";
        return;
    }

    ofstream out("temp.txt");
    string studentID, sessionTutorID, date, time, status;
    bool found = false;

    while (in >> studentID >> sessionTutorID >> date >> time >> status) {
        if (sessionTutorID == tutorID && status == "pending") {
            found = true;
            cout << "Approve session for Student ID: " << studentID << " on " << date << " at " << time << "? (yes/no): ";
            string response;
            cin >> response;
            if (response == "yes") {
                out << studentID << " " << tutorID << " " << date << " " << time << " approved" << endl;

                ifstream inStudents("students.txt");
                string id, name, contact, password;
                while (inStudents >> id) {
                    inStudents.ignore();
                    getline(inStudents, name, ' ');
                    getline(inStudents, contact, ' ');
                    getline(inStudents, password);
                    if (id == studentID) {
                        cout << "Student Contact: " << contact << endl;
                        break;
                    }
                }
                inStudents.close();
            } else {
                out << studentID << " " << tutorID << " " << date << " " << time << " pending" << endl;
            }
        } else {
            out << studentID << " " << sessionTutorID << " " << date << " " << time << " " << status << endl;
        }
    }
    in.close();
    out.close();
    remove("sessions.txt");
    rename("temp.txt", "sessions.txt");

    if (!found) {
        cout << "No pending sessions found for you.\n";
    } else {
        cout << "Session approvals updated!\n";
    }
}

int main() {
    int choice;
    while (true) {
        cout << "1. Register Student\n2. Register Tutor\n3. Schedule Session\n4. Approve Sessions\n5. Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                ofstream out("students.txt", ios::app);
                registerUser(out, "Student");
                break;
            }
            case 2: {
                ofstream out("tutoring.txt", ios::app);
                registerUser(out, "Tutor");
                break;
            }
            case 3: scheduleSession(); break;
            case 4: approveSessions(); break;
            case 5: return 0;
            default: cout << "Invalid choice!\n";
        }
    }
}

