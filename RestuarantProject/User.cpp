#include "User.h"
#include "System.h"
#include <iostream>

// ---------- User ----------
User::User(int id, const std::string& u, const std::string& p, UserRole r)
    : id(id), username(u), password(p), role(r) {
}

int User::getId() const { return id; }
std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
UserRole User::getRole() const { return role; }

// ---------- Customer ----------
Customer::Customer(int id, const std::string& u, const std::string& p)
    : User(id, u, p, UserRole::CUSTOMER) {
}

Cart& Customer::getCart() { return cart; }

std::shared_ptr<Order> Customer::checkout(System& system, int restaurantId) {
    if (cart.getItems().empty()) {
        std::cout << "Cart is empty!\n";
        return nullptr;
    }

    cart.showCart();
    std::cout << "Confirm order? (1 = yes, 0 = no): ";
    int choice;
    std::cin >> choice;
    if (choice != 1) {
        std::cout << "Checkout cancelled.\n";
        return nullptr;
    }

    auto order = system.createOrder(id, restaurantId, cart.getItems());

    if (order) {
        cart.clear();
        std::cout << "Order placed successfully! Order ID: " << order->getOrderId() << "\n";
        return order;

    }
    return nullptr;
}

void Customer::showHistory(System& system) const {
    auto orders = system.getOrdersByCustomerId(id);
    std::cout << "\n---ORDER HISTORY---\n";
    if (orders.empty()) {
        std::cout << "No orders yet.\n";
        return;
    }
    for (const auto& o : orders) {
        if (o) o->showOrder();
    }
}

