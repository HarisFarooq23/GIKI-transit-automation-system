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
    ShuttleNode* shuttle = shuttleHead;
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
            case 3: {
                cout << "\n=== ROUTE STATISTICS ===\n";
                cout << "Processed Requests Counter: " << processedRequestCount << "/5\n";
                cout << "Routes are created after 5 processed requests.\n";
                break;
            }
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);
}