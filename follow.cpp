#include "Follow.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

UserManager::UserManager() : userList(nullptr) {
    populateUsers("usernames.csv");
}

UserManager::~UserManager() {
    recordFollowers("followers_list.csv");
    UserNode* current = userList;
    while (current) {
        UserNode* temp = current->nextUser;
        delete current;
        current = temp;
    }
}

void UserManager::populateUsers(const string& file) {
    ifstream inFile(file);
    if (!inFile) {
        cerr << "Error: File " << file << " cannot be opened." << endl;
        return;
    }

    string name;
    while (inFile >> name) {
        UserNode* newUser = new UserNode(name);
        newUser->nextUser = userList;
        userList = newUser;
    }
    inFile.close();
}

UserNode* UserManager::getUser(const string& name) {
    UserNode* temp = userList;
    while (temp) {
        if (temp->userData == name) {
            return temp;
        }
        temp = temp->nextUser;
    }
    return nullptr;
}

void UserManager::recordFollowers(const string& file) {
    ofstream outFile(file);
    UserNode* current = userList;
    while (current) {
        outFile << current->userData << " " << current->followers;
        UserNode* follower = current->nextUser;
        while (follower) {
            outFile << " " << follower->userData;
            follower = follower->nextUser;
        }
        outFile << endl;
        current = current->nextUser;
    }
}

void UserManager::recordFollowing(const string& file) {
    ofstream outFile(file);
    UserNode* current = userList;
    while (current) {
        outFile << current->userData << " " << current->followers;
        UserNode* follower = current->nextUser;
        while (follower) {
            outFile << " " << follower->userData;
            follower = follower->nextUser;
        }
        outFile << endl;
        current = current->nextUser;
    }
}

bool UserManager::addUserFollow(const string& user, const string& target) {
    UserNode* userNode = getUser(user);
    UserNode* targetNode = getUser(target);
    if (!userNode || !targetNode || user == target) {
        return false;
    }

    UserNode* temp = userNode->nextUser;
    while (temp) {
        if (temp->userData == target) {
            return false; // Already following
        }
        temp = temp->nextUser;
    }

    UserNode* newFollower = new UserNode(target);
    newFollower->nextUser = userNode->nextUser;
    userNode->nextUser = newFollower;

    targetNode->followers++;

    ofstream followFile("following.csv", ios::app);
    if (followFile) {
        followFile << user << " " << target << endl;
        followFile.close();
    }

    ofstream followersFile("followers.csv", ios::app);
    if (followersFile) {
        followersFile << target << " " << user << endl;
        followersFile.close();
    }

    return true;
}

bool UserManager::removeUserFollow(const string& user, const string& target) {
    UserNode* userNode = getUser(user);
    if (!userNode || user == target) {
        return false;
    }

    UserNode* prev = userNode;
    UserNode* temp = userNode->nextUser;
    while (temp) {
        if (temp->userData == target) {
            prev->nextUser = temp->nextUser;
            delete temp;
            return true;
        }
        prev = temp;
        temp = temp->nextUser;
    }

    return false;
}
