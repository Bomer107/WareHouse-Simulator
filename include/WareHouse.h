#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addCustomer(Customer *customer);
        void addVolunteer(Volunteer *volunteer);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        vector <Volunteer*> &getAvailableCollector();
        vector <Volunteer*> &getAvailableDriver();
        vector<Order*> &getInproccessOrders() ;
        vector<Order*> &getPendingOrders() ;
        vector<Order*> &getCompleted();
        vector<Volunteer*> &getVolunteers();
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        int getNumOrders() const;
        int getNumCustomers() const;
        int getNumVolunteers() const;
    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> allOrders;
        vector<Volunteer*>availableDriver;
        vector<Volunteer*> availableCollector;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
};