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
