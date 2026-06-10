#pragma once

#include <vector>
#include <memory>

#include "sqlite3.h"
#include "User.h"
#include "Admin.h"

class UserDAO
{
private:
    sqlite3* db;

public:
    UserDAO(sqlite3* database);

    bool insertUser(std::shared_ptr<User> user);

    std::vector<std::shared_ptr<User>>
        getAllUsers();
};
