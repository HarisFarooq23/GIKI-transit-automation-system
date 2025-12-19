#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
using namespace std;

/* ================= STUDENT DIRECTORY (AVL TREE) ================= */

struct Student {
    int studentID;
    int password;
    int height;
    Student* left;
    Student* right;

    Student(int id, int pass) {
        studentID = id;
        password = pass;
        height = 1;
        left = right = NULL;
    }
};

class StudentAVL {
private:
    Student* root;

    int height(Student* node) { return node ? node->height : 0; }

    int getBalance(Student* node) { return node ? height(node->left) - height(node->right) : 0; }

    Student* rightRotate(Student* y) {
        Student* x = y->left;
        Student* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Student* leftRotate(Student* x) {
        Student* y = x->right;
        Student* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    Student* insert(Student* node, int id, int pass) {
        if (!node) return new Student(id, pass);

        if (id < node->studentID)
            node->left = insert(node->left, id, pass);
        else if (id > node->studentID)
            node->right = insert(node->right, id, pass);
        else
            return node; // duplicate ID not allowed

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        // LL
        if (balance > 1 && id < node->left->studentID) return rightRotate(node);

        // RR
        if (balance < -1 && id > node->right->studentID) return leftRotate(node);

        // LR
        if (balance > 1 && id > node->left->studentID) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // RL
        if (balance < -1 && id < node->right->studentID) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Student* search(Student* node, int id) {
        if (!node || node->studentID == id) return node;
        if (id < node->studentID) return search(node->left, id);
        return search(node->right, id);
    }

    void inorderDisplay(Student* node) {
        if (!node) return;
        inorderDisplay(node->left);
        cout << "ID: " << node->studentID << ", Password: " << node->password << endl;
        inorderDisplay(node->right);
    }

public:
    StudentAVL() { root = NULL; }

    void addStudent(int id, int pass) { root = insert(root, id, pass); }

    int verifyStudent(int id, int pass) {
        Student* s = search(root, id);
        if (!s) return 0;      // new student
        if (s->password == pass) return 1; // correct
        return -1;             // wrong password
    }

    void loadFromFile(string filename) {
        ifstream fin(filename);
        if (!fin) {
            cout << "Error: Student data file not found!\n";
            return;
        }
        int id, pass;
        while (fin >> id >> pass) addStudent(id, pass);
        fin.close();
    }

    void addStudentToFile(string filename, int id, int pass) {
        addStudent(id, pass);
        ofstream fout(filename, ios::app);
        fout << id << " " << pass << endl;
        fout.close();
    }

    void displayAll() { inorderDisplay(root); }
};

/* ================= STOPS (DOUBLY LINKED LIST) ================= */
struct Stop {
    string stopID;
    string stopName;
    int usageCount;
    Stop* next;
    Stop* previous;

    Stop(string id, string name) {
        stopID = id;
        stopName = name;
        usageCount = 0;
        next = previous = NULL;
    }
};

class StopDLL {
private:
    Stop* head;
    Stop* tail;

    Stop* findStopByID(string id) {
        Stop* temp = head;
        while(temp) { if(temp->stopID == id) return temp; temp = temp->next; }
        return NULL;
    }

    Stop* findStopByName(string name) {
        Stop* temp = head;
        while(temp) { if(temp->stopName == name) return temp; temp = temp->next; }
        return NULL;
    }

public:
    StopDLL() { head = tail = NULL; }

    void addStop(string id, string name) {
        Stop* temp = new Stop(id, name);
        if(!head) { head = tail = temp; return; }
        temp->next = head;
        head->previous = temp;
        head = temp;
    }

    Stop* getStop(string input) {
        Stop* s = findStopByID(input);
        if(!s) s = findStopByName(input);
        return s;
    }

    void incrementUsage(string input) {
        Stop* s = getStop(input);
        if(s) s->usageCount++;
    }

    void displayAll() {
        Stop* temp = head;
        while(temp) {
            cout << "Stop ID: " << temp->stopID 
                 << ", Name: " << temp->stopName 
                 << ", Usage: " << temp->usageCount << endl;
            temp = temp->next;
        }
    }

    int calculateDistance(string s1, string s2) {
        Stop* start = getStop(s1);
        Stop* end = getStop(s2);
        if(!start || !end) return -1;

        int dist = 0;
        Stop* temp = start;
        while(temp) { if(temp == end) return dist; temp = temp->next; dist++; }

        dist = 0;
        temp = start;
        while(temp) { if(temp == end) return dist; temp = temp->previous; dist++; }

        return -1;
    }
};

/* ================= SHUTTLES (SINGLE LINKED LIST) ================= */
struct Shuttle {
    int shuttleID, capacity, currentPassengers;
    string status, currentStop;
    Shuttle* next;

    Shuttle(int id, int cap, string stop) {
        shuttleID = id;
        capacity = cap;
        currentPassengers = 0;
        status = "Available";
        currentStop = stop;
        next = NULL;
    }
};

class ShuttleLinkedList {
private:
    Shuttle* head;

public:
    ShuttleLinkedList() { head = NULL; }

    void addShuttle(int id, int cap, string stop) {
        Shuttle* temp = new Shuttle(id, cap, stop);
        temp->next = head;
        head = temp;
    }

    Shuttle* findAvailable(string pickupStop) {
        Shuttle* temp = head;
        while(temp) {
            if(temp->currentPassengers < temp->capacity) return temp;
            temp = temp->next;
        }
        return NULL;
    }

    void displayAllShuttles() {
        Shuttle* temp = head;
        while(temp) {
            cout << "ShuttleID: " << temp->shuttleID 
                 << ", Current Stop: " << temp->currentStop
                 << ", Passengers: " << temp->currentPassengers << "/" << temp->capacity
                 << ", Status: " << temp->status << endl;
            temp = temp->next;
        }
    }
};

/* ================= RIDE QUEUE ================= */
struct RideRequest {
    int studentID;
    string pickupStop;
    string dropStop;
    string timeRequested;
    RideRequest* next;

    RideRequest(int id, string pu, string dr, string t) {
        studentID = id;
        pickupStop = pu;
        dropStop = dr;
        timeRequested = t;
        next = NULL;
    }
};

class RideQueue {
private:
    RideRequest* front;
    RideRequest* rear;

public:
    RideQueue() { front = rear = NULL; }

    void enqueue(int id, string pu, string dr, string t) {
        RideRequest* r = new RideRequest(id, pu, dr, t);
        if(!rear) { front = rear = r; } 
        else { rear->next = r; rear = r; }
        cout << "Ride requested successfully!\n";
    }

    void displayQueue() {
        RideRequest* temp = front;
        cout << "\n--- Ride Queue ---\n";
        while(temp) {
            cout << "StudentID: " << temp->studentID 
                 << ", Pickup: " << temp->pickupStop 
                 << ", Drop: " << temp->dropStop 
                 << ", Time: " << temp->timeRequested << endl;
            temp = temp->next;
        }
    }
};

/* ================= MAIN SYSTEM ================= */
int main() {
    StudentAVL students;
    StopDLL stops;
    ShuttleLinkedList shuttles;
    RideQueue rideQueue;

    string filename = "studentdata.txt"; // ✅ Correct file name
    students.loadFromFile(filename);

    // Add predefined stops
    stops.addStop("S1","Library"); 
    stops.addStop("S2","FCSE");
    stops.addStop("S3","FBS"); 
    stops.addStop("S4","Girls Hostel");
    stops.addStop("S5","Medical Centre"); 
    stops.addStop("S6","Residential Area");

    // Add shuttles
    shuttles.addShuttle(1,4,"Library");
    shuttles.addShuttle(2,6,"FCSE");

    int choice;
    while(true) {
        cout << "\n--- Shuttle System Menu ---\n";
        cout << "1. Student Login/Sign Up\n";
        cout << "2. Display Students\n";
        cout << "3. Display Stops\n";
        cout << "4. Display Shuttles\n";
        cout << "5. Request Ride\n";
        cout << "6. View Ride Queue\n";
        cout << "7. Calculate Stop Distance\n";
        cout << "8. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(choice == 1) {
            int option;
            cout << "\n1. Login\n2. Sign Up\nChoose: ";
            cin >> option;

            int id, pass;
            cout << "Enter Student ID: "; cin >> id;
            cout << "Enter Password: "; cin >> pass;

            int status = students.verifyStudent(id, pass);

            if(option == 1) { // LOGIN
                if(status == 1) cout << "Login Successful!\n";
                else if(status == -1) cout << "Wrong password!\n";
                else cout << "Student ID not found. Please sign up.\n";
            }
            else if(option == 2) { // SIGN UP
                if(status == 0) {
                    students.addStudentToFile(filename, id, pass);
                    cout << "Sign up successful! You are now logged in.\n";
                } else {
                    cout << "Student already exists. Please login.\n";
                }
            } else {
                cout << "Invalid option!\n";
            }
        } 
        else if(choice == 2) students.displayAll();
        else if(choice == 3) stops.displayAll();
        else if(choice == 4) shuttles.displayAllShuttles();
        else if(choice == 5) {
            int sid; string pu, dr, time;
            cout << "Student ID: "; cin >> sid; cin.ignore();
            cout << "Pickup Stop (ID/Name): "; getline(cin, pu);
            cout << "Drop Stop (ID/Name): "; getline(cin, dr);
            cout << "Time: "; getline(cin, time);

            Stop* pickupStop = stops.getStop(pu);
            Stop* dropStop = stops.getStop(dr);

            if(!pickupStop || !dropStop){ 
                cout << "Invalid Stop(s)!\n"; continue; 
            }

            Shuttle* sh = shuttles.findAvailable(pu);
            if(!sh){ cout << "No available shuttle!\n"; continue; }

            rideQueue.enqueue(sid, pu, dr, time);
            stops.incrementUsage(pu); stops.incrementUsage(dr);

            int dist = stops.calculateDistance(pu, dr);
            cout << "Assigned Shuttle ID: " << sh->shuttleID 
                 << ", Distance: " << dist << " stop(s)\n";

            sh->currentPassengers++;
            if(sh->currentPassengers >= sh->capacity) sh->status = "Full";
        } 
        else if(choice == 6) rideQueue.displayQueue();
        else if(choice == 7){
            string s1, s2;
            cout << "Enter first Stop ID/Name: "; getline(cin, s1);
            cout << "Enter second Stop ID/Name: "; getline(cin, s2);
            int dist = stops.calculateDistance(s1, s2);
            if(dist == -1) cout << "Invalid Stop(s)!\n";
            else cout << "Distance: " << dist << " stop(s)\n";
        } 
        else if(choice == 8) { cout << "Exiting...\n"; break; }
        else cout << "Invalid choice!\n";
    }

    return 0;
}

