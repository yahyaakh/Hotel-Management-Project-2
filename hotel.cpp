#include "hotel.h" // Include header file for the hotel classes
#include <iostream> // Input/output stream
#include <string> // String library
#include <unordered_map> // Hash table-based associative container
#include <vector> // Dynamic array
#include <fstream> // File stream
#include <algorithm> // For std::find
#include <limits> // Numeric limits
#include <stdexcept> // For std::runtime_error

using namespace std; // Standard namespace

// Function to check if a string is unique in a file
bool isStringUnique(const string& filename, const string& newString) {
    ifstream file(filename); // Open file stream
    if (!file.is_open()) { // Check if file is successfully opened
        throw runtime_error("Unable to open file: " + filename); // Throw an exception if file cannot be opened
    }

    string line;
    while (getline(file, line)) { // Read each line from the file
        if (line == newString) { // Check if the new string matches any existing string in the file
            file.close(); // Close the file stream
            return false; // Return false if string is not unique
        }
    }

    file.close(); // Close the file stream
    return true; // Return true if string is unique
}

// User class implementation
User::User(const string& uname, const string& pwd) : username(uname), password(pwd) {} // Constructor

bool User::authenticate(const string& uname, const string& pwd) const { // Method to authenticate user
    return (username == uname && password == pwd); // Return true if username and password match
}

string User::getUsername() const { // Method to get username
    return username; // Return username
}

// Customer class implementation
Customer::Customer(const string& uname, const string& pwd) : User(uname, pwd) {} // Constructor

bool Customer::authenticate(const string& uname, const string& pwd) const { // Method to authenticate customer
    return User::authenticate(uname, pwd); // Call base class method to authenticate user
}

// Manager class implementation
Manager::Manager(const string& uname, const string& pwd) : User(uname, pwd) {} // Constructor

bool Manager::authenticate(const string& uname, const string& pwd) const { // Method to authenticate manager
    return User::authenticate(uname, pwd); // Call base class method to authenticate user
}

// Hotel class implementation
Hotel::Hotel(string name, string address, string num) : hotelname(name), hoteladdress(address), hotelPhonenumber(num) {} // Constructor

void Hotel::hotelinfoprint() const { // Method to print hotel information
    cout << "Hotel name: " << hotelname << endl; // Print hotel name
    cout << "Hotel address: " << hoteladdress << endl; // Print hotel address
    cout << "Hotel contact phone number: " << hotelPhonenumber << endl; // Print hotel phone number
}

// RoomReservation class implementation
RoomReservation::RoomReservation(int stdRooms, int dlxRooms) : standardRooms(stdRooms), deluxeRooms(dlxRooms) {} // Constructor

bool RoomReservation::bookRoom(string roomType) { // Method to book a room
    if ((roomType == "standard" && standardRooms == 0) || (roomType == "deluxe" && deluxeRooms == 0)) { // Check if requested room type is available
        cout << "Sorry, no available rooms of this type." << endl; // Print message if room type is not available
        return false; // Return false indicating booking failure
    }

    string roomNumber;
    if (roomType == "standard") { // Generate room number for standard room
        roomNumber = "standard" + to_string(11 - standardRooms);
        standardRooms--;
    } else if (roomType == "deluxe") { // Generate room number for deluxe room
        roomNumber = "deluxe" + to_string(6 - deluxeRooms);
        deluxeRooms--;
    }

    bookedRooms.push_back(roomNumber); // Add booked room to the list of booked rooms

    try {
        if (isStringUnique("file.txt", roomNumber)) { // Check if room number is unique
            ofstream file("file.txt", ios::app); // Open file stream to append room number
            if (!file.is_open()) { // Check if file is successfully opened
                throw runtime_error("Unable to open file.txt"); // Throw an exception if file cannot be opened
            }

            file << roomNumber << endl; // Write room number to file
            file.close(); // Close the file stream

            // Write receipt to receipt file
            ofstream receipt("receipt.txt", ios::app);
            if (!receipt.is_open()) {
                throw runtime_error("Unable to open receipt.txt");
            }

            // Write room reservation details to receipt file
            receipt << "Room Reservation Receipt" << endl;
            receipt << "Room Type: " << roomType << endl;
            receipt << "Room Number: " << roomNumber << endl;
            receipt << "Total Amount: ";
            if (roomType == "standard") {
                receipt << "$30.00" << endl;
            } else if (roomType == "deluxe") {
                receipt << "$55.00" << endl;
            }
            receipt << "-------------------------" << endl;
            receipt.close(); // Close the file stream

            cout << "Room " << roomNumber << " booked successfully." << endl; // Print success message
            return true; // Return true indicating booking success
        } else {
            bookRoom(roomType); // If room number is not unique, try booking again
        }
    } catch (const runtime_error& e) { // Catch any runtime errors
        cerr << e.what() << endl; // Print error message
        return false; // Return false indicating booking failure
    }

    return false; // Return false indicating booking failure
}

// Method to check in a customer to a booked room
bool RoomReservation::checkInCustomer(const string& customerName, const string& roomNumber, const string& checkInDate, const string& checkOutDate) {
    try {
        ifstream file("file.txt"); // Open file containing booked room numbers
        if (!file.is_open()) {
            throw runtime_error("Unable to open file.txt"); // Throw an exception if file cannot be opened
        }

        string line;
        bool roomFound = false;

        while (getline(file, line)) { // Read each line from the file
            if (line == roomNumber) { // Check if booked room number matches the requested room number
                roomFound = true; // Set room found flag to true
                break; // Exit loop if room is found
            }
        }
        file.close(); // Close the file stream

        if (!roomFound) { // If room is not found in the booked rooms list
            cout << "Room not found or not booked yet." << endl; // Print error message
            return false; // Return false indicating check-in failure
        }

        checkedInGuests[roomNumber] = customerName; // Add customer to checked-in guests list
        string roomType = roomNumber.find("standard") != string::npos ? "standard" : "deluxe"; // Determine room type
        writeReceipt(customerName, roomNumber, checkInDate, checkOutDate, roomType); // Write receipt for the customer
        cout << "Customer checked in successfully." << endl; // Print success message

        ofstream receipt("checkedIn.txt", ios::app); // Open file stream to append checked-in rooms
        if (!receipt.is_open()) { // Check if file is successfully opened
            throw runtime_error("Unable to open checkedIn.txt"); // Throw an exception if file cannot be opened
        }
        receipt << roomNumber << endl; // Write room number to file
        return true; // Return true indicating check-in success
    } catch (const runtime_error& e) { // Catch any runtime errors
        cerr << e.what() << endl; // Print error message
        return false; // Return false indicating check-in failure
    }
}

// Method to print list of checked-in rooms
void RoomReservation::printCheckedInRooms() const {
    try {
        ifstream file("checkedIn.txt"); // Open file containing checked-in room numbers
        if (!file.is_open()) {
            throw runtime_error("Unable to open checkedIn.txt"); // Throw an exception if file cannot be opened
        }

        string line;
        while (getline(file, line)) { // Read each line from the file
            cout << line << endl; // Print room number
        }
        file.close(); // Close the file stream
    } catch (const runtime_error& e) { // Catch any runtime errors
        cerr << e.what() << endl; // Print error message
    }
}

// Method to write a receipt for a room reservation
void RoomReservation::writeReceipt(const string& customerName, const string& roomNumber, const string& checkInDate, const string& checkOutDate, const string& roomType) {
    try {
        ofstream receipt("receipt.txt", ios::app); // Open file stream to append receipt details
        if (!receipt.is_open()) { // Check if file is successfully opened
            throw runtime_error("Unable to open receipt.txt"); // Throw an exception if file cannot be opened
        }

        // Write receipt details to file
        receipt << "Room Reservation Receipt" << endl;
        receipt << "Customer Name: " << customerName << endl;
        receipt << "Room Type: " << roomType << endl;
        receipt << "Room Number: " << roomNumber << endl;
        receipt << "Check-In Date: " << checkInDate << endl;
        receipt << "Check-Out Date: " << checkOutDate << endl;
        receipt << "Total Amount: " << (roomType == "standard" ? "$30.00" : "$55.00") << endl;
        receipt << "-------------------------" << endl;
        receipt.close(); // Close the file stream
    } catch (const runtime_error& e) { // Catch any runtime errors
        cerr << e.what() << endl; // Print error message
    }
}


// Method to prompt the user to choose the room type (standard/deluxe)
string RoomReservation::promptRoomType() {
    string roomType;
    while (true) {
        cout << "Please choose the room type (standard/deluxe): ";
        cin >> roomType; // Read user input
        if (roomType == "standard" || roomType == "deluxe") { // Check if input is valid
            return roomType; // Return valid room type
        } else {
            cout << "Invalid input. Please write 'standard' or 'deluxe'." << endl; // Print error message for invalid input
        }
    }
}

// Function to view the list of reserved rooms
void viewrooms() {
    try {
        ifstream file("file.txt"); // Open file containing reserved room numbers
        if (!file.is_open()) {
            throw runtime_error("Unable to open file.txt"); // Throw an exception if file cannot be opened
        }

        string line;
        while (getline(file, line)) { // Read each line from the file
            cout << line << endl; // Print room number
        }
        file.close(); // Close the file stream
    } catch (const runtime_error& e) { // Catch any runtime errors
        cerr << e.what() << endl; // Print error message
    }
}

// Function to cancel a room reservation
void CancelRoomReservation(string roomname) {
    cout << "Which room do you want to make available to book? " << endl; // Prompt user for input
    cin.ignore(); // Ignore any remaining characters in input buffer
    getline(cin, roomname); // Read room name from user input

    try {
        ifstream inFile("file.txt"); // Open file containing reserved room numbers
        if (!inFile.is_open()) {
            throw runtime_error("Unable to open file.txt"); // Throw an exception if file cannot be opened
        }

        ofstream tempFile("temp.txt"); // Open temporary file to store updated reservation data
        if (!tempFile.is_open()) {
            throw runtime_error("Unable to open temp.txt"); // Throw an exception if temporary file cannot be opened
        }

        string line;
        while (getline(inFile, line)) { // Read each line from the file
            if (line != roomname) { // Check if current room is not the one to be canceled
                tempFile << line << endl; // Write current room to temporary file
            }
        }

        inFile.close(); // Close the file streams
        tempFile.close();

        if (remove("file.txt") != 0) { // Delete original file containing reservations
            throw runtime_error("Error deleting original file.txt"); // Throw an exception if file deletion fails
        }

        if (rename("temp.txt", "file.txt") != 0) { // Rename temporary file to original file name
            throw runtime_error("Error renaming temp.txt to file.txt"); // Throw an exception if file renaming fails
        }

        cout << "Room reservation canceled successfully." << endl; // Print success message
    } catch (const runtime_error& e) { // Catch any runtime errors
        cerr << e.what() << endl; // Print error message
    }
}

// Function to display the main menu for the hotel management system
void displayMenu() {
    cout << "Hotel Management System Menu" << endl;
    cout << "1. Book a room" << endl;
    cout << "2. Check room availability" << endl;
    cout << "3. Check-in" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

// Function to display the main menu for the hotel management system (manager)
void displayMenuManager() {
    cout << "Hotel Management Manager System Menu" << endl;
    cout << "1. Book a room" << endl;
    cout << "2. List of the reserved rooms" << endl;
    cout << "3. Cancel Room Reservation" << endl;
    cout << "4. Exit" << endl;
    cout << "5. View checked-in rooms list" << endl;
    cout << "Enter your choice: ";
}



void displayMenu(); // Declare displayMenu
void displayMenuManager(); // Declare displayMenuManager

void login() {
    Customer customer1("customer1", "password1");
    Customer customer2("customer2", "password2");
    Customer customer3("customer3", "password3");
    Manager manager("manager", "admin");

    vector<Customer> customers = {customer1, customer2, customer3};

    unordered_map<string, User*> userMap;
    for (auto& customer : customers) {
        userMap[customer.getUsername()] = &customer;
    }
    userMap["manager"] = &manager;

    RoomReservation* reservation = new RoomReservation();

    cout << "Login" << endl;
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    auto it = userMap.find(username);
    if (it != userMap.end()) {
        if (it->second->authenticate(username, password)) {
            if (dynamic_cast<Manager*>(it->second)) {
                cout << "Welcome, Manager " << username << "!" << endl;

                while (true) {
                    displayMenuManager();

                    int choice;
                    while (true) {
                        cin >> choice;
                        if (cin.fail()) {
                            cin.clear(); // Clear the error flags
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the incorrect input
                            cout << "Invalid input. Please enter a number: ";
                        } else {
                            break; // Break out of the loop if input is valid
                        }
                    }

                    try {
                        if (choice == 1) {
                            cout << "Option 1 selected: Book a room" << endl;
                            string roomType = reservation->promptRoomType();
                            reservation->bookRoom(roomType);
                        } else if (choice == 2) {
                            cout << "Option 2 selected: View list of reserved rooms" << endl;
                            cout << "This is the list of reserved rooms: " << endl;
                            viewrooms();
                        } else if (choice == 3) {
                            cout << "Option 3 selected: Cancel Room Reservation" << endl;
                            string roomname;
                            cout << "Enter room name to cancel reservation: ";
                            cin >> roomname;
                            CancelRoomReservation(roomname);
                            cout << endl;
                            cout << "Reserved rooms now: " << endl;
                            viewrooms();
                        } else if (choice == 4) {
                            cout << "Exiting program. Goodbye!" << endl;
                            delete reservation; // Free allocated memory
                            exit(0);
                        } else if (choice == 5) {
                            cout << "Option 5 selected: View checked-in rooms" << endl;
                            reservation->printCheckedInRooms();
                        } else {
                            cout << "Invalid choice. Please try again." << endl;
                        }
                    } catch (const runtime_error& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                }
            } else if (dynamic_cast<Customer*>(it->second)) {
                cout << "Welcome, Customer " << username << "!" << endl;

                while (true) {
                    displayMenu();

                    int choice;
                    while (true) {
                        cin >> choice;
                        if (cin.fail()) {
                            cin.clear(); // Clear the error flags
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the incorrect input
                            cout << "Invalid input. Please enter a number: ";
                        } else {
                            break; // Break out of the loop if input is valid
                        }
                    }

                    try {
                        if (choice == 1) {
                            cout << "Option 1 selected: Book a room" << endl;
                            string roomType = reservation->promptRoomType();
                            reservation->bookRoom(roomType);
                        } else if (choice == 2) {
                            cout << "Option 2 selected: Check room availability" << endl;
                            cout << "This is the list of reserved rooms: " << endl;
                            viewrooms();
                            cout << "If you want to book a room, choose a room that is not in this list." << endl;
                        } else if (choice == 3) {
                            cout << "Option 3 selected: Check-in" << endl;
                            string roomNumber, checkInDate, checkOutDate;
                            cout << "Enter room number: ";
                            cin >> roomNumber;
                            cout << "Enter check-in date (YYYY-MM-DD): ";
                            cin >> checkInDate;
                            cout << "Enter check-out date (YYYY-MM-DD): ";
                            cin >> checkOutDate;
                            reservation->checkInCustomer(username, roomNumber, checkInDate, checkOutDate);
                        } else if (choice == 4) {
                            cout << "Exiting program. Goodbye!" << endl;
                            delete reservation; // Free allocated memory
                            exit(0);
                        } else {
                            cout << "Invalid choice. Please try again." << endl;
                        }
                    } catch (const runtime_error& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                }
            }
        } else {
            cout << "Invalid username or password. Login failed." << endl;
        }
    } else {
        cout << "User not found. Login failed." << endl;
    }

    delete reservation; // Ensure memory is freed in case login fails
}
