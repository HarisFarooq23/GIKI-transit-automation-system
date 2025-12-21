#include <iostream>
#include <fstream>
using namespace std;

const int MAX = 100;

// BST arrays
int regArr[MAX];
int passArr[MAX];
int leftChild[MAX];
int rightChild[MAX];

int nodeCount = 0;

// Create student file if missing
void createStudentFileIfMissing() {
    ifstream fin("students.txt");
    if (fin.good()) {
        fin.close();
        return;
    }

    fin.close();
    ofstream fout("students.txt");

    int startReg = 2023001;
    int startPass = 1111;

    for (int i = 0; i < 20; i++)
        fout << startReg + i << " " << startPass + i << endl;

    fout.close();
    cout << "students.txt created!\n";
}

// Create new node
int createNode(int r, int p) {
    regArr[nodeCount] = r;
    passArr[nodeCount] = p;
    leftChild[nodeCount] = -1;
    rightChild[nodeCount] = -1;
    return nodeCount++;
}

// Insert into BST
int insert(int root, int r, int p) {
    if (root == -1)
        return createNode(r, p);

    if (r < regArr[root])
        leftChild[root] = insert(leftChild[root], r, p);
    else if (r > regArr[root])
        rightChild[root] = insert(rightChild[root], r, p);

    return root;
}

// Search in BST
int search(int root, int r) {
    if (root == -1)
        return -1;

    if (r == regArr[root])
        return root;

    if (r < regArr[root])
        return search(leftChild[root], r);

    return search(rightChild[root], r);
}

// Load students from file
int loadFromFile() {
    createStudentFileIfMissing();

    ifstream fin("students.txt");
    int r, p;
    int root = -1;

    while (fin >> r >> p)
        root = insert(root, r, p);

    fin.close();
    return root;
}
// Delete student from BST
int deleteStudent(int root, int r) {
    if (root == -1)
        return -1;

    if (r < regArr[root])
        leftChild[root] = deleteStudent(leftChild[root], r);

    else if (r > regArr[root])
        rightChild[root] = deleteStudent(rightChild[root], r);

    else {
        // Case 1: No child
        if (leftChild[root] == -1 && rightChild[root] == -1)
            return -1;

        // Case 2: One child
        if (leftChild[root] == -1)
            return rightChild[root];

        if (rightChild[root] == -1)
            return leftChild[root];

        // Case 3: Two children
        int minNode = findMin(rightChild[root]);
        regArr[root] = regArr[minNode];
        passArr[root] = passArr[minNode];
        rightChild[root] = deleteStudent(rightChild[root], regArr[minNode]);
    }
    return root;
}

int main() {
    int root = loadFromFile();
    int choice;

    do {
        cout << "\n1. Login";
        cout << "\n2. Delete Student";
        cout << "\n3. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        if (choice == 1) {
            int reg, pass;
            cout << "\nEnter Registration Number: ";
            cin >> reg;

            int index = search(root, reg);

            if (index == -1) {
                cout << "Student NOT found.\n";
                continue;
            }

            cout << "Enter Password: ";
            cin >> pass;

            if (pass == passArr[index])
                cout << "Login Successful!\n";
            else
                cout << "Incorrect Password!\n";
        }

        else if (choice == 2) {
            int reg;
            cout << "\nEnter Registration Number to delete: ";
            cin >> reg;

            if (search(root, reg) == -1) {
                cout << "Student NOT found.\n";
            } else {
                root = deleteStudent(root, reg);
                cout << "Student deleted successfully.\n";
            }
        }

    } while (choice != 3);

    return 0;
}
