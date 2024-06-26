#include "../include/Action.h"  


CustomerType getCustomerType(string customerType)
{
     if(customerType == "soldier")
        return CustomerType::Soldier;
    else
        return CustomerType::Civilian;
}

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

string BaseAction::getStatusString() const
{
   switch (getStatus()) 
   {
      case ActionStatus::COMPLETED: return "COMPLETED";
      case ActionStatus::ERROR: return "ERROR";
      default: return "Unknown";
   }
}

void BaseAction::setStatus(ActionStatus status)
{
   this->status = status;
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
   vector<Order*> &pending = wareHouse.getPendingOrders();
   vector<Order*> &inProccess = wareHouse.getInproccessOrders();
   vector<Order*> &completed = wareHouse.getCompleted();
   vector<Volunteer*> &volunteers = wareHouse.getVolunteers();
   for(int i{}; i < numOfSteps; ++i) {

      //assigning volunteers to orders
      for(vector<Order*>::iterator it = pending.begin(); it != pending.end();){
         bool accepted{false};
         for(size_t j{}; j < volunteers.size() && !accepted; ++j){
            Volunteer *volunteer{volunteers[j]};
            Order& order = *(*it);
            if(volunteer && volunteer->canTakeOrder(order)){
               accepted = true;
               volunteer->acceptOrder(order);
               order.update(volunteer->getId());
               inProccess.push_back(&order);
               it = pending.erase(it);
            }
         }
         if(!accepted)
            ++it;
      }

      //doing step for every volunteer. if he finished is job, the order will
      for (Volunteer* volunteer : volunteers) {
         if(volunteer){
            volunteer->step();
            if(volunteer->getFinishedNow()){
               int id = volunteer->getCompletedOrderId();
               Order &order = wareHouse.getOrder(id);
               order.setfinish(true);
               if(order.getStatus() == OrderStatus::COLLECTING)
                  pending.push_back(&order);
               else if(order.getStatus() == OrderStatus::DELIVERING){
                  completed.push_back(&order);
                  order.update(0);
               }
            }
         }
      }

      for(vector<Order*>::iterator it = inProccess.begin(); it != inProccess.end();){
         if((*it)->getfinish()){
            (*it)->setfinish(false);
            it = inProccess.erase(it);
         }
         else
            ++it;
      }

      //deleting limited volunteers that ended their orders
      for(vector<Volunteer*>::iterator it = volunteers.begin(); it != volunteers.end(); ++it){
         Volunteer* volunteer{*it};
         if (volunteer && !(volunteer->hasOrdersLeft()) && volunteer->getActiveOrderId() == NO_ORDER){
            delete volunteer;
            *it = nullptr;
         }
      }
   }
}

SimulateStep * SimulateStep::clone() const
{
   SimulateStep * clone {new SimulateStep(numOfSteps)};
   clone->error(getErrorMsg());
   clone->setStatus(getStatus());
   return clone;
}


std::string SimulateStep::toString() const
{
   stringstream ss;
   ss << "simulateStep " << numOfSteps << " " << getStatusString();
   return ss.str();
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
   if(customerId < wareHouse.getNumCustomers()){
      Customer& customer {wareHouse.getCustomer(customerId)};
      if (customer.canMakeOrder()) {
         int distance {customer.getCustomerDistance()};
         int numOrder {wareHouse.getNumOrders()};
         int id {customerId};
         Order* order {new Order(numOrder,id,distance)};
         wareHouse.addOrder(order);
         customer.addOrder(order->getId());
         complete();
      }
      else{
         error("Error: Cannot place this order, the customer can't make any more orders");
         cout << getErrorMsg() << endl;
      }
   }
   else{
      error("Error: Cannot place this order, the customer doesn't exist");
      cout << getErrorMsg() << endl;
   }
}

AddOrder * AddOrder::clone() const
{
   AddOrder * clone {new AddOrder(customerId)};
   clone->error(getErrorMsg());
   clone->setStatus(getStatus());
   return clone;
}

string AddOrder::toString() const
{
   stringstream ss;
   ss << "order " << customerId << " " <<  getStatusString();
   return ss.str();
}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------AddCustomer--------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders):
BaseAction(), customerName{customerName}, customerType{getCustomerType(customerType)}, distance{distance},maxOrders{maxOrders}
{}

void AddCustomer::act(WareHouse &wareHouse){
   if(getErrorMsg() != "The customer type you entered doesn't exist"){
      wareHouse.addAction(this);
      if(customerType == CustomerType::Soldier){
         Customer *customer{new SoldierCustomer(wareHouse.getNumCustomers(), customerName ,distance, maxOrders)};
         wareHouse.addCustomer(customer);
         complete();
      }
      else {
         Customer *customer{new CivilianCustomer(wareHouse.getNumCustomers(), customerName ,distance, maxOrders)};
         wareHouse.addCustomer(customer);
         complete();
      }
   }
}


AddCustomer *AddCustomer:: clone() const{
   string customerType = getcustomrType();
   AddCustomer * clone {new AddCustomer (customerName,customerType ,distance,maxOrders)};
   clone->error(getErrorMsg());
   clone->setStatus(getStatus());
   return clone;

}

string AddCustomer:: toString() const 
{
   stringstream ss;
   ss << "customer " << customerName << " " << getcustomrType() << " " << distance << " " << maxOrders;
   return ss.str();
}

string AddCustomer::getcustomrType() const
{
   switch (customerType) 
   {
      case CustomerType::Soldier: return "Soldier";
      case CustomerType::Civilian: return "Civilian";
      default: return "Unknown";
   }
}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------PrintOrderStatus---------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

PrintOrderStatus::PrintOrderStatus(int id):orderId{id}
{}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
   wareHouse.addAction(this);
   if(orderId < wareHouse.getNumOrders() && orderId > -1){
      Order * order = &(wareHouse.getOrder(orderId));
      cout << "--------------------------------------------" << endl;
      cout << order->toString() << endl;
      complete();
   }
   else
   {
      error("Error: Order doesn't exist");
      cout << getErrorMsg() << endl;
   }
}
         
PrintOrderStatus * PrintOrderStatus::clone() const
{
   PrintOrderStatus* clone {new PrintOrderStatus (orderId)};
   clone->error(getErrorMsg());
   clone->setStatus(getStatus());
   return clone;
   
}

string PrintOrderStatus::toString() const 
{ 
   stringstream ss;
   ss << "orderStatus " << orderId << " " << getStatusString();
   return ss.str();
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
   wareHouse.addAction(this);
   if(customerId < wareHouse.getNumCustomers() && customerId > -1){
      cout << "--------------------------------------------" << endl;
      Customer &customer = wareHouse.getCustomer(customerId);
      cout << "CustomerID: " << customer.getId() << endl;
      const vector<int> & customerOrders {customer.getOrdersIds()};
      for(int orderID : customerOrders){
         cout << "--------------------------------------------" << endl;
         cout << "OrderID: " << orderID << endl;
         cout << "OrderStatus: " << (wareHouse.getOrder(orderID)).getStatusString() << endl;

      }
      cout << "numOrdersLeft: " << customer.getMaxOrders() - customerOrders.size() << endl;
      complete();
   }
   else
   {
      error("Error: Customer doesn't exist");
      cout << getErrorMsg() << endl;
   }
}

PrintCustomerStatus * PrintCustomerStatus::clone() const
{
   PrintCustomerStatus * clone{new PrintCustomerStatus(customerId)};
   clone->error(getErrorMsg());
   clone->setStatus(getStatus());
   return clone;
}

string PrintCustomerStatus::toString() const
{
   stringstream ss;
   ss << "customerStatus " << customerId << " " << getStatusString();
   return ss.str();
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
   wareHouse.addAction(this);
   if(VolunteerId < wareHouse.getNumVolunteers() && VolunteerId > -1){
      cout << "--------------------------------------------" << endl;
      Volunteer &volunteer = wareHouse.getVolunteer(VolunteerId);
      if(volunteer.getId() != -1){
         cout << volunteer.toString() << endl;
         complete();
      }
      else{
         error("Error: Volunteer doesn't exist");
         cout << getErrorMsg() << endl;
      }
   }
   
   else
   {
      error("Error: Volunteer doesn't exist");
      cout << getErrorMsg() << endl;
   }
}

PrintVolunteerStatus * PrintVolunteerStatus::clone() const
{
   PrintVolunteerStatus * clone {new PrintVolunteerStatus(VolunteerId)};
   clone->error(getErrorMsg());
   clone->setStatus(getStatus());
   return clone;
}

string PrintVolunteerStatus::toString() const
{
   stringstream ss;
   ss << "volunteerStatus " << VolunteerId << " " << getStatusString();
   return ss.str();
}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------PrintActionsLog----------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(WareHouse &wareHouse)
{
   cout << "--------------------------------------------" << endl;
   const vector<BaseAction *> & actions {wareHouse.getActions()};
   for(BaseAction * action: actions)
      cout << action->toString() << endl;

   wareHouse.addAction(this);
   complete();
}

PrintActionsLog * PrintActionsLog::clone() const
{
   PrintActionsLog * clone {new PrintActionsLog()};
   clone->error(getErrorMsg());
   clone->setStatus(getStatus());
   return clone;
}

string PrintActionsLog::toString() const
{
   stringstream ss;
   ss << "log " << getStatusString();
   return ss.str();
}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------Close--------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

        
Close::Close(){}
        
void Close::act(WareHouse &wareHouse)
{
   wareHouse.addAction(this);
   cout << "--------------------------------------------" << endl;
   for(int i{}; i < wareHouse.getNumOrders(); ++i){
      cout << (wareHouse.getOrder(i)).toString() << endl << endl;
      cout << "--------------------------------------------" << endl;
   }
   complete();
   wareHouse.close();
}
        
Close * Close::clone() const
{
   Close * clone {new Close()};
   clone->error(getErrorMsg());
   clone->setStatus(getStatus());
   return clone;
}
        
string Close::toString() const
{
   stringstream ss;
   ss << "close " << getStatusString();
   return ss.str();
}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------BackupWareHouse----------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

        
BackupWareHouse::BackupWareHouse(){}
        
void BackupWareHouse::act(WareHouse &wareHouse)
{
   if (backup == nullptr)
      backup = new WareHouse{wareHouse};
   else
      (*backup) = wareHouse;
   wareHouse.addAction(this);
   complete();
}
        
BackupWareHouse * BackupWareHouse::clone() const
{
   BackupWareHouse * clone {new BackupWareHouse()};
   clone->error(getErrorMsg());
   clone->setStatus(getStatus());
   return clone;
}
        
string BackupWareHouse::toString() const
{
   stringstream ss;
   ss << "backup " << getStatusString();
   return ss.str();
}

/*
-----------------------------------------------------------------------------------------------------------------------
----------------------------------RestoreWareHouse---------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------
*/

        
RestoreWareHouse::RestoreWareHouse(){}
        
void RestoreWareHouse::act(WareHouse &wareHouse)
{
   if(backup){
      wareHouse = *backup;
      wareHouse.addAction(this);
      complete();
   }
   else{
      wareHouse.addAction(this);
      error("Error: No backup available");
      cout << getErrorMsg() << endl;
   }
}
        
RestoreWareHouse * RestoreWareHouse::clone() const
{
   RestoreWareHouse * clone {new RestoreWareHouse()};
   clone->error(getErrorMsg());
   clone->setStatus(getStatus());
   return clone;
}
        
string RestoreWareHouse::toString() const
{
   stringstream ss;
   ss << "restore " << getStatusString();
   return ss.str();
}
