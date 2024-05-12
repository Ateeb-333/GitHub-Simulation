#ifndef REPO_MANAGEMENT_H
#define REPO_MANAGEMENT_H

#include <memory>
#include <string>
#include <fstream>

struct CommitDetails {
    std::string message;
    int number;
    std::unique_ptr<CommitDetails> nextCommit;

    CommitDetails(std::string msg, int num) : message(msg), number(num), nextCommit(nullptr) {}
};

struct FileInfo {
    std::string name;
    std::unique_ptr<CommitDetails> commitHistory;
    std::unique_ptr<FileInfo> nextFile;

    FileInfo(std::string fileName) : name(fileName), commitHistory(nullptr), nextFile(nullptr) {}
};

struct RepoNode {
    std::string owner;
    std::string repoName;
    bool isPublic;
    std::unique_ptr<FileInfo> fileHead;
    std::unique_ptr<RepoNode> nextRepo;

    RepoNode(std::string user, std::string name, bool publicStatus)
        : owner(user), repoName(name), isPublic(publicStatus), fileHead(nullptr), nextRepo(nullptr) {}
};

class RepoManager {
private:
    std::unique_ptr<RepoNode> repoList;
   
    void rewriteRepoFiles();
    bool checkRepoExists(const std::string& owner, const std::string& repoName);
    bool checkFileExists(const std::string& path, const std::string& fileName);
    void logFileActivity(const std::string& owner, const std::string& repoName, const std::string& fileName);
    void logCommitDetails(const std::string& owner, const std::string& repoName, const std::string& fileName, const std::string& commitMsg, int commitNum);
    RepoNode* locateOrCreateRepo(const std::string& owner, const std::string& repoName);
    FileInfo* locateOrCreateFile(RepoNode* repo, const std::string& fileName);

public:
    RepoManager();
    ~RepoManager();
    bool addRepoToFile(std::string owner, std::string repoName, bool publicStatus);
    bool establishRepo(std::string owner, std::string repoName, bool isPublic);
    bool deleteRepo(std::string owner, std::string repoName);
    void loadReposFromFile(const std::string& file, bool isPublic);
    bool addFile(std::string owner, std::string repoName, std::string fileName, std::string content, std::string commitMsg);
    bool removeFile(std::string owner, std::string repoName, std::string fileName);
    void cloneRepository();
};

#endif
