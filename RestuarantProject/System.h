#pragma once
#include <vector>
#include <memory>
#include <string>

#include "User.h"
#include "Restaurant.h"
#include "Order.h"
#include "Cart.h"
#include "DatabaseManager.h"

class System {
private:
    std::vector<std::shared_ptr<User>> users;
    std::vector<std::shared_ptr<Restaurant>> restaurants;
    std::vector<std::shared_ptr<Order>> orders;

    DatabaseManager db;

    int nextOrderId = 1;

public:
    System();
    void registerUser(std::shared_ptr<User> user);
    std::shared_ptr<User> login(const std::string& u, const std::string& p);

    void addRestaurant(std::shared_ptr<Restaurant> r);

    
    void showRestaurants() const;

    std::vector<std::shared_ptr<Restaurant>>& getRestaurants();
    const std::vector<std::shared_ptr<Restaurant>>& getRestaurants() const;

    std::shared_ptr<Restaurant> findRestaurantById(int id);
    std::shared_ptr<User> findUserById(int id);

    
    std::shared_ptr<Order> createOrder(int customerId, int restaurantId,
        const std::vector<std::shared_ptr<CartItem>>& cartItems);

  
    std::vector<std::shared_ptr<Order>> getOrdersByCustomerId(int customerId) const;
    std::vector<std::shared_ptr<Order>> getOrdersByRestaurantId(int restaurantId) const;

    bool setOrderStatus(int orderId, int restaurantId, OrderStatus status);

    
    bool toggleRestaurantStatus(int restaurantId);
    void showReports() const;
    void loadFromDatabase();
};