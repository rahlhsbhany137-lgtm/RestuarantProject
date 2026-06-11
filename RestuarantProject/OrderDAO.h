#pragma once

#include <vector>
#include <memory>

#include "sqlite3.h"
#include "Order.h"
#include "MenuItemDAO.h"

class OrderDAO
{
private:
    sqlite3* db;

public:
    OrderDAO(sqlite3* database);

    bool insertOrder(
        std::shared_ptr<Order> order
    );

    std::vector<
        std::shared_ptr<Order>
    > getAllOrders();

    std::shared_ptr<Order>
        getOrderById(int orderId);

    bool updateOrderStatus(
        int orderId,
        OrderStatus status
    );

    bool deleteOrder(
        int orderId
    );
};