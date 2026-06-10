#include "UserDAO.h"

UserDAO::UserDAO(sqlite3* database)
    : db(database)
{
}
bool UserDAO::insertUser(std::shared_ptr<User> user)
{
    std::string sql =
        "INSERT OR IGNORE INTO Users VALUES(" +
        std::to_string(user->getId()) +
        ",'" +
        user->getUsername() +
        "','" +
        user->getPassword() +
        "'," +
        std::to_string((int)user->getRole()) +
        ");";

    char* errMsg = nullptr;

    int result =
        sqlite3_exec(
            db,
            sql.c_str(),
            nullptr,
            nullptr,
            &errMsg
        );

    if (result != SQLITE_OK)
    {
        sqlite3_free(errMsg);
        return false;
    }

    return true;
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