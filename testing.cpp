#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
using namespace std;

/* ================= STUDENT DIRECTORY (AVL TREE) ================= */
struct Student
{
    int studentID;
    int password;
    int height;
    Student* left;
    Student* right;

    Student(int id, int pass)
    {
        studentID = id;
        password = pass;
        height = 1;
        left = NULL;
        right = NULL;
    }
};

class StudentAVL
{
private:
    Student* root;

    int height(Student* node)
    {
        if (node == NULL)
        {
            return 0;
        }
        return node->height;
    }

    int getBalance(Student* node)
    {
        if (node == NULL)
        {
            return 0;
        }
        return height(node->left) - height(node->right);
    }

    Student* rightRotate(Student* y)
    {
        Student* x = y->left;
        Student* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Student* leftRotate(Student* x)
    {
        Student* y = x->right;
        Student* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    Student* insert(Student* node, int id, int pass)
    {
        if (node == NULL)
        {
            Student* newStudent = new Student(id, pass);
            return newStudent;
        }

        if (id < node->studentID)
        {
            node->left = insert(node->left, id, pass);
        }
        else if (id > node->studentID)
        {
            node->right = insert(node->right, id, pass);
        }
        else
        {
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && id < node->left->studentID)
        {
            return rightRotate(node);
        }

        if (balance < -1 && id > node->right->studentID)
        {
            return leftRotate(node);
        }

        if (balance > 1 && id > node->left->studentID)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && id < node->right->studentID)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Student* search(Student* node, int id)
    {
        if (node == NULL)
        {
            return NULL;
        }

        if (node->studentID == id)
        {
            return node;
        }

        if (id < node->studentID)
        {
            return search(node->left, id);
        }
        else
        {
            return search(node->right, id);
        }
    }

    void inorderDisplay(Student* node)
    {
        if (node == NULL)
        {
            return;
        }

        inorderDisplay(node->left);
        cout << "ID: " << node->studentID
             << ", Password: " << node->password
             << endl;
        inorderDisplay(node->right);
    }

public:
    StudentAVL()
    {
        root = NULL;
    }

    void addStudent(int id, int pass)
    {
        root = insert(root, id, pass);
    }

    int verifyStudent(int id, int pass)
    {
        Student* s = search(root, id);

        if (s == NULL)
        {
            return 0; // New student
        }

        if (s->password == pass)
        {
            return 1; // Correct password
        }

        return -1; // Wrong password
    }

    void loadFromFile(string filename)
    {
        ifstream fin(filename);
        if (!fin.is_open())
        {
            cout << "File not found: " << filename << endl;
            return;
        }

        int id;
        int pass;

        while (fin >> id >> pass)
        {
            addStudent(id, pass);
        }

        fin.close();
    }

    void addStudentToFile(string filename, int id, int pass)
    {
        addStudent(id, pass);

        ofstream fout(filename, ios::app);
        fout << id << " " << pass << endl;
        fout.close();
    }

    void displayAll()
    {
        inorderDisplay(root);
    }

    bool isPasswordValid(int pass)
    {
        string s = to_string(pass);
        for (size_t i = 0; i < s.length(); i++)
        {
            if (!isdigit(s[i]))
            {
                return false;
            }
        }
        return true;
    }
};

/* ================= INPUT VALIDATION FUNCTIONS ================= */
int getNumericInput(string prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Enter numbers only.\n";
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

/* ================= STOPS (DOUBLY LINKED LIST) ================= */
struct Stop
{
    string stopID;
    string stopName;
    int usageCount;
    Stop* next;
    Stop* previous;

    Stop(string id, string name)
    {
        stopID = id;
        stopName = name;
        usageCount = 0;
        next = NULL;
        previous = NULL;
    }
};

class StopDLL
{
private:
    Stop* head;
    Stop* tail;

    Stop* findStopByID(string id)
    {
        Stop* temp = head;
        while (temp)
        {
            if (temp->stopID == id)
            {
                return temp;
            }
            temp = temp->next;
        }
        return NULL;
    }

    Stop* findStopByName(string name)
    {
        Stop* temp = head;
        while (temp)
        {
            if (temp->stopName == name)
            {
                return temp;
            }
            temp = temp->next;
        }
        return NULL;
    }

public:
    StopDLL()
    {
        head = NULL;
        tail = NULL;
    }

    void addStop(string id, string name)
    {
        Stop* temp = new Stop(id, name);
        if (!head)
        {
            head = temp;
            tail = temp;
            return;
        }

        temp->next = head;
        head->previous = temp;
        head = temp;
    }

    Stop* getStop(string input)
    {
        Stop* s = findStopByID(input);
        if (!s)
        {
            s = findStopByName(input);
        }
        return s;
    }

    void incrementUsage(string input)
    {
        Stop* s = getStop(input);
        if (s)
        {
            s->usageCount++;
        }
    }

    void displayAll()
    {
        Stop* temp = head;
        while (temp)
        {
            cout << "Stop ID: " << temp->stopID
                 << ", Name: " << temp->stopName
                 << ", Usage: " << temp->usageCount
                 << endl;
            temp = temp->next;
        }
    }

    int calculateDistance(string s1, string s2)
    {
        Stop* start = getStop(s1);
        Stop* end = getStop(s2);

        if (!start || !end)
        {
            return -1;
        }

        int dist = 0;
        Stop* temp = start;
        while (temp)
        {
            if (temp == end)
            {
                return dist;
            }
            temp = temp->next;
            dist++;
        }

        dist = 0;
        temp = start;
        while (temp)
        {
            if (temp == end)
            {
                return dist;
            }
            temp = temp->previous;
            dist++;
        }

        return -1;
    }
};

/* ================= SHUTTLES (SINGLE LINKED LIST) ================= */
struct Shuttle
{
    int shuttleID;
    int capacity;
    int currentPassengers;
    string status;
    string currentStop;
    Shuttle* next;

    Shuttle(int id, int cap, string stop)
    {
        shuttleID = id;
        capacity = cap;
        currentPassengers = 0;
        status = "Available";
        currentStop = stop;
        next = NULL;
    }
};

class ShuttleLinkedList
{
private:
    Shuttle* head;

public:
    ShuttleLinkedList()
    {
        head = NULL;
    }

    void addShuttle(int id, int cap, string stop)
    {
        Shuttle* temp = new Shuttle(id, cap, stop);
        temp->next = head;
        head = temp;
    }

    Shuttle* findAvailable(string pickupStop)
    {
        Shuttle* temp = head;
        while (temp)
        {
            if (temp->currentPassengers < temp->capacity)
            {
                return temp;
            }
            temp = temp->next;
        }
        return NULL;
    }

    void displayAllShuttles()
    {
        Shuttle* temp = head;
        while (temp)
        {
            cout << "ShuttleID: " << temp->shuttleID
                 << ", Current Stop: " << temp->currentStop
                 << ", Passengers: " << temp->currentPassengers << "/" << temp->capacity
                 << ", Status: " << temp->status
                 << endl;
            temp = temp->next;
        }
    }
};

/* ================= RIDE QUEUE ================= */
struct RideRequest
{
    int studentID;
    string pickupStop;
    string dropStop;
    string timeRequested;
    RideRequest* next;

    RideRequest(int id, string pu, string dr, string t)
    {
        studentID = id;
        pickupStop = pu;
        dropStop = dr;
        timeRequested = t;
        next = NULL;
    }
};

class RideQueue
{
private:
    RideRequest* front;
    RideRequest* rear;

public:
    RideQueue()
    {
        front = NULL;
        rear = NULL;
    }

    void enqueue(int id, string pu, string dr, string t)
    {
        RideRequest* r = new RideRequest(id, pu, dr, t);

        if (!rear)
        {
            front = r;
            rear = r;
        }
        else
        {
            rear->next = r;
            rear = r;
        }

        cout << "Ride requested successfully!\n";
    }

    void displayQueue()
    {
        RideRequest* temp = front;
        cout << "\n--- Ride Queue ---\n";
        while (temp)
        {
            cout << "StudentID: " << temp->studentID
                 << ", Pickup: " << temp->pickupStop
                 << ", Drop: " << temp->dropStop
                 << ", Time: " << temp->timeRequested
                 << endl;
            temp = temp->next;
        }
    }
};

/* ================= MAIN SYSTEM ================= */
int main()
{
    StudentAVL students;
    StopDLL stops;
    ShuttleLinkedList shuttles;
    RideQueue rideQueue;

    string filename = "studentdata.txt";
    students.loadFromFile(filename);

    stops.addStop("S1", "Library");
    stops.addStop("S2", "FCSE");
    stops.addStop("S3", "FBS");
    stops.addStop("S4", "Girls Hostel");
    stops.addStop("S5", "Medical Centre");
    stops.addStop("S6", "Residential Area");

    shuttles.addShuttle(1, 4, "Library");
    shuttles.addShuttle(2, 6, "FCSE");

    int choice;
    while (true)
    {
        cout << "\n--- Shuttle System Menu ---\n";
        cout << "1. Login\n2. Sign Up\n3. Display Students\n4. Display Stops\n5. Display Shuttles\n6. Request Ride\n7. View Ride Queue\n8. Calculate Stop Distance\n9. Exit\n";
        choice = getNumericInput("Choice: ");

        if (choice == 1)
        {
            int id = getNumericInput("Enter Student ID: ");
            int pass = getNumericInput("Enter Password: ");

            int status = students.verifyStudent(id, pass);
            if (status == 1)
            {
                cout << "Login Successful!\n";
            }
            else if (status == -1)
            {
                cout << "Error: Wrong password!\n";
            }
            else
            {
                cout << "Error: Student ID not found. Please sign up.\n";
            }
        }
        else if (choice == 2)
        {
            int id = getNumericInput("Enter Student ID: ");
            int pass = getNumericInput("Enter Password (numbers only): ");

            if (!students.isPasswordValid(pass))
            {
                cout << "Error: Password must be numeric only.\n";
                continue;
            }

            int status = students.verifyStudent(id, pass);
            if (status == 0)
            {
                students.addStudentToFile(filename, id, pass);
                cout << "Sign Up Successful! You are now logged in.\n";
            }
            else
            {
                cout << "Error: Student already exists. Please login.\n";
            }
        }
        else if (choice == 3)
        {
            students.displayAll();
        }
        else if (choice == 4)
        {
            stops.displayAll();
        }
        else if (choice == 5)
        {
            shuttles.displayAllShuttles();
        }
        else if (choice == 6)
        {
            int sid = getNumericInput("Student ID: ");
            string pu, dr, time;
            cin.ignore();
            cout << "Pickup Stop (ID/Name): ";
            getline(cin, pu);
            cout << "Drop Stop (ID/Name): ";
            getline(cin, dr);
            cout << "Time: ";
            getline(cin, time);

            Stop* pickupStop = stops.getStop(pu);
            Stop* dropStop = stops.getStop(dr);

            if (!pickupStop || !dropStop)
            {
                cout << "Invalid Stop(s)!\n";
                continue;
            }

            Shuttle* sh = shuttles.findAvailable(pu);
            if (!sh)
            {
                cout << "No available shuttle!\n";
                continue;
            }

            rideQueue.enqueue(sid, pu, dr, time);
            stops.incrementUsage(pu);
            stops.incrementUsage(dr);

            int dist = stops.calculateDistance(pu, dr);
            cout << "Assigned Shuttle ID: " << sh->shuttleID
                 << ", Distance: " << dist << " stop(s)\n";

            sh->currentPassengers++;
            if (sh->currentPassengers >= sh->capacity)
            {
                sh->status = "Full";
            }
        }
        else if (choice == 7)
        {
            rideQueue.displayQueue();
        }
        else if (choice == 8)
        {
            string s1, s2;
            cin.ignore();
            cout << "Enter first Stop ID/Name: ";
            getline(cin, s1);
            cout << "Enter second Stop ID/Name: ";
            getline(cin, s2);

            int dist = stops.calculateDistance(s1, s2);
            if (dist == -1)
            {
                cout << "Invalid Stop(s)!\n";
            }
            else
            {
                cout << "Distance: " << dist << " stop(s)\n";
            }
        }
        else if (choice == 9)
        {
            cout << "Exiting...\n";
            break;
        }
        else
        {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}
