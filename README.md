# GIKI-transit-automation-system

This project simulates the entire shuttle request and routing system used inside GIKI, using only pure C++, NO STL, NO file handling, and classic data structures such as:

AVL Tree

BST

Queue

Stack

Linked List

It is designed as a full university-level DSA project, showcasing real-world logic implemented purely with academic data structure fundamentals.

📌 Project Overview

The system manages:

Shuttle Stops (stored in AVL Tree)

Student Directory (BST)

Ride Requests (Queue — FIFO)

Shuttle Assignment History (Stack)

Shuttle Fleet (Linked List)

Cancelled Requests (Stack)

Static Route Map (Doubly Linked List)

This creates a complete simulation of the GIKI shuttle workflow — from request placement to shuttle assignment, undo operations, route traversal, and logs.

🛠 System Features
✅ 1. Student Ride Request System

Each student can request a shuttle by providing:

Student ID

Pickup Stop

Drop Stop

Time Requested

Requests enter a Queue, ensuring first-come-first-served processing.

✅ 2. Shuttle Fleet Management (Linked List)

Each shuttle node stores:

Shuttle ID

Capacity

Status (Available / Busy)

Current Stop

Linked list allows efficient updates and traversal.

✅ 3. Shuttle Assignment Algorithm (Stack + AVL Tree)

Front of queue request is checked

AVL Tree used to search stops fast

Nearest available shuttle selected

Assignment stored in a Stack (allowing undo)

Undoing restores:

Shuttle capacity

Request back into queue

✅ 4. Cancellation Handling (Stack)

When a student cancels a request:

Request is stored on a Cancellation Stack

Admin can review or restore cancelled requests

✅ 5. Static Route Optimization (Doubly Linked List)

Since graphs are not allowed:

Every official GIKI shuttle route is stored as a DLL

System simulates distance by node index difference

Shuttles iterate through routes from pickup → drop-off

This gives realistic routing while staying within DSA constraints.

⭐ Advanced Features
🔥 Peak-Hour Priority Handling

Uses Stack as a priority structure for urgent requests (late students).

🔥 Shuttle Load Forecasting

Uses a BST to track frequency of pickup/drop usage.

🔥 Live Shuttle Dashboard

Print live shuttle details:

Current position

Seats filled

Next target

🔥 Completed Ride Logs

Uses LL + Stack to store history for admin review.

🧩 Data Structure Breakdown
Feature	Data Structure	Reason
Shuttle Stops	AVL Tree	Fast O(log n) search
Student Directory	BST	Simple ordered storage
Ride Requests	Queue	FIFO fairness
Shuttle Assignment History	Stack	Easy undo
Cancelled Requests	Stack	LIFO review
Shuttle List	Linked List	Easy update & traversal
Static Route Map	Doubly Linked List	Ordered traversal
Ride Logs	Linked List	Dynamic growth
📦 Stack Table

Below is the Stack Table describing how stack is used in the project for undo/redo and cancellation operations.

🧱 Stack Table

| Stack Name                              | Purpose                           | Push Trigger                      | Pop / Use Trigger                    | Data Stored                            |
| --------------------------------------- | --------------------------------- | --------------------------------- | ------------------------------------ | -------------------------------------- |
| **Assignment Stack**                    | Undo shuttle assignments          | Every time a shuttle is assigned  | Admin chooses "Undo"                 | Shuttle ID, Request Info, Old Capacity |
| **Cancellation Stack**                  | Store cancelled student requests  | Student cancels request           | Admin reviews/corrects cancellations | Request Info                           |
| **Ride History Stack** *(optional)*     | View last completed rides         | Ride completed                    | Admin queries history                | Student ID, Stop, Time                 |
| **Priority Request Stack** *(optional)* | Urgent requests during peak hours | Student marks request as priority | System assigns priority first        | Request Info                           |


🧪 Sample Workflow

Student places request → enters Queue

System checks shuttles (Linked List)

AVL tree finds stops

Nearest shuttle found

Route assigned bsaed on Priority

Assignment pushed to Stack

Shuttle departs using route DLL

Admin can increase/decrease number of operations stops and shuttles , or ban a certain user ( lets say theres a student violating certain policies )

Canceled requests go to secondary Stack


🔥  Priority-Based Stop Urgency Function

This system includes an intelligent Stop Priority Calculation Function that determines which stop needs a shuttle most urgently.

| User Type | Points        |
| --------- | ------------- |
| Student   | **1 point**   |
| Teacher   | **5 points**  |
| Dean      | **10 points** |
| Rector    | **20 points** |
