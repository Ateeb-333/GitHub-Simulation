#ifndef FOLLOW_MANAGER_H
#define FOLLOW_MANAGER_H

#include <fstream>
#include <iostream>
#include <string>

struct UserNode {
    std::string userData;
    UserNode* nextUser;
    int followers;
    UserNode(std::string data) : userData(data), nextUser(nullptr), followers(0) {}
};

class UserManager {
private:
    UserNode* userList;

    UserNode* getUser(const std::string& user);

public:
    UserManager();
    ~UserManager();
    void recordFollowing(const std::string& file);
    void populateUsers(const std::string& file);
    void recordFollowers(const std::string& file);
    bool addUserFollow(const std::string& user, const std::string& target);
    bool removeUserFollow(const std::string& user, const std::string& target);
};

#endif
