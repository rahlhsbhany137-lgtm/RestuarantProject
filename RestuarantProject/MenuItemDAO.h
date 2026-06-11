#pragma once
#include <vector>
#include <memory>
#include "sqlite3.h"
#include "Models.h"

class MenuItemDAO {
private:
    sqlite3* db;

public:
    MenuItemDAO(sqlite3* database);

    std::shared_ptr<MenuItem>
        getMenuItemById(int itemId);

    bool insertMenuItem(int restaurantId, std::shared_ptr<MenuItem> item);

    std::vector<std::shared_ptr<MenuItem>>
        getMenuItemsByRestaurant(int restaurantId);

    bool updateMenuItem(std::shared_ptr<MenuItem> item);

    bool deleteMenuItem(int itemId);
};
