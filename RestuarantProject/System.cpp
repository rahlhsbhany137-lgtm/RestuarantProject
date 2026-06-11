#include "System.h"
#include <iostream>
#include <algorithm>


System::System() {
    db.openDatabase("restaurant.db");
    db.createTables();
    userDAO = new UserDAO(db.getDB());
    orderDAO = new OrderDAO(db.getDB());
    restaurantDAO =new RestaurantDAO(db.getDB());
    menuItemDAO = new MenuItemDAO(db.getDB());


    loadFromDatabase();

    std::cout << "Loaded users" << users.size() << std::endl;
}

void System::registerUser(std::shared_ptr<User> user)
{
    if (!user) return;

    
    users.push_back(user);

    userDAO->insertUser(user);
}

std::shared_ptr<User> System::login(const std::string& u, const std::string& p) {
    for (auto& user : users) {
        if (user->getUsername() == u && user->getPassword() == p)
            return user;
    }
    return nullptr;
}

void System::addRestaurant(std::shared_ptr<Restaurant> r) {
    restaurants.push_back(r);
    restaurantDAO->insertRestaurant(r);
}


void System::showRestaurants() const {
    std::cout << "\n---List of active restaurants---\n";
    int idx = 1;
    for (auto& r : restaurants) {
        if (!r) continue;
        if (!r->isActiveStatus()) continue;
        std::cout << idx++ << ". Name: " << r->getName() << "\n"
            << "About:" << r->getExtraDesc() << "\n"
            << "| Address:" << r->getAddress()
            << "|phone:" << r->getPhone() << "\n"
            << "|Est. Prep Time:" << r->getPrepTimeMinutes() << "mins\n"
            << " (ID=" << r->getId() << ")\n";
    }
    if (idx == 1) std::cout << "No active restaurants.\n";
}

std::vector<std::shared_ptr<Restaurant>>& System::getRestaurants() { return restaurants; }
const std::vector<std::shared_ptr<Restaurant>>& System::getRestaurants() const { return restaurants; }

std::shared_ptr<Restaurant> System::findRestaurantById(int id) {
    for (auto& r : restaurants) {
        if (r && r->getId() == id) return r;
    }
    return nullptr;
}

std::shared_ptr<User> System::findUserById(int id)
{
    for (auto& u : users)
    {
        if (u && u->getId() == id)
            return u;
    }

    return nullptr;
}

std::shared_ptr<Order> System::createOrder(int customerId, int restaurantId,
    const std::vector<std::shared_ptr<CartItem>>& cartItems)
{
    auto r = findRestaurantById(restaurantId);
    if (!r) {
        std::cout << "Restaurant not found.\n";
        return nullptr;
    }
    if (!r->isActiveStatus()) {
        std::cout << "Restaurant is inactive.\n";
        return nullptr;
    }

    auto order = std::make_shared<Order>(nextOrderId++, customerId, restaurantId, cartItems);
    orders.push_back(order);
    orderDAO->insertOrder(order);

 
    return order;
}

std::vector<std::shared_ptr<Order>> System::getOrdersByCustomerId(int customerId) const {
    std::vector<std::shared_ptr<Order>> res;
    for (auto& o : orders) {
        if (o && o->getCustomerId() == customerId) res.push_back(o);
    }
    return res;
}

std::vector<std::shared_ptr<Order>> System::getOrdersByRestaurantId(int restaurantId) const {
    std::vector<std::shared_ptr<Order>> res;
    for (auto& o : orders) {
        if (o && o->getRestaurantId() == restaurantId) res.push_back(o);
    }
    return res;
}

bool System::setOrderStatus(int orderId, int restaurantId, OrderStatus status) {
    for (auto& o : orders) {
        if (!o) continue;
        if (o->getOrderId() == orderId && o->getRestaurantId() == restaurantId) {
            o->setStatus(status);
            orderDAO->updateOrderStatus(orderId, status);
            return true;
        }
    }
    std::cout << "Order not found or you dont heve permission to update it.\n";
    return false;
}

bool System::toggleRestaurantStatus(int restaurantId) {
    auto r = findRestaurantById(restaurantId);
    if (!r) return false;
    r->toggleStatus();
    return true;
}

void System::showReports() const {
    std::cout << "\n--- SYSTEM REPORTS ---\n";
    std::cout << "Total Registered Users: " << users.size() << "\n";
    std::cout << "Total Restaurants: " << restaurants.size() << "\n";

   
    size_t activeCount = std::count_if(restaurants.begin(), restaurants.end(),
        [](const std::shared_ptr<Restaurant>& r) {
            return r && r->isActiveStatus();
        });

    std::cout << "Total Active: " << activeCount << "\n";
    std::cout << "Total Orders: " << orders.size() << "\n";

    double totalSales = 0;
    for (auto& o : orders) {
        if (o) { 
            totalSales += o->getTotalPrice();
        }
    }
    std::cout << "Total Sales: $" << totalSales << "\n";
    std::cout << "----------------------\n";
}

void System::loadFromDatabase()
{
    users =
        userDAO->getAllUsers();

    restaurants =
        restaurantDAO->getAllRestaurants();

    for (auto& r : restaurants)
    {
        auto menu =
            menuItemDAO->getMenuItemsByRestaurant(
                r->getId()
            );

        r->setMenu(menu);
    }

    orders =
        orderDAO->getAllOrders();
}