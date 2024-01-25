#include "../include/Action.h"
#include <iostream>   


/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------BaseAction---------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

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

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------SimulateStep-------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/
    
SimulateStep::SimulateStep(int numOfSteps) :
BaseAction(), numOfSteps{numOfSteps}
{}

void SimulateStep::act(WareHouse &wareHouse)
{
   wareHouse.addAction(this);
   vector<Volunteer*> &availableDriver=wareHouse.getAvailableDriver();
   vector<Volunteer*> &availableCollector=wareHouse.getAvailableCollector();
   vector<Order*> &pending=wareHouse.getPendingOrders();
   vector<Order*> &inProccess=wareHouse.getInproccessOrders();
   vector<Order*> &comleted=wareHouse.getCompleted();
   vector<Volunteer*>volunteers= wareHouse.getVolunteers();
   while (!pending.empty() && !availableCollector.empty()) {
            Order *order = pending.back();
            pending.pop_back();
            Volunteer *volunteer = availableCollector.back();
            availableCollector.pop_back();
            volunteer->acceptOrder(*order);
            inProccess.insert(order);
}





   for(Volunteer *volunteer:volunteers){
      volunteer->step();
      if(!volunteer->canMakeOrder())
      


   }

std::string SimulateStep::toString() const
{

}

SimulateStep * SimulateStep::clone() const
{

}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------AddOrder-----------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

AddOrder::AddOrder(int id) : BaseAction::BaseAction(), customerId{id}
{}

void AddOrder::act(WareHouse &wareHouse)
{
   wareHouse.addAction(this);
   if(customerId <= wareHouse.getNumCustomers()){
      Customer& customer {wareHouse.getCustomer(customerId)};
      if (customer.canMakeOrder()) {
         int distance {customer.getCustomerDistance()};
         int numOrder {wareHouse.getNumOrders()};
         int id {customerId};
         Order* order {new Order(numOrder,id,distance)};
         wareHouse.addOrder(order);
         BaseAction::complete();
      }
      else{
         BaseAction::error("Cannot place this order, the customer can't make any more orders");
      }
   }
   else{
      BaseAction::error("Cannot place this order, the customer doesn't exist");
   }
}

AddOrder * AddOrder::clone() const
{
   return (new AddOrder(customerId));
}

string AddOrder::toString() const
{
}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------AddCustomer--------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders):
BaseAction(), customerName{customerName}, customerType{}, distance{distance},maxOrders{maxOrders}
{ 
   if (customerType == "soldier") {
      const CustomerType customerType = CustomerType::Soldier;
   } 
   else if (customerType == "civilian"){
      const CustomerType customerType = CustomerType::Civilian; 
   }
   else{

   }
};

void AddCustomer::act(WareHouse &wareHouse){
   wareHouse.addAction(this);
   if(customerType == CustomerType::Soldier){
      Customer *customer{new SoldierCustomer(wareHouse.getNumCustomers(), customerName ,distance, maxOrders)};
      wareHouse.addCustomer(customer);
   }
   else{
      Customer *customer{new CivilianCustomer(wareHouse.getNumCustomers(), customerName ,distance, maxOrders)};
      wareHouse.addCustomer(customer);
   }
}


AddCustomer *AddCustomer:: clone() const{
   string customerType = getcustomrType();
   return new AddCustomer (customerName,customerType ,distance,maxOrders);

}

string AddCustomer:: toString() const 
{

}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------PrintOrderStatus---------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

PrintOrderStatus:: PrintOrderStatus(int id):orderId{id}
{
   
}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
   wareHouse.addAction(this);
   Order &order = wareHouse.getOrder(orderId);
   order.toString();
}
         
PrintOrderStatus * PrintOrderStatus::clone() const
{
}

std::string PrintOrderStatus::toString() const 
{ 

}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------PrintCustomerStatus------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

PrintCustomerStatus::PrintCustomerStatus(int customerId): customerId{customerId}
{}

void PrintCustomerStatus::act(WareHouse &wareHouse) 
{

}

PrintCustomerStatus * PrintCustomerStatus::clone() const
{

}

string PrintCustomerStatus::toString() const
{

}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------PrintVolunteerStatus-----------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

PrintVolunteerStatus::PrintVolunteerStatus(int id): VolunteerId{id}
{}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{

}

PrintVolunteerStatus * PrintVolunteerStatus::clone() const
{

}

string PrintVolunteerStatus::toString() const
{

}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------PrintActionsLog----------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(WareHouse &wareHouse)
{

}

PrintActionsLog * PrintActionsLog::clone() const
{

}

string PrintActionsLog::toString() const
{

}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------Close--------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

        
Close::Close(){}
        
void Close::act(WareHouse &wareHouse)
{

}
        
Close * Close::clone() const
{

}
        
string Close::toString() const
{

}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------BackupWareHouse----------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

        
BackupWareHouse::BackupWareHouse(){}
        
void BackupWareHouse::act(WareHouse &wareHouse)
{

}
        
BackupWareHouse * BackupWareHouse::clone() const
{

}
        
string BackupWareHouse::toString() const
{

}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------RestoreWareHouse---------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

        
RestoreWareHouse::RestoreWareHouse(){}
        
void RestoreWareHouse::act(WareHouse &wareHouse)
{

}
        
RestoreWareHouse * RestoreWareHouse::clone() const
{

}
        
string RestoreWareHouse::toString() const
{

}
