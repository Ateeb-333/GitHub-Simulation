#include "User.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

UserTable::UserTable(int size) : capacity(size) {
    buckets = new User * [capacity];
    for (int i = 0; i < capacity; ++i) {
        buckets[i] = nullptr;
    }
    loadUsers();
}

UserTable::~UserTable() {
    saveUsers();
    for (int i = 0; i < capacity; ++i) {
        User* entry = buckets[i];
        while (entry != nullptr) {
            User* prev = entry;
            entry = entry->next;
            delete prev;
        }
    }
    delete[] buckets;
}

int UserTable::generateHash(string key) {
    int hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c) % capacity;
    }
    return hash;
}

bool UserTable::addUser(string username, string password) {
    int index = generateHash(username);
    User* newUser = new User(username, password);
    if (!buckets[index]) {
        buckets[index] = newUser;
    }
    else {
        User* temp = buckets[index];
        while (temp->next) {
            if (temp->username == username) {
                delete newUser; // Prevent memory leak if user exists
                return false; // User already exists
            }
            temp = temp->next;
        }
        if (temp->username == username) {
            delete newUser;
            return false;
        }
        temp->next = newUser;
    }
    saveUsers();
    return true;
}

bool UserTable::authenticateUser(string username, string password) {
    int index = generateHash(username);
    User* temp = buckets[index];
    while (temp) {
        if (temp->username == username && temp->password == password) {
            cout << "Login successful!\n";
            return true;
        }
        temp = temp->next;
    }
    cout << "Invalid username or password!\n";
    return false;
}

void UserTable::signOut() {
    cout << "User signed out successfully.\n";
}

void UserTable::loadUsers() {
    ifstream file("user.csv");
    string username, password;
    while (file >> username >> password) {
        addUser(username, password);
    }
    file.close();
}

void UserTable::saveUsers() {
    ofstream file("users.csv", ofstream::trunc);
    for (int i = 0; i < capacity; ++i) {
        User* temp = buckets[i];
        while (temp) {
            file << temp->username << " " << temp->password << endl;
            temp = temp->next;
        }
    }
    file.close();
}

void UserTable::displayUserDetails() {
    string searchUsername;
    cout << "Enter the username to display details: ";
    cin >> searchUsername;
    int index = generateHash(searchUsername);
    User* temp = buckets[index];
    while (temp) {
        if (temp->username == searchUsername) {
            cout << "Username: " << temp->username << endl;
            // Add any additional details to display here, such as followers or following, based on available data
            return;
        }
        temp = temp->next;
    }
    cout << "User not found." << endl;
}
