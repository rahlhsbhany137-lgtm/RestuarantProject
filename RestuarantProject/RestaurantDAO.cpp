#include "RestaurantDAO.h"

RestaurantDAO::RestaurantDAO(
    sqlite3* database
)
    : db(database)
{
}

bool RestaurantDAO::insertRestaurant(
    std::shared_ptr<Restaurant> restaurant
)
{
    std::string sql =
        "INSERT OR IGNORE INTO Restaurants VALUES(" +
        std::to_string(
            restaurant->getId()
        ) +
        ",'" +
        restaurant->getName() +
        "','" +
        restaurant->getAddress() +
        "','" +
        restaurant->getPhone() +
        "','" +
        restaurant->getExtraDesc() +
        "'," +
        std::to_string(
            restaurant->getPrepTimeMinutes()
        ) +
        "," +
        std::to_string(
            restaurant->isActiveStatus()
        ) +
        "," +
        std::to_string(
            restaurant->getAdminUserId()
        ) +
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

std::vector<
    std::shared_ptr<Restaurant>
>
RestaurantDAO::getAllRestaurants()
{
    std::vector<
        std::shared_ptr<Restaurant>
    > result;

    const char* sql =
        "SELECT * FROM Restaurants;";

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

    while (
        sqlite3_step(stmt)
        ==
        SQLITE_ROW
        )
    {
        int id =
            sqlite3_column_int(stmt, 0);

        std::string name =
            (const char*)
            sqlite3_column_text(stmt, 1);

        std::string address =
            (const char*)
            sqlite3_column_text(stmt, 2);

        std::string phone =
            (const char*)
            sqlite3_column_text(stmt, 3);

        std::string desc =
            (const char*)
            sqlite3_column_text(stmt, 4);

        int prep =
            sqlite3_column_int(stmt, 5);

        int adminId =
            sqlite3_column_int(stmt, 7);

        auto restaurant =
            std::make_shared<
            Restaurant>(
                id,
                name,
                address,
                prep,
                phone,
                desc,
                adminId
            );

        result.push_back(
            restaurant
        );
    }

    sqlite3_finalize(stmt);

    return result;
}