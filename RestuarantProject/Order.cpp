#include "Order.h"
#include "Cart.h"
#include <iostream>
#include <iomanip>

Order::Order(int id, int customerId, int restaurantId)
    : id(id),
    customerId(customerId),
    restaurantId(restaurantId),
    status(OrderStatus::PREPARING) {
}

Order::Order(int id, int customerId, int restaurantId,
    const std::vector<std::shared_ptr<CartItem>>& cartItems)
    : id(id),
    customerId(customerId),
    restaurantId(restaurantId),
    status(OrderStatus::PREPARING)
{
    for (const auto& ci : cartItems) {
        if (ci && ci->getItem()) {
            items.push_back(ci->getItem());
            quantities.push_back(ci->getQuantity());
        }

    }
}

int Order::getOrderId() const { return id; }
int Order::getCustomerId() const { return customerId; }
int Order::getRestaurantId() const { return restaurantId; }

void Order::addItem(std::shared_ptr<MenuItem> item, int quantity) {
    if (item && quantity > 0) {
        items.push_back(item);
        quantities.push_back(quantity);
    }

}

void Order::setStatus(OrderStatus s) { status = s; }
OrderStatus Order::getStatus() const { return status; }

double Order::getTotalPrice() const {
    double total = 0;
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]) {
            total += items[i]->getPrice() * quantities[i];
        }
    }
    return total;
}

void Order::showOrder() const {
    std::cout << "Order ID: " << id
        << " | RestaurantID: " << restaurantId
        << " | CustomerID: " << customerId
        << "\n";

    for (size_t i = 0; i < items.size(); i++) {
        if (items[i]) {
            std::cout << "-" << std::left << std::setw(15) << items[i]->getName()
                << " x" << quantities[i]
                << "($" << (items[i]->getPrice() * quantities[i]) << ")\n";
        }

    }

    std::cout << "Total: $" << std::fixed << std::setprecision(2) << getTotalPrice() << "\n";

    std::cout << "Status: ";
    switch (status) {
    case OrderStatus::PREPARING:
        std::cout << "Preparing";
        break;
    case OrderStatus::READY_TO_SEND:
        std::cout << "Ready To Send";
        break;
    case OrderStatus::DELIVERED:
        std::cout << "Delivered";
        break;
    default:
        std::cout << "UNKNOWN";
        break;
    }
    std::cout << "\n";
}