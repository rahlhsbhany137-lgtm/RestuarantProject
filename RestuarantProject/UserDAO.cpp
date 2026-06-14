#include "UserDAO.h"

UserDAO::UserDAO(sqlite3* database)
    : db(database)
{
}
bool UserDAO::insertUser(
    std::shared_ptr<User> user
)
{
    const char* sql =
        "INSERT OR IGNORE INTO Users "
        "(id, username, password, role) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (
        sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr
        ) != SQLITE_OK
        )
    {
        return false;
    }

    sqlite3_bind_int(
        stmt,
        1,
        user->getId()
    );

    sqlite3_bind_text(
        stmt,
        2,
        user->getUsername().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt,
        3,
        user->getPassword().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(
        stmt,
        4,
        (int)user->getRole()
    );

    bool success =
        (
            sqlite3_step(stmt)
            ==
            SQLITE_DONE
            );

    sqlite3_finalize(stmt);

    return success;
}

std::vector<std::shared_ptr<User>>
UserDAO::getAllUsers()
{
    std::vector<std::shared_ptr<User>> result;

    const char* sql =
        "SELECT id, username, password, role FROM Users;";

    sqlite3_stmt* stmt;

    if (
        sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr
        ) != SQLITE_OK
        )
    {
        return result;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id =
            sqlite3_column_int(stmt, 0);

        std::string username =
            (const char*)
            sqlite3_column_text(stmt, 1);

        std::string password =
            (const char*)
            sqlite3_column_text(stmt, 2);

        int roleInt =
            sqlite3_column_int(stmt, 3);

        UserRole role =
            static_cast<UserRole>(roleInt);

        std::shared_ptr<User> user;

        if (role == UserRole::CUSTOMER)
        {
            user =
                std::make_shared<Customer>(
                    id,
                    username,
                    password
                );
        }
        else if (
            role ==
            UserRole::RESTAURANT_ADMIN
            )
        {
            user =
                std::make_shared<
                RestaurantAdmin>(
                    id,
                    username,
                    password,
                    0
                );
        }
        else
        {
            user =
                std::make_shared<
                SystemAdmin>(
                    id,
                    username,
                    password
                );
        }

        result.push_back(user);
    }

    sqlite3_finalize(stmt);

    return result;
}

std::shared_ptr<User>
UserDAO::getUserById(int id)
{
    const char* sql =
        "SELECT id, username, password, role FROM Users WHERE id=?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_int(stmt, 1, id);

    std::shared_ptr<User> user = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int uid = sqlite3_column_int(stmt, 0);
        std::string username = (const char*)sqlite3_column_text(stmt, 1);
        std::string password = (const char*)sqlite3_column_text(stmt, 2);
        int roleInt = sqlite3_column_int(stmt, 3);

        UserRole role = static_cast<UserRole>(roleInt);

        if (role == UserRole::CUSTOMER)
            user = std::make_shared<Customer>(uid, username, password);
        else if (role == UserRole::RESTAURANT_ADMIN)
            user = std::make_shared<RestaurantAdmin>(uid, username, password, 0);
        else
            user = std::make_shared<SystemAdmin>(uid, username, password);
    }

    sqlite3_finalize(stmt);
    return user;
}

std::shared_ptr<User>
UserDAO::getUserByCredentials(const std::string& username,
    const std::string& password)
{
    const char* sql =
        "SELECT id, username, password, role "
        "FROM Users WHERE username=? AND password=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    std::shared_ptr<User> user = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        std::string u = (const char*)sqlite3_column_text(stmt, 1);
        std::string p = (const char*)sqlite3_column_text(stmt, 2);
        int roleInt = sqlite3_column_int(stmt, 3);

        UserRole role = static_cast<UserRole>(roleInt);

        if (role == UserRole::CUSTOMER)
        {
            user = std::make_shared<Customer>(id, u, p);
        }
        else if (role == UserRole::RESTAURANT_ADMIN)
        {
            user = std::make_shared<RestaurantAdmin>(id, u, p, 0);
        }
        else
        {
            user = std::make_shared<SystemAdmin>(id, u, p);
        }
    }

    sqlite3_finalize(stmt);
    return user;
}

bool UserDAO::updateUser(std::shared_ptr<User> user)
{
    const char* sql =
        "UPDATE Users SET username=?, password=?, role=? WHERE id=?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, user->getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user->getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, (int)user->getRole());
    sqlite3_bind_int(stmt, 4, user->getId());

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return ok;
}

bool UserDAO::deleteUser(int id)
{
    const char* sql = "DELETE FROM Users WHERE id=?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, id);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return ok;
}