#include "OrderDAO.h"

OrderDAO::OrderDAO(sqlite3* database)
    : db(database)
{
}

bool OrderDAO::insertOrder(
    std::shared_ptr<Order> order
)
{
    if (!order)
        return false;

    std::string sql =
        "INSERT INTO Orders VALUES("
        + std::to_string(order->getOrderId()) + ","
        + std::to_string(order->getCustomerId()) + ","
        + std::to_string(order->getRestaurantId()) + ","
        + std::to_string((int)order->getStatus()) + ","
        + std::to_string(order->getTotalPrice()) +
        ");";

    if (
        sqlite3_exec(
            db,
            sql.c_str(),
            nullptr,
            nullptr,
            nullptr
        ) != SQLITE_OK
        )
    {
        return false;
    }

    const auto& items =
        order->getItems();

    const auto& quantities =
        order->getQuantities();

    for (
        size_t i = 0;
        i < items.size();
        i++
        )
    {
        if (!items[i])
            continue;

        std::string itemSql =
            "INSERT INTO OrderItems VALUES("
            + std::to_string(order->getOrderId()) + ","
            + std::to_string(items[i]->getId()) + ","
            + std::to_string(quantities[i]) +
            ");";

        sqlite3_exec(
            db,
            itemSql.c_str(),
            nullptr,
            nullptr,
            nullptr
        );
    }

    return true;
}

std::vector<std::shared_ptr<Order>>
OrderDAO::getAllOrders()
{
    std::vector<std::shared_ptr<Order>> result;

    MenuItemDAO menuDAO(db);

    const char* sql =
        "SELECT * FROM Orders;";

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

        int customerId =
            sqlite3_column_int(stmt, 1);

        int restaurantId =
            sqlite3_column_int(stmt, 2);

        int status =
            sqlite3_column_int(stmt, 3);

        auto order =
            std::make_shared<Order>(
                id,
                customerId,
                restaurantId
            );

        order->setStatus(
            static_cast<OrderStatus>(status)
        );

        std::string itemSql =
            "SELECT itemId, quantity "
            "FROM OrderItems "
            "WHERE orderId="
            + std::to_string(id)
            + ";";

        sqlite3_stmt* itemStmt = nullptr;

        if (
            sqlite3_prepare_v2(
                db,
                itemSql.c_str(),
                -1,
                &itemStmt,
                nullptr
            ) == SQLITE_OK
            )
        {
            while (
                sqlite3_step(itemStmt)
                ==
                SQLITE_ROW
                )
            {
                int itemId =
                    sqlite3_column_int(
                        itemStmt,
                        0
                    );

                int quantity =
                    sqlite3_column_int(
                        itemStmt,
                        1
                    );

                auto item =
                    menuDAO.getMenuItemById(
                        itemId
                    );

                if (item)
                {
                    order->addItem(
                        item,
                        quantity
                    );
                }
            }

            sqlite3_finalize(itemStmt);
        }

        result.push_back(order);
    }

    sqlite3_finalize(stmt);

    return result;
}

std::shared_ptr<Order>
OrderDAO::getOrderById(int orderId)
{
    auto orders = getAllOrders();

    for (auto& order : orders)
    {
        if (
            order &&
            order->getOrderId() == orderId
            )
        {
            return order;
        }
    }

    return nullptr;
}

bool OrderDAO::updateOrderStatus(
    int orderId,
    OrderStatus status
)
{
    std::string sql =
        "UPDATE Orders SET status="
        + std::to_string((int)status)
        + " WHERE id="
        + std::to_string(orderId)
        + ";";

    return sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        nullptr
    ) == SQLITE_OK;
}

bool OrderDAO::deleteOrder(
    int orderId
)
{
    std::string deleteItems =
        "DELETE FROM OrderItems WHERE orderId="
        + std::to_string(orderId)
        + ";";

    sqlite3_exec(
        db,
        deleteItems.c_str(),
        nullptr,
        nullptr,
        nullptr
    );

    std::string deleteOrder =
        "DELETE FROM Orders WHERE id="
        + std::to_string(orderId)
        + ";";

    return sqlite3_exec(
        db,
        deleteOrder.c_str(),
        nullptr,
        nullptr,
        nullptr
    ) == SQLITE_OK;
}