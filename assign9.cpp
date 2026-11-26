#include <iostream>
#include <string>
using namespace std;

// Node for each course a student takes
struct Course {
    string courseID;
    string courseName;
    Course* next;
};

// Node for each student record
struct Student {
    int rollNo;
    string name;
    string department;
    int semester;
    Course* courses; // Head of this student's course list
    Student* next;   // Next student in the hash bucket
};

const int TABLE_SIZE = 10;
Student* buckets[TABLE_SIZE] = {nullptr};

// Simple modulo hash function based on roll number
int getBucketIndex(int rollNo) {
    return rollNo % TABLE_SIZE;
}

// Finds a student pointer by roll number, or nullptr if missing
Student* findStudent(int rollNo) {
    int idx = getBucketIndex(rollNo);
    Student* curr = buckets[idx];

    while (curr != nullptr) {
        if (curr->rollNo == rollNo) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

void registerStudent() {
    int rollNo, semester;
    string name, department;

    cout << "Enter Roll Number: ";
    cin >> rollNo;

    if (findStudent(rollNo) != nullptr) {
        cout << "A student with this roll number already exists.\n";
        return;
    }

    cin.ignore(); // Clear leftover newline before getline
    cout << "Enter Student Name: ";
    getline(cin, name);

    cout << "Enter Department: ";
    getline(cin, department);

    cout << "Enter Semester: ";
    cin >> semester;

    Student* s = new Student;
    s->rollNo = rollNo;
    s->name = name;
    s->department = department;
    s->semester = semester;
    s->courses = nullptr;

    // Insert at the front of the chain
    int idx = getBucketIndex(rollNo);
    s->next = buckets[idx];
    buckets[idx] = s;

    cout << "Student registered.\n";
}

void updateStudent() {
    int rollNo;
    cout << "Enter Roll Number: ";
    cin >> rollNo;

    Student* s = findStudent(rollNo);
    if (s == nullptr) {
        cout << "Student not found.\n";
        return;
    }

    cin.ignore();
    cout << "Enter New Name: ";
    getline(cin, s->name);

    cout << "Enter New Department: ";
    getline(cin, s->department);

    cout << "Enter New Semester: ";
    cin >> s->semester;

    cout << "Student details updated.\n";
}

void enrollCourse() {
    int rollNo;
    string courseID, courseName;

    cout << "Enter Roll Number: ";
    cin >> rollNo;

    Student* s = findStudent(rollNo);
    if (s == nullptr) {
        cout << "Student not found.\n";
        return;
    }

    cout << "Enter Course ID: ";
    cin >> courseID;

    // Avoid duplicate enrollment in the same course
    Course* c = s->courses;
    while (c != nullptr) {
        if (c->courseID == courseID) {
            cout << "Student is already enrolled in " << courseID << ".\n";
            return;
        }
        c = c->next;
    }

    cin.ignore();
    cout << "Enter Course Name: ";
    getline(cin, courseName);

    Course* newCourse = new Course;
    newCourse->courseID = courseID;
    newCourse->courseName = courseName;
    newCourse->next = s->courses;
    s->courses = newCourse;

    cout << "Course added.\n";
}

void withdrawCourse() {
    int rollNo;
    string courseID;

    cout << "Enter Roll Number: ";
    cin >> rollNo;

    Student* s = findStudent(rollNo);
    if (s == nullptr) {
        cout << "Student not found.\n";
        return;
    }

    cout << "Enter Course ID to withdraw: ";
    cin >> courseID;

    Course* curr = s->courses;
    Course* prev = nullptr;

    while (curr != nullptr) {
        if (curr->courseID == courseID) {
            if (prev == nullptr) {
                s->courses = curr->next;
            } else {
                prev->next = curr->next;
            }
            delete curr;
            cout << "Course withdrawn.\n";
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    cout << "Course not found for this student.\n";
}

void displayStudent() {
    int rollNo;
    cout << "Enter Roll Number: ";
    cin >> rollNo;

    Student* s = findStudent(rollNo);
    if (s == nullptr) {
        cout << "Student not found.\n";
        return;
    }

    cout << "\n--- Student Details ---\n";
    cout << "Roll No   : " << s->rollNo << "\n";
    cout << "Name      : " << s->name << "\n";
    cout << "Department: " << s->department << "\n";
    cout << "Semester  : " << s->semester << "\n";
    cout << "Courses   :\n";

    Course* c = s->courses;
    if (c == nullptr) {
        cout << "  (None)\n";
    }
    while (c != nullptr) {
        cout << "  - " << c->courseID << ": " << c->courseName << "\n";
        c = c->next;
    }
}

void displayAllStudents() {
    cout << "\n--- Registered Students ---\n";
    bool anyFound = false;

    for (int i = 0; i < TABLE_SIZE; i++) {
        Student* curr = buckets[i];
        while (curr != nullptr) {
            anyFound = true;
            cout << "Roll No: " << curr->rollNo
                 << " | Name: " << curr->name
                 << " | Dept: " << curr->department
                 << " | Sem: " << curr->semester << "\n";
            curr = curr->next;
        }
    }

    if (!anyFound) {
        cout << "No students registered yet.\n";
    }
}

void removeStudent() {
    int rollNo;
    cout << "Enter Roll Number to remove: ";
    cin >> rollNo;

    int idx = getBucketIndex(rollNo);
    Student* curr = buckets[idx];
    Student* prev = nullptr;

    while (curr != nullptr) {
        if (curr->rollNo == rollNo) {
            if (prev == nullptr) {
                buckets[idx] = curr->next;
            } else {
                prev->next = curr->next;
            }

            // Free all course nodes before deleting the student
            Course* c = curr->courses;
            while (c != nullptr) {
                Course* nextCourse = c->next;
                delete c;
                c = nextCourse;
            }

            delete curr;
            cout << "Student record removed.\n";
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    cout << "Student not found.\n";
}

int main() {
    int choice;

    do {
        cout << "\n=== Student & Course System ===\n";
        cout << "1. Register Student\n";
        cout << "2. Update Student Info\n";
        cout << "3. Enroll in Course\n";
        cout << "4. Withdraw from Course\n";
        cout << "5. Search / View Student\n";
        cout << "6. View All Students\n";
        cout << "7. Remove Student\n";
        cout << "8. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerStudent(); break;
            case 2: updateStudent(); break;
            case 3: enrollCourse(); break;
            case 4: withdrawCourse(); break;
            case 5: displayStudent(); break;
            case 6: displayAllStudents(); break;
            case 7: removeStudent(); break;
            case 8: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 8);

    return 0;
}
