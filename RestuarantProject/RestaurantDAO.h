#pragma once

#include <vector>
#include <memory>

#include "sqlite3.h"
#include "Restaurant.h"


class RestaurantDAO
{
private:
    sqlite3* db;
    

public:
    RestaurantDAO(sqlite3* database);

    bool insertRestaurant(
        std::shared_ptr<Restaurant> restaurant
    );

    std::vector<
        std::shared_ptr<Restaurant>
    > getAllRestaurants();

    std::shared_ptr<Restaurant> getRestaurantById(int id);
    bool updateRestaurant(std::shared_ptr<Restaurant> restaurant);
    bool deleteRestaurant(int id);

    bool updateRestaurantStatus(
        int restaurantId,
        bool status
    );
};
