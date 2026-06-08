#pragma once
#include <vector>
#include <memory>
#include "Models.h"

class CartItem;

class Order {
private:
    int id;
    int customerId;
    int restaurantId;

    std::vector<std::shared_ptr<MenuItem>> items;
    std::vector<int> quantities;

    OrderStatus status;

public:
    Order(int id, int customerId, int restaurantId);
    Order(int id, int customerId, int restaurantId,
        const std::vector<std::shared_ptr<CartItem>>& cartItems);

    int getOrderId() const;
    int getCustomerId() const;
    int getRestaurantId() const;

    void addItem(std::shared_ptr<MenuItem> item, int quantity);

    void setStatus(OrderStatus s);
    OrderStatus getStatus() const;

    double getTotalPrice() const;
    void showOrder() const;
};
