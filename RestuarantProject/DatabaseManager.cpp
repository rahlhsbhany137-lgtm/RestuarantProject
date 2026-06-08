#include "DatabaseManager.h"
#include "User.h"
#include "Admin.h"
#include "Models.h"
#include <iostream>
#include <vector>
#include <memory>

DatabaseManager::DatabaseManager()
{
    db = nullptr;
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::openDatabase(const std::string& dbName)
{
    if (sqlite3_open(dbName.c_str(), &db))
    {
        std::cout << "Cannot open database!\n";
        return false;
    }

    std::cout << "Database opened successfully.\n";
    return true;
}

void DatabaseManager::closeDatabase()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseManager::execute(const std::string& sql)
{
    char* errMsg = nullptr;

    int result = sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        &errMsg
    );

    if (result != SQLITE_OK)
    {
        std::cout << "SQL Error: "
            << errMsg
            << std::endl;

        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool DatabaseManager::insertUser(
    int id,
    const std::string& username,
    const std::string& password,
    int role
)
{
    std::string sql =
        "INSERT INTO Users VALUES(" +
        std::to_string(id) + ",'" +
        username + "','" +
        password + "'," +
        std::to_string(role) + ");";

    return execute(sql);
}

void DatabaseManager::createTables()
{
    std::string usersTable =
        "CREATE TABLE IF NOT EXISTS Users ("
        "id INTEGER PRIMARY KEY,"
        "username TEXT,"
        "password TEXT,"
        "role INTEGER);";

    execute(usersTable);

    std::string restaurantsTable =
        "CREATE TABLE IF NOT EXISTS Restaurants ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT,"
        "address TEXT,"
        "phone TEXT,"
        "description TEXT,"
        "prepTime INTEGER,"
        "status INTEGER,"
        "adminId INTEGER);";

    execute(restaurantsTable);

    std::string ordersTable =
        "CREATE TABLE IF NOT EXISTS Orders ("
        "id INTEGER PRIMARY KEY,"
        "customerId INTEGER,"
        "restaurantId INTEGER,"
        "status INTEGER,"
        "total REAL);";

    execute(ordersTable);

    std::cout << "Tables created.\n";
}

std::vector<std::shared_ptr<User>> DatabaseManager::loadUsers()
{
    std::vector<std::shared_ptr<User>> result;

    const char* sql = "SELECT id, username, password, role FROM Users;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cout << "Failed to load users.\n";
        return result;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        std::string username = (const char*)sqlite3_column_text(stmt, 1);
        std::string password = (const char*)sqlite3_column_text(stmt, 2);
        int roleInt = sqlite3_column_int(stmt, 3);

        UserRole role = static_cast<UserRole>(roleInt);

        std::shared_ptr<User> user;

        if (role == UserRole::CUSTOMER)
            user = std::make_shared<Customer>(id, username, password);
        else if (role == UserRole::RESTAURANT_ADMIN)
            user = std::make_shared<RestaurantAdmin>(id, username, password, 0);
        else
            user = std::make_shared<SystemAdmin>(id, username, password);

        result.push_back(user);
    }

    sqlite3_finalize(stmt);
    return result;
}