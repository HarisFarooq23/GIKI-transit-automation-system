# GIKI Shuttle Transit Automation System – Final Plan

## 1. Project Overview

The **GIKI Shuttle Transit Automation System** is a console-based C++ application designed to automate shuttle transportation within the GIKI campus. The system integrates **data structures and algorithms** (AVL Trees, Graphs, Queues, Linked Lists, Stacks, and Dijkstra’s Algorithm) to efficiently manage students, shuttles, ride requests, routing, and administrative operations.

The system supports **three main user roles**:

* **Students** – Request, view, and cancel rides
* **Admins** – Manage students, shuttles, and ride processing
* **Drivers** – View and complete optimized shuttle routes

---

## 2. System Architecture

### Core Components

* **Student Directory (AVL Tree)** – Fast student lookup and priority handling
* **Campus Map (Graph + Dijkstra)** – Shortest path calculations between stops
* **Ride Request Queue** – FIFO handling of shuttle requests
* **Processed Ride Queue** – Ride history and analytics
* **Cancelled Request Stack** – Undo and cancellation tracking
* **Shuttle Fleet (Linked List)** – Dynamic shuttle management
* **Active Routes (Linked List)** – Optimized shuttle routes for drivers

---

## 3. Data Structures Used

### 3.1 Student AVL Tree

**Purpose:** Efficient student management with priority handling

* Key: `studentID`
* Stores: name, user type, priority points
* Self-balancing for **O(log n)** operations

**Priority Mapping:**

| User Type | Priority Points |
| --------- | --------------- |
| Student   | 1               |
| Teacher   | 5               |
| Dean      | 10              |
| Rector    | 20              |

---

### 3.2 Shuttle Graph (Adjacency Matrix)

**Purpose:** Campus navigation and shortest path computation

* Nodes: 19 campus shuttle stops
* Edges: Weighted, undirected paths
* Algorithm: **Dijkstra’s Algorithm**

**Features:**

* Shortest distance calculation
* Route validation
* Graph visualization

---

### 3.3 Shuttle Fleet (Linked List)

**Purpose:** Manage available shuttles dynamically

Each shuttle stores:

* Shuttle ID
* Capacity
* Seats filled
* Current stop
* Status (Available / Busy / On Route)

---

### 3.4 Ride Request Queue

**Purpose:** Handle ride requests in arrival order

* FIFO queue
* Stores pickup, drop-off, student ID, request ID

---

### 3.5 Processed Ride Queue

**Purpose:** Maintain ride history

* Stores shuttle assignment
* Distance and estimated time
* Enables undo functionality

---

### 3.6 Cancelled Request Stack

**Purpose:** Track cancellations and undo actions

* LIFO stack
* Supports admin undo feature

---

### 3.7 Active Routes (Linked List)

**Purpose:** Driver route planning and execution

* Created after processing 5+ ride requests
* Uses nearest-neighbor heuristic
* Contains ordered list of route stops

---

## 4. Functional Modules

### 4.1 Student Portal

* Register student
* Request shuttle ride
* Cancel ride request
* View current and past bookings

---

### 4.2 Admin Portal

* View / remove students
* Add and view shuttles
* View and process ride requests
* Undo last processed ride
* View cancelled and processed rides
* Test Dijkstra’s algorithm
* View campus route map

---

### 4.3 Driver Portal

* View active optimized routes
* Complete routes
* View route statistics

---

## 5. Ride Processing Logic

1. Student submits ride request
2. Request enters pending queue
3. Admin processes requests (max 3 at a time)
4. Nearest available shuttle selected using Dijkstra
5. Shuttle assigned and moved to drop stop
6. Ride added to processed queue
7. After 5 processed rides → optimized route generated

---

## 6. Route Optimization Strategy

* Triggered after **5 processed requests**
* Collects last **8 processed rides**
* Extracts unique pickup & drop stops
* Applies **Nearest Neighbor heuristic**
* Generates an optimized active route
* Assigns route to an available shuttle

---

## 7. Distance & Time Estimation

* Distance unit: Graph edge weights
* Time estimation: **5 minutes per distance unit**

---

## 8. Error Handling & Validation

* Duplicate student ID prevention
* Invalid stop ID checks
* Same pickup/drop validation
* No-route detection
* Shuttle availability checks

---

## 9. System Initialization

* Loads 19 campus stops
* Builds weighted campus graph
* Initializes 3 shuttles
* Prepares all data structures

---

## 10. Strengths of the System

* Efficient data access using AVL Trees
* Realistic routing via Dijkstra’s Algorithm
* Modular and scalable design
* Clear separation of user roles
* Strong use of core CS concepts

---

## 11. Limitations

* Console-based UI only
* No persistent file storage
* Simple route optimization heuristic
* No real-time GPS tracking
* Limited concurrency handling

---

## 12. Future Enhancements

* File handling / database integration
* GUI or web-based interface
* Advanced route optimization (TSP / clustering)
* Real-time shuttle tracking
* Priority-based request scheduling
* Multi-shuttle collaborative routing

---

## 13. Conclusion

This project demonstrates a **complete shuttle automation system** using fundamental and advanced **Data Structures & Algorithms**. It successfully integrates AVL Trees, Graphs, Queues, Stacks, and Linked Lists into a real-world-inspired application suitable for academic evaluation and future expansion.
