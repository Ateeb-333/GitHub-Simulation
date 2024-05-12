#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <iostream>
#include <fstream>
#include <string>

// Structure for a user in the hash table
struct User {
    std::string username;
    std::string password;
    User* next;

    User(std::string uname, std::string pword) : username(uname), password(pword), next(nullptr) {}
};

// Hash table class to manage user data
class UserTable {
private:
    User** buckets;
    int capacity;
    int generateHash(std::string key);
    void loadUsers();
    void saveUsers();
    void saveUsernames();

public:
    UserTable(int size);
    ~UserTable();
    bool addUser(std::string username, std::string password);
    bool authenticateUser(std::string username, std::string password);
    void signOut();
    void displayUserDetails();
};

#endif
