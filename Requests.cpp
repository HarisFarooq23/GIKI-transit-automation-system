#include <iostream>
#include <fstream>
using namespace std;

/* ================= STUDENT BST ================= */

struct Student {
    int id;
    char name[30];
    Student* left;
    Student* right;
};

Student* createStudent(int id, char name[]) {
    Student* s = new Student;
    s->id = id;
    int i = 0;
    while (name[i] != '\0') {
        s->name[i] = name[i];
        i++;
    }
    s->name[i] = '\0';
    s->left = s->right = NULL;
    return s;
}

Student* insertStudent(Student* root, int id, char name[]) {
    if (root == NULL)
        return createStudent(id, name);

    if (id < root->id)
        root->left = insertStudent(root->left, id, name);
    else if (id > root->id)
        root->right = insertStudent(root->right, id, name);

    return root;
}

bool searchStudent(Student* root, int id) {
    if (root == NULL) return false;
    if (root->id == id) return true;
    if (id < root->id) return searchStudent(root->left, id);
    return searchStudent(root->right, id);
}

/* ================= RIDE REQUEST ================= */

struct Request {
    int studentID;
    char pickup[20];
    char drop[20];
};

/* ================= QUEUE ================= */

struct QNode {
    Request data;
    QNode* next;
};

struct Queue {
    QNode* front;
    QNode* rear;
};

void initQueue(Queue &q) {
    q.front = q.rear = NULL;
}

bool isQueueEmpty(Queue q) {
    return q.front == NULL;
}

void enqueue(Queue &q, Request r) {
    QNode* n = new QNode;
    n->data = r;
    n->next = NULL;

    if (q.rear == NULL) {
        q.front = q.rear = n;
        return;
    }
    q.rear->next = n;
    q.rear = n;
}

bool dequeue(Queue &q, Request &r) {
    if (isQueueEmpty(q))
        return false;

    QNode* temp = q.front;
    r = temp->data;
    q.front = q.front->next;

    if (q.front == NULL)
        q.rear = NULL;

    delete temp;
    return true;
}

void displayQueue(Queue q) {
    if (isQueueEmpty(q)) {
        cout << "No records found.\n";
        return;
    }
    while (q.front != NULL) {
        cout << "Student ID: " << q.front->data.studentID
             << " | " << q.front->data.pickup
             << " -> " << q.front->data.drop << endl;
        q.front = q.front->next;
    }
}

/* ================= STACK ================= */

struct SNode {
    Request data;
    SNode* next;
};

struct Stack {
    SNode* top;
};

void initStack(Stack &s) {
    s.top = NULL;
}

bool isStackEmpty(Stack s) {
    return s.top == NULL;
}

void push(Stack &s, Request r) {
    SNode* n = new SNode;
    n->data = r;
    n->next = s.top;
    s.top = n;
}

bool pop(Stack &s, Request &r) {
    if (isStackEmpty(s))
        return false;

    SNode* temp = s.top;
    r = temp->data;
    s.top = temp->next;
    delete temp;
    return true;
}

void displayStack(Stack s) {
    if (isStackEmpty(s)) {
        cout << "No records found.\n";
        return;
    }
    while (s.top != NULL) {
        cout << "Student ID: " << s.top->data.studentID
             << " | " << s.top->data.pickup
             << " -> " << s.top->data.drop << endl;
        s.top = s.top->next;
    }
}

/* ================= FILE HANDLING ================= */

void loadStudents(Student* &root) {
    ifstream file("students.txt");
    if (!file) {
        cout << "Student file not found!\n";
        return;
    }
    int id;
    char name[30];
    while (file >> id >> name) {
        root = insertStudent(root, id, name);
    }
    file.close();
}

/* ================= MAIN ================= */

int main() {
    Student* studentRoot = NULL;
    loadStudents(studentRoot);

    Queue pending, processed;
    Stack cancelled;

    initQueue(pending);
    initQueue(processed);
    initStack(cancelled);

    int choice;

    do {
        cout << "\n1 Add Request\n2 Process Request\n3 View Processed\n4 View Cancelled\n0 Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            Request r;
            cout << "Enter Student ID: ";
            cin >> r.studentID;

            if (!searchStudent(studentRoot, r.studentID)) {
                cout << "Invalid Student ID!\n";
                continue;
            }

            cout << "Pickup: ";
            cin >> r.pickup;
            cout << "Drop: ";
            cin >> r.drop;

            enqueue(pending, r);
            cout << "Request Added.\n";
        }

        else if (choice == 2) {
            Request r;
            if (!dequeue(pending, r)) {
                cout << "No pending requests.\n";
                continue;
            }

            int adminChoice;
            cout << "1 Process\n2 Cancel\nChoice: ";
            cin >> adminChoice;

            if (adminChoice == 1) {
                enqueue(processed, r);
                cout << "Request Processed.\n";
            }
            else if (adminChoice == 2) {
                push(cancelled, r);
                cout << "Request Cancelled.\n";
            }
            else {
                cout << "Invalid option. Request returned to pending.\n";
                enqueue(pending, r);
            }
        }

        else if (choice == 3) {
            cout << "\nProcessed Requests:\n";
            displayQueue(processed);
        }

        else if (choice == 4) {
            cout << "\nCancelled Requests:\n";
            displayStack(cancelled);
        }

    } while (choice != 0);

    return 0;
}

