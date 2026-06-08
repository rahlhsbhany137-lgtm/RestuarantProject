#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "User.h"
#include "Admin.h"
#include "sqlite3.h"

class DatabaseManager {
private:
    sqlite3* db;

public:
    DatabaseManager();
    ~DatabaseManager();

    bool openDatabase(const std::string& dbName);
    void closeDatabase();

    bool execute(const std::string& sql);

    bool insertUser(
        int id,
        const std::string& username,
        const std::string& password,
        int role
    );

    std::vector<std::shared_ptr<User>> loadUsers();

    void createTables();
};

#endif
