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

sqlite3* DatabaseManager::getDB()
{
    return db;
}