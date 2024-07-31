#ifndef HOTEL_H
#define HOTEL_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

bool isStringUnique(const string& filename, const string& newString);

// Define a class for User
class User {
protected:
    string username;
    string password;

public:
    User(const string& uname, const string& pwd);
    virtual bool authenticate(const string& uname, const string& pwd) const;
    string getUsername() const;
};

// Define a subclass for Customer, inheriting from User
class Customer : public User {
public:
    Customer(const string& uname, const string& pwd);
    bool authenticate(const string& uname, const string& pwd) const override;
};

// Define a subclass for Manager, inheriting from User
class Manager : public User {
public:
    Manager(const string& uname, const string& pwd);
    bool authenticate(const string& uname, const string& pwd) const override;
};

class Hotel {
private:
    string hotelname;
    string hoteladdress;
    string hotelPhonenumber;

public:
    Hotel(string name, string address, string num);
    void hotelinfoprint() const;
};

class RoomReservation {
private:
    int standardRooms;
    int deluxeRooms;
    vector<string> bookedRooms;
    unordered_map<string, string> checkedInGuests; // Maps roomNumber to customer name

public:
    RoomReservation(int stdRooms = 10, int dlxRooms = 5);
    bool bookRoom(string roomType);
    bool checkInCustomer(const string& customerName, const string& roomNumber, const string& checkInDate, const string& checkOutDate);
    void writeReceipt(const string& customerName, const string& roomNumber, const string& checkInDate, const string& checkOutDate, const string& roomType);
    string promptRoomType();
    void printCheckedInRooms() const ; // New function
};

void viewrooms();
void CancelRoomReservation(string roomname);
void displayMenu();
void displayMenuManager();
void login();

#endif // HOTEL_H
