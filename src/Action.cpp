#include "../include/Action.h"
#include <iostream>
#include <string>
using std::string;     
BaseAction::BaseAction(): 
errorMsg{}, status{}
{}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
   status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
   BaseAction::errorMsg = errorMsg;
   status = ActionStatus::ERROR;
}

string BaseAction:: getErrorMsg() const
{
    return errorMsg;
}
    
SimulateStep::SimulateStep(int numOfSteps) :
BaseAction(), numOfSteps{numOfSteps}
{}

void SimulateStep::act(WareHouse &wareHouse)
{
}

std::string SimulateStep::toString() const
{

}

SimulateStep * SimulateStep::clone() const
{

}


AddOrder::AddOrder(int id) : BaseAction::BaseAction(), customerId{id}
{}

void AddOrder::act(WareHouse &wareHouse)
{
   wareHouse.addAction(this);
   if(customerId<=wareHouse.getNumCustomer()){
       Customer& customer {wareHouse.getCustomer(customerId)};
       if (customer.canMakeOrder()) {
           int distance {customer.getCustomerDistance()};
           int numOrder {wareHouse.getNumOrder()};
           int id {customerId};
           Order* order {new Order(numOrder,id,distance)};
           wareHouse.addOrder(order);
           BaseAction::complete();
       }}
   else{
      BaseAction::error("Cannot place this order");
   }
}

AddOrder * AddOrder::clone() const
{
   return (new AddOrder(customerId));
}

string AddOrder::toString() const
{
}




AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders)
:BaseAction(), customerName{customerName},
distance{distance},maxOrders{maxOrders}
{ 
   
if (customerType == "soldier") {
      const CustomerType customerType = CustomerType::Soldier;
}
   else {
      const CustomerType customerType = CustomerType::Civilian; 
   }
};
void AddCustomer:: act(WareHouse &wareHouse){
wareHouse.addAction(this);

if(customerType == CustomerType::Soldier) 
{
    Customer *customer{new SoldierCustomer(wareHouse.getNumCustomer(), customerName ,distance, maxOrders)}; 
 }
 else
 {
    Customer *customer{new CivilianCustomer(wareHouse.getNumCustomer(), customerName ,distance, maxOrders)};
 }
};
string AddCustomer::getcustomrType() const
{
   
}
AddCustomer *AddCustomer:: clone() const{
   string customerType=getcustomrType();
    return new AddCustomer (customerName,customerType ,distance,maxOrders);
}
string toString() 
{

};

PrintOrderStatus:: PrintOrderStatus(int id):orderId{id}
{

};
void PrintOrderStatus:: act(WareHouse &wareHouse)
{
    wareHouse.addAction(this);
   Order &order=wareHouse.getOrder(orderId);
    order.toString();
}
 PrintOrderStatus* PrintOrderStatus:: clone() const
 {
   return (new PrintOrderStatus(orderId));
 };
  std::string toString() const 
{ 
        
        };