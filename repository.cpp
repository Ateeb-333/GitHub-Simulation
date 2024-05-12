#include "repository.h"
#include <direct.h>
#include <errno.h>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

RepoManager::RepoManager() {
    loadReposFromFile("public_repos.csv", true);
    loadReposFromFile("private_repositries.csv", false);
}

RepoManager::~RepoManager() {}

void RepoManager::loadReposFromFile(const string& filename, bool isPublic) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << " for reading." << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string owner, repoName;
        if (!(ss >> owner >> repoName)) {
            cerr << "Failed to parse line: " << line << endl;
            continue;
        }
        auto newRepo = make_unique<RepoNode>(owner, repoName, isPublic);
        newRepo->nextRepo = move(repoList);
        repoList = move(newRepo);
    }
    file.close();
}

void RepoManager::rewriteRepoFiles() {
    ofstream publicFile("public_repos.csv", ofstream::trunc);
    ofstream privateFile("private_repositries.csv", ofstream::trunc);
    RepoNode* current = repoList.get();
    while (current != nullptr) {
        ofstream& file = current->isPublic ? publicFile : privateFile;
        file << current->owner << " " << current->repoName << endl;
        current = current->nextRepo.get();
    }
}

bool RepoManager::establishRepo(string owner, string repoName, bool isPublic) {
    if (checkRepoExists(owner, repoName)) {
        cout << "Repository already exists.\n";
        return false;
    }
    string path = "./" + owner + "_" + repoName;
    if (_mkdir(path.c_str()) == -1) {
        perror("Error creating directory");
        return false;
    }
    auto newRepo = make_unique<RepoNode>(owner, repoName, isPublic);
    newRepo->nextRepo = move(repoList);
    repoList = move(newRepo);
    addRepoToFile(owner, repoName, isPublic);
    cout << "Repository created successfully.\n";
    return true;
}

bool RepoManager::deleteRepo(string owner, string repoName) {
    unique_ptr<RepoNode>* current = &repoList;
    while (*current) {
        if ((*current)->owner == owner && (*current)->repoName == repoName) {
            string path = "./" + owner + "_" + repoName;
            if (_rmdir(path.c_str()) == -1) {
                perror("Error deleting directory");
                return false;
            }
            *current = move((*current)->nextRepo);
            cout << "Repository deleted successfully.\n";
            return true;
        }
        current = &((*current)->nextRepo);
    }
    cout << "Repository not found.\n";
    return false;
}

bool RepoManager::addFile(string owner, string repoName, string fileName, string content, string commitMsg) {
    auto repoNode = locateOrCreateRepo(owner, repoName);
    if (!repoNode) return false;
    auto fileNode = locateOrCreateFile(repoNode, fileName);
    if (!fileNode) return false;
    ofstream file("./" + owner + "_" + repoName + "/" + fileName);
    if (!file.is_open()) {
        cerr << "Failed to create file.\n";
        return false;
    }
    file << content;
    file.close();
    int commitNumber = 1;
    if (fileNode->commitHistory) {
        auto commit = fileNode->commitHistory.get();
        while (commit->nextCommit) {
            commit = commit->nextCommit.get();
            commitNumber++;
        }
        commit->nextCommit = make_unique<CommitDetails>(commitMsg, commitNumber + 1);
    }
    else {
        fileNode->commitHistory = make_unique<CommitDetails>(commitMsg, commitNumber);
    }
    logCommitDetails(owner, repoName, fileName, commitMsg, commitNumber);
    return true;
}

bool RepoManager::removeFile(string owner, string repoName, string fileName) {
    if (!checkRepoExists(owner, repoName)) {
        cerr << "Repository does not exist.\n";
        return false;
    }
    string path = "./" + owner + "_" + repoName;
    string fullPath = path + "/" + fileName;
    if (!checkFileExists(path, fileName)) {
        cerr << "File does not exist.\n";
        return false;
    }
    if (remove(fullPath.c_str()) != 0) {
        perror("Error deleting file");
        return false;
    }
    cout << "File deleted successfully.\n";
    return true;
}

void RepoManager::cloneRepository() {
    // Implementation for cloning a repository
}

// Helper functions to manage files, repos, and commits:
bool RepoManager::checkRepoExists(const string& owner, const string& repoName) {
    for (RepoNode* current = repoList.get(); current != nullptr; current = current->nextRepo.get()) {
        if (current->owner == owner && current->repoName == repoName) {
            return true;
        }
    }
    return false;
}

bool RepoManager::checkFileExists(const string& path, const string& fileName) {
    ifstream file(path + "/" + fileName);
    return file.good();
}

RepoNode* RepoManager::locateOrCreateRepo(const string& owner, const string& repoName) {
    for (RepoNode* current = repoList.get(); current != nullptr; current = current->nextRepo.get()) {
        if (current->owner == owner && current->repoName == repoName) {
            return current;
        }
    }
    return nullptr; // Simplification: assume repository must already exist
}

FileInfo* RepoManager::locateOrCreateFile(RepoNode* repo, const string& fileName) {
    for (FileInfo* current = repo->fileHead.get(); current != nullptr; current = current->nextFile.get()) {
        if (current->name == fileName) {
            return current;
        }
    }
    auto newFile = make_unique<FileInfo>(fileName);
    FileInfo* newFilePtr = newFile.get();
    newFile->nextFile = move(repo->fileHead);
    repo->fileHead = move(newFile);
    return newFilePtr;
}

bool RepoManager::addRepoToFile(std::string owner, std::string repoName, bool publicStatus) 
{
    string filename = publicStatus ? "public_repositories.csv" : "private_repositories.csv";
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << owner << " " << repoName << "\n";

        return true;
    }
	else {
		return false;
}
}

void RepoManager::logCommitDetails(const string& owner, const string& repoName, const string& fileName, const string& commitMsg, int commitNum) {
    ofstream log("commit_log.csv", ios::app);
    if (log.is_open()) {
        log << "User: " << owner << ", Repo: " << repoName << ", File: " << fileName << ", Commit #" << commitNum << ": " << commitMsg << endl;
    }
}
