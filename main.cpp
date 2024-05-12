#include "follow.h"
#include "repository.h"
#include "User.h"
#include <iostream>
#include <string>

using namespace std;

void displayMainOptions() {
    cout << "====================================================\n";
    cout << "||                Welcome Menu:                   ||\n";
    cout << "||------------------------------------------------||\n";
    cout << "||  1. Sign Up                                    ||\n";
    cout << "||  2. Sign In                                    ||\n";
    cout << "||  3. Terminate Program                          ||\n";
    cout << "||------------------------------------------------||\n";
    cout << "||  Select your choice:                           ||\n";
    cout << "====================================================\n";
}

void displayUserInterface(const string& username) {
    cout << "====================================================\n";
    cout << "||          User Dashboard - " << username << "                ||\n";
    cout << "||------------------------------------------------||\n";
    cout << "||  1. Initiate Repository                        ||\n";
    cout << "||  2. Remove Repository                          ||\n";
    cout << "||  3. Upload File to Repository                  ||\n";
    cout << "||  4. Erase File from Repository                 ||\n";
    cout << "||  5. Follow User                                ||\n";
    cout << "||  6. Stop Following User                        ||\n";
    cout << "||  7. Profile Overview                           ||\n";
    cout << "||  8. Clone Repo                                 ||\n";
    cout << "||  9. Sign Out                                   ||\n";
    cout << "||  10. Close Application                         ||\n";
    cout << "||-------------------------------------------------||\n";
    cout << "||  Make a selection:                             ||\n";
    cout << "====================================================\n";
}

int main() {
    UserTable users(100);
    RepoManager repos;
    UserManager followControl;
    bool active = true;
    string username;
    string password, repoName, fileName, fileContent, commitMsg, targetUser;
    int action;
    bool repoVisibility;

    while (active) {
        displayMainOptions();
        cin >> action;

        switch (action) {
        case 1:
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (users.addUser(username, password)) {
                cout << "You are now registered.\n";
            }
            else {
                cout << "Registration unsuccessful. This username is already in use.\n";
            }
            break;

        case 2:
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (users.authenticateUser(username, password)) {
                bool signedIn = true;
                while (signedIn) {
                    displayUserInterface(username);
                    cin >> action;
                    switch (action) {
                    case 1:
                        cout << "Specify repository name: ";
                        cin >> repoName;
                        cout << "Should this repository be public? (1 for Yes, 0 for No): ";
                        cin >> repoVisibility;
                        if (repos.addRepoToFile(username, repoName, repoVisibility)) 
                        {
                            cout << "Repository successfully established.\n";
                        }
                        else
                        {
                            cout << "Repository setup failed.\n";
                        }
                        break;
                    case 2:
                        cout << "Name of repository to remove: ";
                        cin >> repoName;
                        if (repos.deleteRepo(username, repoName)) {
                            cout << "Repository removed successfully.\n";
                        }
                        else {
                            cout << "Failed to locate or remove repository.\n";
                        }
                        break;
                    case 3:
                        cout << "Repository for file upload: ";
                        cin >> repoName;
                        cout << "File name: ";
                        cin >> fileName;
                        cin.ignore();  // Ignore the newline character for correct stream handling
                        cout << "Enter file content: ";
                        getline(cin, fileContent);
                        cout << "Commit message: ";
                        getline(cin, commitMsg);
                        if (repos.addFile(username, repoName, fileName, fileContent, commitMsg)) {
                            cout << "File uploaded and commit logged successfully.\n";
                        }
                        else {
                            cout << "File upload failed.\n";
                        }
                        break;
                    case 4:
                        cout << "Repository for file deletion: ";
                        cin >> repoName;
                        cout << "File name to delete: ";
                        cin >> fileName;
                        if (repos.removeFile(username, repoName, fileName)) {
                            cout << "File deleted successfully.\n";
                        }
                        else {
                            cout << "File deletion failed or file not found.\n";
                        }
                        break;
                    case 5:
                        cout << "Username to follow: ";
                        cin >> targetUser;
                        if (followControl.addUserFollow(username, targetUser)) {
                            cout << "You are now following " << targetUser << ".\n";
                        }
                        else {
                            cout << "Unable to follow " << targetUser << " (might already be following or user does not exist).\n";
                        }
                        break;
                    case 6:
                        cout << "Username to unfollow: ";
                        cin >> targetUser;
                        if (followControl.removeUserFollow(username, targetUser)) {
                            cout << "You have stopped following " << targetUser << ".\n";
                        }
                        else {
                            cout << "Unable to unfollow " << targetUser << " (not following or user does not exist).\n";
                        }
                        break;
                    case 7:
                        users.displayUserDetails();
                        break;
                    case 8:
                        repos.cloneRepository();
                        break;
                    case 9:
                        users.signOut();
                        signedIn = false;
                        break;
                    case 10:
                        users.signOut();
                        signedIn = false;
                        active = false;
                        break;
                    default:
                        cout << "Invalid selection. Please try again.\n";
                    }
                }
            }
            break;

        case 3:
            active = false;
            break;

        default:
            cout << "Invalid selection. Please try again.\n";
        }
    }

    return 0;
}
