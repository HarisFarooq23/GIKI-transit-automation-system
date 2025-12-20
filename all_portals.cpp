#include <iostream>                     
#include <algorithm>                //Libraries
#include <fstream>                  //Used
#include <string>

using namespace std;

const string PROCESSED_FILE = "processed_rides.txt";   //Global
const string CANCELLED_FILE = "cancelled_rides.txt";   //Constants


//====GIKI SHUTTLE MANAGEMENT AND ROUTING SYSTEM====//

//------Student AVL tree------//
struct Student{
    int studentID,height,priorityPoints;
    string name;
    Student* left;
    Student* right;
    Student(int id,string n){
        studentID=id;
        name=n;
        height=1;
        left=right=NULL;
        if(id>2023999) priorityPoints =1 ;              //A priority system
        else if (id>19 && id <101) priorityPoints =5;   //that recongnizes
        else if (id>0 && id <20) priorityPoints = 10;   //the type of user
        else if ( id == 0 ) priorityPoints=20;          //from their registration id 
        else priorityPoints =1 ;                        //and assigns priority

    }
};

//====student AVL essentials!!======//

int getHeight (Student* node){         
    if (node==NULL) return 0;
    return node->height;
}

Student* leftRotate(Student* root) {
    Student* child = root->right;
    Student* childLeft = child->left;
    child->left = root;
    root->right = childLeft;
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    child->height = 1 + max(getHeight(child->left), getHeight(child->right));
    return child;
}

Student* rightRotate(Student* root) {
    Student* child = root->left;
    Student* childRight = child->right;
    child->right = root;
    root->left = childRight;
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    child->height = 1 + max(getHeight(child->left), getHeight(child->right));
    return child;
}

Student* insert(Student* root , int id , string name){
    if (root == NULL ){
        return new Student(id , name);
    }
    if(id<root->studentID){
        root->left=insert(root->left,id,name);
    }
    else if (id > root->studentID){
        root->right =insert(root->right,id,name);
    }
    root->height =1+max(getHeight(root->left),getHeight(root->right));
    int balance =getHeight (root->left)-getHeight(root->right);
    if(balance > 1 ){
        if (id < root->left->studentID){
            return rightRotate(root);

        } else {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
    }
    if (balance < -1) {
        if (id > root->right->studentID) {
            return leftRotate(root);
        } else {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
    }
    return root;
}
Student* minStudentNode(Student* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

Student* deleteStudent(Student* root, int id) {
    if (root == NULL) return NULL;
    if (id > root->studentID) {
        root->right = deleteStudent(root->right, id);
    } else if (id < root->studentID) {
        root->left = deleteStudent(root->left, id);
    } else {
        if (root->left == NULL && root->right == NULL) {
            delete root;
            return NULL;
        } else if (root->left != NULL && root->right == NULL) {
            Student* temp = root->left;
            delete root;
            return temp;
        } else if (root->left == NULL && root->right != NULL) {
            Student* temp = root->right;
            delete root;
            return temp;
        } else {
            Student* temp = minStudentNode(root->right);
            root->studentID = temp->studentID;
            root->name = temp->name;
            root->priorityPoints = temp->priorityPoints;
            root->right = deleteStudent(root->right, temp->studentID);
        }
    }
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int balance = getHeight(root->left) - getHeight(root->right);
    if (balance > 1) {
        if (getHeight(root->left->left) >= getHeight(root->left->right)) {
            return rightRotate(root);
        } else {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
    }
    if (balance < -1) {
        if (getHeight(root->right->right) >= getHeight(root->right->left)) {
            return leftRotate(root);
        } else {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
    }
    return root;
}

Student* searchStudent(Student* root, int id) {
    if (root == NULL || root->studentID == id) {
        return root;
    }
    if (id < root->studentID) {
        return searchStudent(root->left, id);
    }
    return searchStudent(root->right, id);
}

//========ShuttleStops + Dijkstras Implementation=====//

class StopsGraph {
private:                               //Will be making Graphs here
    int** adjacencyMatrix;             //Graph will be both
    int numStops;                      //Undirected and weighted
    string* stopNames;
    int* stopIDs;

public:
    StopsGraph(int stops) {
        numStops = stops;
        adjacencyMatrix = new int*[numStops];
        stopNames = new string[numStops];
        stopIDs = new int[numStops];

        for (int i = 0; i < numStops; i++) {
            adjacencyMatrix[i] = new int[numStops];
            stopIDs[i] = i + 1;
            for (int j = 0; j < numStops; j++) {
                adjacencyMatrix[i][j] = 0; 
            }
        }
    }

    ~StopsGraph() {
        for (int i = 0; i < numStops; i++) {
            delete[] adjacencyMatrix[i];
        }
        delete[] adjacencyMatrix;
        delete[] stopNames;
        delete[] stopIDs;
    }

    void setStopName(int id, string name) {
        if (id >= 1 && id <= numStops) {
            stopNames[id - 1] = name;
        }
    }

    string getStopName(int id) {
        if (id >= 1 && id <= numStops) {
            return stopNames[id - 1];
        }
        return "Unknown";
    }

    void addEdge(int u, int v, int weight) {
        if (u >= 1 && u <= numStops && v >= 1 && v <= numStops) {
            adjacencyMatrix[u-1][v-1] = weight;
            adjacencyMatrix[v-1][u-1] = weight; 
        }
    }

    void removeEdge(int u, int v) {
        if (u >= 1 && u <= numStops && v >= 1 && v <= numStops) {
            adjacencyMatrix[u-1][v-1] = 0;
            adjacencyMatrix[v-1][u-1] = 0;
        }
    }

    // Dijkstra's Algorithm Implementation
    int* findShortestPaths(int source) {
        int* distances = new int[numStops];
        bool* visited = new bool[numStops];
        int* previous = new int[numStops];

        // Initialize
        for (int i = 0; i < numStops; i++) {
            distances[i] = INT_MAX;
            visited[i] = false;
            previous[i] = -1;
        }
        distances[source - 1] = 0;

        // Dijkstra's algorithm
        for (int count = 0; count < numStops - 1; count++) {
            // Find minimum distance vertex
            int minDist = INT_MAX;
            int minIndex = -1;

            for (int v = 0; v < numStops; v++) {
                if (!visited[v] && distances[v] < minDist) {
                    minDist = distances[v];
                    minIndex = v;
                }
            }

            if (minIndex == -1) break; // No more reachable vertices
            visited[minIndex] = true;

            // Update distances of adjacent vertices
            for (int v = 0; v < numStops; v++) {
                if (!visited[v] && adjacencyMatrix[minIndex][v] != 0 &&
                    distances[minIndex] != INT_MAX &&
                    distances[minIndex] + adjacencyMatrix[minIndex][v] < distances[v]) {
                    distances[v] = distances[minIndex] + adjacencyMatrix[minIndex][v];
                    previous[v] = minIndex;
                }
            }
        }

        delete[] visited;
        delete[] previous;
        return distances;
    }

    int getShortestDistance(int source, int destination) {
        if (source < 1 || source > numStops || destination < 1 || destination > numStops) {
            return -1; // Invalid stops
        }

        int* distances = findShortestPaths(source);
        int result = distances[destination - 1];
        delete[] distances;
        return result;
    }

    void displayGraph() {
        cout << "\n=== SHUTTLE STOPS GRAPH ===\n";
        cout << "Stop ID | Stop Name\n";
        cout << "--------|----------\n";
        for (int i = 0; i < numStops; i++) {
            cout << "   " << (i+1) << "   | " << stopNames[i] << "\n";
        }

        cout << "\nAdjacency Matrix (Weights):\n";
        cout << "   ";
        for (int i = 0; i < numStops; i++) {
            cout << " " << (i+1) << " ";
        }
        cout << "\n";
        for (int i = 0; i < numStops; i++) {
            cout << " " << (i+1) << " ";
            for (int j = 0; j < numStops; j++) {
                if (adjacencyMatrix[i][j] == 0) {
                    cout << " . ";
                } else {
                    cout << " " << adjacencyMatrix[i][j] << " ";
                }
            }
            cout << "\n";
        }
    }

    int getNumStops() { return numStops; }
};


// ---------- SHUTTLE LINKED LIST ----------
struct Shuttle {
    int shuttleID, capacity, currentStop, seatsFilled;
    string status; // "Available", "Busy"
    Shuttle* next;
    Shuttle(int id, int cap) {
        shuttleID = id;
        capacity = cap;
        status = "Available";
        currentStop = 1; // Default to first stop
        seatsFilled = 0;
        next = NULL;
    }
};

// ---------- RIDE REQUEST QUEUE ----------
struct RideRequest {
    int studentID, pickupStop, dropStop, requestID;
    string timeRequested, priority;
    RideRequest* next;
    RideRequest(int sid, int pickup, int drop, string time, string pri, int rid) {
        studentID = sid;
        pickupStop = pickup;
        dropStop = drop;
        timeRequested = time;
        priority = pri;
        requestID = rid;
        next = NULL;
    }
};

// ---------- PROCESSED RIDES QUEUE ----------
struct ProcessedRide {
    int studentID, shuttleID, pickupStop, dropStop, requestID;
    int totalDistance, estimatedTime; // estimatedTime in minutes (5 mins per unit)
    string timeProcessed, status;
    ProcessedRide* next;
    ProcessedRide(int sid, int shid, int pickup, int drop, int rid, int dist, string time) {
        studentID = sid;
        shuttleID = shid;
        pickupStop = pickup;
        dropStop = drop;
        requestID = rid;
        totalDistance = dist;
        estimatedTime = dist * 5; // 5 minutes per unit distance
        timeProcessed = time;
        status = "Completed";
        next = NULL;
    }
};

// ---------- ACTIVE SHUTTLE ROUTES ----------
struct RouteStop {
    int stopID;
    string stopName;
    string action; // "Pickup" or "Dropoff"
    int studentID;
    RouteStop* next;
    RouteStop(int sid, string sname, string act, int stdid) {
        stopID = sid;
        stopName = sname;
        action = act;
        studentID = stdid;
        next = NULL;
    }
};

struct ActiveRoute {
    int shuttleID;
    int routeID;
    RouteStop* stops;
    int totalStops;
    string status; // "Planning", "Active", "Completed"
    ActiveRoute* next;
    ActiveRoute(int shid, int rid) {
        shuttleID = shid;
        routeID = rid;
        stops = NULL;
        totalStops = 0;
        status = "Planning";
        next = NULL;
    }
};

// ---------- CANCELLED REQUESTS STACK ----------
struct CancelledRequest {
    int studentID, pickupStop, dropStop, requestID;
    string timeRequested, priority;
    CancelledRequest* next;
    CancelledRequest(int sid, int pickup, int drop, string time, string pri, int rid) {
        studentID = sid;
        pickupStop = pickup;
        dropStop = drop;
        timeRequested = time;
        priority = pri;
        requestID = rid;
        next = NULL;
    }
};


// ==========================================
// GLOBAL VARIABLES SET
// ==========================================

Student* studentRoot = NULL;
StopsGraph* campusGraph = NULL;
Shuttle* shuttleHead = NULL;
RideRequest* requestFront = NULL;
RideRequest* requestRear = NULL;
ProcessedRide* processedFront = NULL;
ProcessedRide* processedRear = NULL;
CancelledRequest* cancelledTop = NULL;
ActiveRoute* activeRoutes = NULL;
int nextRequestID = 1;
int nextShuttleID = 1;
int nextRouteID = 1;
int processedRequestCount = 0; // Counter for route planning trigger


// ==========================================
// UTILITY FUNCTIONS
// ==========================================

void loadCampusData() {
    // Initialize 9 shuttle stops
    campusGraph = new StopsGraph(9);

    // Load stop names from data.txt
    campusGraph->setStopName(1, "auditorium");
    campusGraph->setStopName(2, "Academic block");
    campusGraph->setStopName(3, "Sports Complex");
    campusGraph->setStopName(4, "Admin Block");
    campusGraph->setStopName(5, "Faculty Club");
    campusGraph->setStopName(6, "medical centre");
    campusGraph->setStopName(7, "faculty residential area");
    campusGraph->setStopName(8, "Boys Hostel 1");
    campusGraph->setStopName(9, "New Girls Hostel");


    
    campusGraph->addEdge(1, 2, 1);  
    campusGraph->addEdge(2, 1, 1);
    campusGraph->addEdge(2, 4, 2);
    campusGraph->addEdge(4, 8, 2);
    campusGraph->addEdge(8, 3, 1);
    campusGraph->addEdge(4, 9, 3);
    campusGraph->addEdge(9, 5, 2);
    campusGraph->addEdge(4, 6, 3);
    campusGraph->addEdge(6, 7, 3);
    campusGraph->addEdge(9, 6, 1);         


}

void initializeShuttles(int numShuttles) {
    for (int i = 0; i < numShuttles; i++) {
        Shuttle* newShuttle = new Shuttle(nextShuttleID++, 25); // Default capacity 25

        if (shuttleHead == NULL) {
            shuttleHead = newShuttle;
        } else {
            Shuttle* temp = shuttleHead;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newShuttle;
        }
    }
}


// ==========================================
// STUDENT MANAGEMENT FUNCTIONS
// ==========================================

void registerStudent() {
    int id;
    string name, type;

    cout << "\n=== STUDENT REGISTRATION ===\n";
    cout << "Enter Student ID: ";
    cin >> id;

    if (searchStudent(studentRoot, id) != NULL) {
        cout << "Student with ID " << id << " already exists!\n";
        return;
    }

    cout << "Enter Student Name: ";
    cin>>name;


    studentRoot = insert(studentRoot, id, name);
    cout << "Student registered successfully!\n";
}

void removeStudent() {
    int id;
    cout << "\n=== REMOVE STUDENT ===\n";
    cout << "Enter Student ID to remove: ";
    cin >> id;

    Student* student = searchStudent(studentRoot, id);
    if (student == NULL) {
        cout << "Student not found!\n";
        return;
    }

    cout << "Removing User: " << student->name << "with the user id :" << student->studentID << ")\n";
    studentRoot = deleteStudent(studentRoot, id);
    cout << "User removed successfully!\n";
}

void displayStudents(Student* root) {
    if (root != NULL) {
        displayStudents(root->left);
        cout << "ID: " << root->studentID << " | Name: " << root->name
            <<" | Priority: " << root->priorityPoints << "\n";
        displayStudents(root->right);
    }
}

void viewStudents() {
    cout << "\n=== USER DIRECTORY ===\n";
    if (studentRoot == NULL) {
        cout << "No USERS registered.\n";
    } else {
        displayStudents(studentRoot);
    }
}


// ==========================================
// SHUTTLE MANAGEMENT FUNCTIONS
// ==========================================

void addShuttle() {
    int capacity;
    cout << "\n=== ADD SHUTTLE ===\n";
    cout << "Enter shuttle capacity: ";
    cin >> capacity;

    Shuttle* newShuttle = new Shuttle(nextShuttleID++, capacity);

    if (shuttleHead == NULL) {
        shuttleHead = newShuttle;
    } else {
        Shuttle* temp = shuttleHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newShuttle;
    }

    cout << "Shuttle added successfully! ID: " << newShuttle->shuttleID << "\n";
}

void viewShuttles() {
    cout << "\n=== SHUTTLE FLEET ===\n";
    if (shuttleHead == NULL) {
        cout << "No shuttles in fleet.\n";
        return;
    }

    Shuttle* temp = shuttleHead;
    cout << "ID | Capacity | Status | Current Stop | Seats Filled\n";
    cout << "---|----------|--------|--------------|-------------\n";

    while (temp != NULL) {
        cout << temp->shuttleID << "  |   " << temp->capacity << "     | " << temp->status
             << " |      " << temp->currentStop << "      |      " << temp->seatsFilled << "\n";
        temp = temp->next;
    }
}

Shuttle* findAvailableShuttle() {
    Shuttle* temp = shuttleHead;
    while (temp != NULL) {
        if (temp->status == "Available" && temp->seatsFilled < temp->capacity) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

Shuttle* findNearestShuttle(int pickupStop) {
    Shuttle* temp = shuttleHead;
    Shuttle* nearest = NULL;
    int minDistance = INT_MAX;

    while (temp != NULL) {
        if (temp->status == "Available" && temp->seatsFilled < temp->capacity) {
            int distance = campusGraph->getShortestDistance(temp->currentStop, pickupStop);
            if (distance != -1 && distance < minDistance) {
                minDistance = distance;
                nearest = temp;
            }
        }
        temp = temp->next;
    }

    return nearest;
}

// ==========================================
// RIDE REQUEST FUNCTIONS
// ==========================================

void requestRide() {
    int studentID, pickupStop, dropStop;
    string priority = "Normal";

    cout << "\n=== REQUEST SHUTTLE RIDE ===\n";
    cout << "Enter your Student ID: ";
    cin >> studentID;

    Student* student = searchStudent(studentRoot, studentID);
    if (student == NULL) {
        cout << "Student not found! Please register first.\n";
        return;
    }

    cout << "Welcome, " << student->name << " - UserI ID =" << student->studentID << "\n";

    campusGraph->displayGraph();
    cout << "\nEnter Pickup Stop ID: ";
    cin >> pickupStop;
    cout << "Enter Drop Stop ID: ";
    cin >> dropStop;

    if (pickupStop < 1 || pickupStop > campusGraph->getNumStops() ||
        dropStop < 1 || dropStop > campusGraph->getNumStops()) {
        cout << "Invalid stop IDs!\n";
        return;
    }

    if (pickupStop == dropStop) {
        cout << "Pickup and drop stops cannot be the same!\n";
        return;
    }

    // Check if route exists
    int distance = campusGraph->getShortestDistance(pickupStop, dropStop);
    if (distance == -1 || distance == INT_MAX) {
        cout << "No route available between these stops!\n";
        return;
    }

    cout << "\nRoute found! Distance: " << distance << " units\n";
    cout << "From: " << campusGraph->getStopName(pickupStop) << "\n";
    cout << "To: " << campusGraph->getStopName(dropStop) << "\n";

    // Create request
    RideRequest* newRequest = new RideRequest(studentID, pickupStop, dropStop, "Now", priority, nextRequestID++);

    if (requestFront == NULL) {
        requestFront = requestRear = newRequest;
    } else {
        requestRear->next = newRequest;
        requestRear = newRequest;
    }

    cout << "Ride request submitted successfully! Request ID: " << newRequest->requestID << "\n";
}

void createOptimizedRoute() {
    cout << "\n=== CREATING OPTIMIZED ROUTE ===\n";
    cout << "Analyzing recent ride patterns for route optimization...\n";

    // Find available shuttle
    Shuttle* availableShuttle = NULL;
    Shuttle* temp = shuttleHead;
    while (temp != NULL) {
        if (temp->status == "Available") {
            availableShuttle = temp;
            break;
        }
        temp = temp->next;
    }

    if (availableShuttle == NULL) {
        cout << "No available shuttles for route creation.\n";
        return;
    }

    // Collect recent stops from processed rides (last 8 rides)
    int recentStops[20]; // Max 20 stops
    int stopCount = 0;
    ProcessedRide* prtemp = processedRear;

    // Get last 8 processed rides (working backwards from queue rear)
    for (int i = 0; i < 8 && prtemp != NULL; i++) {
        // Add pickup and dropoff stops if not already in list
        bool pickupExists = false, dropoffExists = false;
        for (int j = 0; j < stopCount; j++) {
            if (recentStops[j] == prtemp->pickupStop) pickupExists = true;
            if (recentStops[j] == prtemp->dropStop) dropoffExists = true;
        }

        if (!pickupExists && stopCount < 20) {
            recentStops[stopCount++] = prtemp->pickupStop;
        }
        if (!dropoffExists && stopCount < 20) {
            recentStops[stopCount++] = prtemp->dropStop;
        }

        // Move to previous ride (queue doesn't have prev pointer, so we'll use a different approach)
        ProcessedRide* findPrev = processedFront;
        while (findPrev != NULL && findPrev->next != prtemp) {
            findPrev = findPrev->next;
        }
        prtemp = findPrev;
    }

    if (stopCount < 3) {
        cout << "Not enough diverse stops for route optimization. Need at least 3 different stops.\n";
        return;
    }

    // Create optimized route using simple nearest neighbor algorithm
    // Start from shuttle's current position
    int currentStop = availableShuttle->currentStop;
    bool visited[20] = {false};
    int routeStops[20];
    int routeCount = 0;

    // Add current position as starting point
    routeStops[routeCount++] = currentStop;

    // Find nearest neighbor for each step
    for (int i = 0; i < stopCount && routeCount < 20; i++) {
        int nearestStop = -1;
        int minDistance = INT_MAX;

        for (int j = 0; j < stopCount; j++) {
            if (!visited[j]) {
                int distance = campusGraph->getShortestDistance(currentStop, recentStops[j]);
                if (distance < minDistance && distance > 0) {
                    minDistance = distance;
                    nearestStop = j;
                }
            }
        }

        if (nearestStop != -1) {
            visited[nearestStop] = true;
            currentStop = recentStops[nearestStop];
            routeStops[routeCount++] = currentStop;
        }
    }

    // Create active route
    ActiveRoute* newRoute = new ActiveRoute(availableShuttle->shuttleID, nextRouteID++);
    newRoute->totalStops = routeCount;
    newRoute->status = "Active";

    // Add route stops
    RouteStop* lastStop = NULL;
    for (int i = 0; i < routeCount; i++) {
        RouteStop* newStop = new RouteStop(routeStops[i], campusGraph->getStopName(routeStops[i]), "Service", 0);
        if (newRoute->stops == NULL) {
            newRoute->stops = newStop;
        } else {
            lastStop->next = newStop;
        }
        lastStop = newStop;
    }

    // Add to active routes list
    if (activeRoutes == NULL) {
        activeRoutes = newRoute;
    } else {
        ActiveRoute* temp = activeRoutes;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newRoute;
    }

    // Update shuttle status
    availableShuttle->status = "On Route";

    cout << "? Optimized route created for Shuttle " << availableShuttle->shuttleID << "!\n";
    cout << "Route ID: " << newRoute->routeID << "\n";
    cout << "Stops: " << routeCount << "\n";
    cout << "Route: ";
    RouteStop* stop = newRoute->stops;
    while (stop != NULL) {
        cout << stop->stopName;
        if (stop->next != NULL) cout << " ? ";
        stop = stop->next;
    }
    cout << "\n\n";
}

void loadProcessedRides() {
    ifstream pf(PROCESSED_FILE);
    if (!pf.is_open()) return;

    int reqID, sid, shid, pickup, drop, dist, estTime;
    string status;

    while (pf >> reqID >> sid >> shid >> pickup >> drop >> dist >> estTime >> status) {
        ProcessedRide* pr = new ProcessedRide(sid, shid, pickup, drop, reqID, dist, ""); // empty time if not needed
        pr->estimatedTime = estTime;
        pr->status = status;
        pr->next = processedRear;
        processedRear = pr;
        if (processedFront == NULL) processedFront = processedRear;
    }
    pf.close();
}

void loadCancelledRides() {
    ifstream cf(CANCELLED_FILE);
    if (!cf.is_open()) return;

    int reqID, sid, pickup, drop;
    string time, pri;

    while (cf >> reqID >> sid >> pickup >> drop >> time >> pri) {
        CancelledRequest* cr = new CancelledRequest(sid, pickup, drop, time, pri, reqID);
        cr->next = cancelledTop;
        cancelledTop = cr;
    }
    cf.close();
}


void processRideRequests() {
    cout << "\n=== PROCESSING RIDE REQUESTS ===\n";

    if (requestFront == NULL) {
        cout << "No pending requests.\n";
        return;
    }

    int processedCount = 0;

    while (requestFront != NULL && processedCount < 3) {
        RideRequest* current = requestFront;

        // Find nearest shuttle
        Shuttle* shuttle = findNearestShuttle(current->pickupStop);
        // if (shuttle == NULL) {
        //     cout << "No available shuttles for Request ID " << current->requestID << "\n";
        //     requestFront = requestFront->next;
        //     processedCount++;
        //     continue;
        // }

        int shuttleToPickup = campusGraph->getShortestDistance(shuttle->currentStop, current->pickupStop);
        int pickupToDrop = campusGraph->getShortestDistance(current->pickupStop, current->dropStop);
        int totalDistance = shuttleToPickup + pickupToDrop;

        // Update shuttle
        shuttle->status = "Busy";
        shuttle->seatsFilled++;

        // Create processed ride (stack push)
        ProcessedRide* newProcessedRide = new ProcessedRide(
            current->studentID, shuttle->shuttleID, current->pickupStop,
            current->dropStop, current->requestID, totalDistance, "Completed"
        );

        newProcessedRide->next = processedRear; // push to stack
        processedRear = newProcessedRide;
        if (processedFront == NULL) processedFront = processedRear;

        // Append to file
        ofstream pf(PROCESSED_FILE, ios::app);
        if (pf.is_open()) {
            pf << newProcessedRide->requestID << " "
               << newProcessedRide->studentID << " "
               << newProcessedRide->shuttleID << " "
               << newProcessedRide->pickupStop << " "
               << newProcessedRide->dropStop << " "
               << newProcessedRide->totalDistance << " "
               << newProcessedRide->estimatedTime << " "
               << newProcessedRide->status << "\n";
            pf.close();
        }

        // Move shuttle to drop
        shuttle->currentStop = current->dropStop;

        // Remove ride request from queue
        requestFront = current->next;
        delete current;

        processedCount++;
        processedRequestCount++;
    }

    cout << "\nProcessed " << processedCount << " ride(s).\n";

    if (processedRequestCount >= 5) {
        createOptimizedRoute();
        processedRequestCount = 0;
    }
}


// ==========================================
// ROUTE OPTIMIZATION FUNCTIONS
// ==========================================


void cancelRide() {
    int requestID;
    cout << "\n=== CANCEL RIDE REQUEST ===\n";
    cout << "Enter Request ID: ";
    cin >> requestID;

    RideRequest* current = requestFront;
    RideRequest* prev = NULL;

    while (current != NULL) {
        if (current->requestID == requestID) {
            CancelledRequest* cancelled = new CancelledRequest(
                current->studentID, current->pickupStop, current->dropStop,
                current->timeRequested, "User Cancelled", current->requestID
            );

            // Push to stack
            cancelled->next = cancelledTop;
            cancelledTop = cancelled;

            // Save to file
            ofstream cf(CANCELLED_FILE, ios::app);
            if (cf.is_open()) {
                cf << cancelled->requestID << " "
                   << cancelled->studentID << " "
                   << cancelled->pickupStop << " "
                   << cancelled->dropStop << " "
                   << cancelled->timeRequested << " "
                   << cancelled->priority << "\n";
                cf.close();
            }

            // Remove from request queue
            if (prev == NULL) requestFront = current->next;
            else prev->next = current->next;
            delete current;

            cout << "Ride cancelled successfully.\n";
            return;
        }
        prev = current;
        current = current->next;
    }

    cout << "Request ID not found!\n";
}


void viewCurrentBooking() {
    int studentID;
    cout << "\n=== VIEW CURRENT BOOKING ===\n";
    cout << "Enter your Student ID: ";
    cin >> studentID;

    Student* student = searchStudent(studentRoot, studentID);
    if (student == NULL) {
        cout << "Student not found! Please register first.\n";
        return;
    }

    cout << "Welcome, " << student->name << "!\n\n";

    // Check pending requests
    RideRequest* temp = requestFront;
    bool foundPending = false;
    cout << "=== PENDING REQUESTS ===\n";
    while (temp != NULL) {
        if (temp->studentID == studentID) {
            cout << "Request ID: " << temp->requestID << "\n";
            cout << "From: " << campusGraph->getStopName(temp->pickupStop) << "\n";
            cout << "To: " << campusGraph->getStopName(temp->dropStop) << "\n";
            cout << "Status: Waiting for shuttle assignment\n\n";
            foundPending = true;
        }
        temp = temp->next;
    }

    if (!foundPending) {
        cout << "No pending requests.\n\n";
    }

    // Check processed rides (recently completed)
    ProcessedRide* ptemp = processedFront;
    bool foundProcessed = false;
    cout << "=== RECENT RIDES ===\n";
    while (ptemp != NULL) {
        if (ptemp->studentID == studentID) {
            cout << "Request ID: " << ptemp->requestID << "\n";
            cout << "Shuttle ID: " << ptemp->shuttleID << "\n";
            cout << "From: " << campusGraph->getStopName(ptemp->pickupStop) << "\n";
            cout << "To: " << campusGraph->getStopName(ptemp->dropStop) << "\n";
            cout << "Total Distance: " << ptemp->totalDistance << " units\n";
            cout << "Estimated Time: " << ptemp->estimatedTime << " minutes\n";
            cout << "Status: " << ptemp->status << "\n\n";
            foundProcessed = true;
        }
        ptemp = ptemp->next;
    }

    if (!foundProcessed) {
        cout << "No recent rides.\n\n";
    }

}

void viewPendingRequests() {
    cout << "\n=== PENDING RIDE REQUESTS ===\n";
    if (requestFront == NULL) {
        cout << "No processed requests.\n";
        return;
    }

    RideRequest* temp = requestFront;
    cout << "Request ID | Student ID | From | To | Time\n";
    cout << "-----------|------------|------|----|-----\n";

    while (temp != NULL) {
        cout << "    " << temp->requestID << "     |     " << temp->studentID << "    | "
             << campusGraph->getStopName(temp->pickupStop) << " | "
             << campusGraph->getStopName(temp->dropStop) << " | " << temp->timeRequested << "\n";
        temp = temp->next;
    }
}

// ==========================================
// SHUTTLE MANAGER FUNCTIONS
// ==========================================

void undoLastAssignment() {
    cout << "\n=== UNDO LAST PROCESSED RIDE ===\n";

    if (processedRear == NULL) {
        cout << "No processed rides to undo.\n";
        return;
    }

    // Get the last processed ride
    ProcessedRide* temp = processedRear;

    // Find shuttle and restore capacity
    Shuttle* shuttle = shuttleHead;
    while (shuttle != NULL) {
        if (shuttle->shuttleID == temp->shuttleID) {
            // Restore shuttle capacity (reduce seats filled)
            if (shuttle->seatsFilled > 0) {
                shuttle->seatsFilled--;
            }
            if (shuttle->seatsFilled <= 0) {
                shuttle->status = "Available";
                shuttle->seatsFilled = 0;
            }
            cout << "Processed ride undone. Shuttle " << shuttle->shuttleID << " status restored.\n";
            break;
        }
        shuttle = shuttle->next;
    }

    // Remove from processed rides queue
    if (processedFront == processedRear) {
        processedFront = processedRear = NULL;
    } else {
        ProcessedRide* current = processedFront;
        while (current->next != processedRear) {
            current = current->next;
        }
        current->next = NULL;
        processedRear = current;
    }

    // Add to cancelled requests
    CancelledRequest* cancelled = new CancelledRequest(
        temp->studentID, temp->pickupStop, temp->dropStop,
        "Cancelled by Shuttle Manager", "Normal", temp->requestID
    );
    cancelled->next = cancelledTop;
    cancelledTop = cancelled;

    delete temp;
}

void viewProcessedRides() {
    cout << "\n=== PROCESSED RIDES HISTORY ===\n";
    if (processedFront == NULL) {
        cout << "No processed rides.\n";
        return;
    }

    ProcessedRide* temp = processedFront;
    cout << "Request ID | Student ID | Shuttle ID | From | To | Distance | Est. Time | Status\n";
    cout << "-----------|------------|------------|------|----|----------|-----------|--------\n";

    while (temp != NULL) {
        cout << "    " << temp->requestID << "     |     " << temp->studentID << "    |     "
             << temp->shuttleID << "     | " << campusGraph->getStopName(temp->pickupStop) << " | "
             << campusGraph->getStopName(temp->dropStop) << " |    " << temp->totalDistance
             << "    |    " << temp->estimatedTime << " mins | " << temp->status << "\n";
        temp = temp->next;
    }
}

void viewCancelledRequests() {
    cout << "\n=== CANCELLED REQUESTS ===\n";
    if (cancelledTop == NULL) {
        cout << "No cancelled requests.\n";
        return;
    }

    CancelledRequest* temp = cancelledTop;
    cout << "Request ID | Student ID | From | To | Time | Reason\n";
    cout << "-----------|------------|------|----|------|--------\n";

    while (temp != NULL) {
        cout << "    " << temp->requestID << "     |     " << temp->studentID << "    | "
             << campusGraph->getStopName(temp->pickupStop) << " | "
             << campusGraph->getStopName(temp->dropStop) << " | " << temp->timeRequested << " | "
             << (temp->priority == "Cancelled by Shuttle Manager" ? "Shuttle Manager Action" : "User Cancelled") << "\n";
        temp = temp->next;
    }
}

void testDijkstra() {
    cout << "\n=== DIJKSTRA'S ALGORITHM TEST ===\n";

    int source;
    cout << "Enter source stop ID (1-19): ";
    cin >> source;

    if (source < 1 || source > campusGraph->getNumStops()) {
        cout << "Invalid stop ID!\n";
        return;
    }

    cout << "\nShortest distances from " << campusGraph->getStopName(source) << ":\n";
    cout << "To | Distance\n";
    cout << "---|---------\n";

    int* distances = campusGraph->findShortestPaths(source);

    for (int i = 0; i < campusGraph->getNumStops(); i++) {
        int dist = distances[i];
        if (dist == INT_MAX) {
            cout << campusGraph->getStopName(i+1) << " | Unreachable\n";
        } else {
            cout << campusGraph->getStopName(i+1) << " | " << dist << " units\n";
        }
    }

    delete[] distances;
}
void viewHistory() {
    cout << "\n=== VIEW HISTORY ===\n";

    // Processed rides (stack)
    cout << "\n--- PROCESSED RIDES ---\n";
    ProcessedRide* pr = processedRear;
    if (!pr) cout << "No processed rides.\n";
    while (pr != NULL) {
        cout << "RequestID: " << pr->requestID
             << " | StudentID: " << pr->studentID
             << " | ShuttleID: " << pr->shuttleID
             << " | From: " << campusGraph->getStopName(pr->pickupStop)
             << " | To: " << campusGraph->getStopName(pr->dropStop)
             << " | Distance: " << pr->totalDistance
             << " | Status: " << pr->status << "\n";
        pr = pr->next;
    }

    // Cancelled rides (stack)
    cout << "\n--- CANCELLED RIDES ---\n";
    CancelledRequest* cr = cancelledTop;
    if (!cr) cout << "No cancelled rides.\n";
    while (cr != NULL) {
        cout << "RequestID: " << cr->requestID
             << " | StudentID: " << cr->studentID
             << " | From: " << campusGraph->getStopName(cr->pickupStop)
             << " | To: " << campusGraph->getStopName(cr->dropStop)
             << " | Time: " << cr->timeRequested
             << " | Reason: " << cr->priority << "\n";
        cr = cr->next;
    }
}

// ==========================================
// DRIVER PORTAL FUNCTIONS
// ==========================================

// ==========================================
// DRIVER PORTAL FUNCTIONS
// ==========================================

void displayActiveRoutes() {
    cout << "\n=== ACTIVE SHUTTLE ROUTES ===\n";

    if (activeRoutes == NULL) {
        cout << "No active routes currently.\n";
        cout << "Routes are created after processing 5 or more ride requests.\n";
        return;
    }

    ActiveRoute* route = activeRoutes;
    int routeCount = 0;

    while (route != NULL) {
        if (route->status == "Active") {
            routeCount++;
            cout << "\nRoute " << route->routeID << " - Shuttle " << route->shuttleID << "\n";
            cout << "Status: " << route->status << "\n";
            cout << "Total Stops: " << route->totalStops << "\n";
            cout << "Route Path:\n";

            RouteStop* stop = route->stops;
            int stopNum = 1;
            while (stop != NULL) {
                cout << "  " << stopNum << ". " << stop->stopName << " (" << stop->stopID << ")";
                if (stop->studentID != 0) {
                    cout << " - Student " << stop->studentID;
                }
                cout << "\n";
                stop = stop->next;
                stopNum++;
            }

            // Calculate total route distance
            int totalDistance = 0;
            if (route->stops != NULL && route->stops->next != NULL) {
                RouteStop* current = route->stops;
                RouteStop* next = current->next;
                while (next != NULL) {
                    int dist = campusGraph->getShortestDistance(current->stopID, next->stopID);
                    if (dist != -1) totalDistance += dist;
                    current = next;
                    next = next->next;
                }
            }

            cout << "Total Route Distance: " << totalDistance << " units\n";
            cout << "Estimated Completion Time: " << (totalDistance * 5) << " minutes\n";
            cout << string(50, '-') << "\n";
        }
        route = route->next;
    }

    if (routeCount == 0) {
        cout << "No active routes currently.\n";
        cout << "Routes are created after processing 5 or more ride requests.\n";
    } else {
        cout << "\nTotal Active Routes: " << routeCount << "\n";
    }
}

void completeRoute() {
    cout << "\n=== COMPLETE ROUTE ===\n";

    if (activeRoutes == NULL) {
        cout << "No active routes to complete.\n";
        return;
    }

    cout << "Active Routes:\n";
    ActiveRoute* route = activeRoutes;
    int count = 1;
    while (route != NULL) {
        if (route->status == "Active") {
            cout << count << ". Route " << route->routeID << " - Shuttle " << route->shuttleID << "\n";
        }
        route = route->next;
        count++;
    }

    cout << "Enter route number to complete (0 to cancel): ";
    int choice;
    cin >> choice;

    if (choice == 0) return;

    // Find the route
    route = activeRoutes;
    ActiveRoute* prev = NULL;
    count = 1;

    while (route != NULL && count != choice) {
        if (route->status == "Active") count++;
        if (count <= choice) {
            prev = route;
            route = route->next;
        }
    }

    if (route == NULL || route->status != "Active") {
        cout << "Invalid route selection.\n";
        return;
    }

    // Mark route as completed
    route->status = "Completed";

    // Free shuttle
    Shuttle* shuttle = shuttleHead;
    while (shuttle != NULL) {
        if (shuttle->shuttleID == route->shuttleID) {
            shuttle->status = "Available";
            shuttle->seatsFilled = 0;
            // Move shuttle to last stop on route
            if (route->stops != NULL) {
                RouteStop* lastStop = route->stops;
                while (lastStop->next != NULL) {
                    lastStop = lastStop->next;
                }
                shuttle->currentStop = lastStop->stopID;
            }
            break;
        }
        shuttle = shuttle->next;
    }

    cout << "Route " << route->routeID << " completed!\n";
    cout << "Shuttle " << route->shuttleID << " is now available.\n";
}

void driverMenu() {
    int choice;
    do {
        cout << "\n=== DRIVER PORTAL ===\n";
        cout << "1. View Active Routes\n";
        cout << "2. Complete Route\n";
        cout << "3. View Route Statistics\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: displayActiveRoutes(); break;
            case 2: completeRoute(); break;
            case 3:
                cout << "\n=== ROUTE STATISTICS ===\n";
                cout << "Processed Requests Counter: " << processedRequestCount << "/5\n";
                cout << "Routes are created after 5 processed requests.\n";
                break;
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);
}


// ==========================================
// MAIN MENU SYSTEM
// ==========================================

void studentMenu() {
    int choice;
    do {
        cout << "\n=== STUDENT MENU ===\n";
        cout << "1. Register as Student\n";
        cout << "2. Request Shuttle Ride\n";
        cout << "3. Cancel Ride Request\n";
        cout << "4. View Current Booking\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerStudent(); break;
            case 2: requestRide(); break;
            case 3: cancelRide(); break;
            case 4: viewCurrentBooking(); break;
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);
}

void ShuttleManagerMenu() {
    int choice;
    do {
        cout << "\n=== SHUTTLE MANAGER MENU ===\n";
        cout << "1. View All Students\n";
        cout << "2. Remove Student\n";
        cout << "3. Add Shuttle\n";
        cout << "4. View Shuttle Fleet\n";
        cout << "5. View Pending Requests\n";
        cout << "6. Process Ride Requests\n";
        cout << "7. Undo Last Assignment\n";
        cout << "8. View Cancelled Requests\n";
        cout << "9. View Processed Rides\n";
        cout << "10. View Route Map\n";
        cout << "11. Test Dijkstra's Algorithm\n";
        cout << "12. View History\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewStudents(); break;
            case 2: removeStudent(); break;
            case 3: addShuttle(); break;
            case 4: viewShuttles(); break;
            case 5: viewPendingRequests(); break;
            case 6: processRideRequests(); break;
            case 7: undoLastAssignment(); break;
            case 8: viewCancelledRequests(); break;
            case 9: viewProcessedRides(); break;
            case 10: campusGraph->displayGraph(); break;
            case 11: testDijkstra(); break;
            case 12: viewHistory(); break;
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);
}

int main() {
    cout << "=========================================\n";
    cout << "  GIKI SHUTTLE TRANSIT AUTOMATION SYSTEM\n";
    cout << "=========================================\n";

    // Initialize system
    loadCampusData();
    initializeShuttles(3); // Load 3 shuttles as per data.txt
     loadProcessedRides();
    loadCancelledRides();

    cout << "System initialized successfully!\n";
    cout << "Loaded " << campusGraph->getNumStops() << " shuttle stops.\n";
    cout << "Initialized 3 shuttles.\n";

    int choice;
    do {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Student Portal\n";
        cout << "2. Shuttle Manager Portal\n";
        cout << "3. Driver Portal\n";
        cout << "4. View System Status\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: studentMenu(); break;
            case 2: ShuttleManagerMenu(); break;
            case 3: driverMenu(); break;            
            case 4:
                cout << "\n=== SYSTEM STATUS ===\n";
                cout << "Shuttle Stops: " << campusGraph->getNumStops() << "\n";
                cout << "Registered Students: " << (studentRoot != NULL ? "Available" : "None") << "\n";
                cout << "Active Shuttles: " << (shuttleHead != NULL ? "Available" : "None") << "\n";
                cout << "Pending Requests: " << (requestFront != NULL ? "Yes" : "None") << "\n";
                break;
            case 0:
                cout << "Thank you for using GIKI Shuttle System!\n";
                break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    // Cleanup (in a real system, you'd want proper memory cleanup)
    // For now, the OS will handle cleanup when program exits

    return 0;
}
