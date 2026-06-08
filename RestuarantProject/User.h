#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Cart.h"
#include "Order.h"

class System; // forward

class User {
protected:
    int id;
    std::string username;
    std::string password;
    UserRole role;

public:
    User(int id, const std::string& u, const std::string& p, UserRole r);
    virtual ~User() = default;

    int getId() const;
    std::string getUsername() const;
    std::string getPassword() const;
    UserRole getRole() const;
};

// ---------- Customer ----------
class Customer : public User {
private:
    Cart cart;

public:
    Customer(int id, const std::string& u, const std::string& p);

    Cart& getCart();

    // طبق نیازمندی: بعد از نهایی‌سازی، سفارش در سیستم ذخیره می‌شود
    std::shared_ptr<Order> checkout(System& system, int restaurantId);

    void showHistory(System& system) const;
};