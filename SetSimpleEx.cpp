#include <iostream>
#include <bits/stdc++.h>

using namespace std;

/*
 Class to store the data (fields) and actions (methods) related to one order
 as well as getters and setters exposing only the necessary fields
 getters are implemented as constant functions to avoid unwanted modification of fields
 */
class Order
{
private:
    /*
      Fields
      orderID: ID of the order, can be any sequence of printable characters
      orderType: Type of the order, can be BUY or SELL
      executionType: Execution type of the order, can be GFD (Good for Day) or IOC (Insert or Cancel)
      priceBid: Integer value representing the price bid related to the order
      quantity: Integer value representing the quantity of the order
      turn: Turn in wich the order was inserted, is common for both BuyBook and SellBook
     */
    string orderID;
    string orderType;
    string executionType;
    int priceBid;
    int quantity;
    int turn;
public:
    /*
     Default constructor
     Initialize with initializer list
     */
    Order():orderID(""),orderType(""),executionType(""),priceBid(0),quantity(0),turn(0){};
    /*
     Function to create order with user defined parameters
     @param ordID: ID of the order, can be any sequence of printable characters
     @param ordType: Type of the order, can be BUY or SELL
     @param execType: Execution type of the order, can be GFD (Good for Day) or IOC (Insert or Cancel)
     @param priceB: Integer value representing the price bid related to the order
     @param qty: Integer value representing the quantity of the order
     */
    void createOrder(string ordID, string ordType, string execType, int priceB,int qty,int tn)
    {
        orderID=ordID;
        orderType=ordType;
        executionType=execType;
        priceBid=priceB;
        quantity=qty;
        turn=tn;
    }
    /*
     Function to modify an existing order with user defined parameters
     @param ordType: Type of the order, can be BUY or SELL
     @param priceB: Integer value representing the price bid related to the order
     @param qty: Integer value representing the quantity of the order
     @param tn: Integer value representing the turn the order was inserted
     */
    void modifyOrder(string ordType, int priceB, int qty, int tn)
    {
        orderType=ordType;
        priceBid=priceB;
        quantity=qty;
        turn=tn;
    }
    /*
     Function to get the value of field OrderID
     */
    string getOrderID() const
    {
        return orderID;
    }
    /*
     Function to get the value of field OrderType
     */
    string getOrderType() const
    {
        return orderType;
    }
    /*
     Function to get the value of field Price Bid
     */
    int getPriceBid() const
    {
        return priceBid;
    }
    /*
     Function to get the value of field Quantity
     */
    int getQuantity() const
    {
        return quantity;
    }
    /*
     Function to get the value of field Turn
     */
    int getTurn() const
    {
        return turn;
    }
    /*
     Function to set the value of field Quantity
     */
    void setQuantity(int qty)
    {
        quantity=qty;
    }
};

/*
  Overload operator less than, is used to provide order when using different STL containers like maps, sets, and priority queues,
  Orders are compared based first on priceBid, and second on turn
  @param order1: Order at the rhs of less than < operator. Declared as constant to avoid unwanted modification and as reference to avoid
  copying the original object
  @param order2: Order at the lhs of less than < operator. Declared as constant to avoid unwanted modification and as reference to avoid
  copying the original object
 */
bool operator<(const Order& order1, const Order& order2)
{
    if(order1.getPriceBid()==order2.getPriceBid())
    {
        return order1.getTurn()<order2.getTurn();
    }
    return order1.getPriceBid() < order2.getPriceBid();
}
/*
  Overload operator greater than, is used to provide order when using different STL containers like maps, sets, and priority queues
  Orders are compared based first on priceBid, and second on turn
  @param order1: Order at the rhs of greater than > operator. Declared as constant to avoid unwanted modification and as reference to avoid copying the original object
 @param order2: Order at the lhs of greater than > operator. Declared as constant to avoid unwanted modification and as reference to avoid copying the original object
 */
bool operator>(const Order& order1, const Order& order2)
{
    if(order1.getPriceBid()==order2.getPriceBid())
    {
        return order1.getTurn()<order2.getTurn();
    }
    return order1.getPriceBid() > order2.getPriceBid();
}
/*
 Overload operator equal to, is used when using different STL algorithms like find and erase
 @param order1: Order at the rhs of greater than > operator. Declared as constant to avoid unwanted modification and as reference to avoid copying the original object
 @param order2: Order at the lhs of greater than > operator. Declared as constant to avoid unwanted modification and as reference to avoid copying the original object
 */

bool operator==(const Order& order1, const Order& order2)
{
    return order1.getOrderID() == order2.getOrderID();
}

/*
 Class to works as a handler for both buyPriority queue and sellPriority queue
 buyPriority queue is implemented as a decreasing order set recreating the functionality of a maximum heap
 sellPriority queue is implemented as a increasing order set recreating the functionality of a minimum heap
 for performance reasons both buyPriority and sellPriority are implemented as set which internally are implemented as Binary Search Trees (BST)
 BST have insert, delete, and find operations in O(logN)
 Priority Queues also known as heaps have insert and delete from the top in O(1) but when deleting and finding elements from the middle or the back the worst case scenario is O(N) for that reason I didnt used this data structure
 */

class QueueHandler
{
private:
    /*
     Fields
     buyPriority decreasing order set recreating the functionality of a maximum heap for storing the buy orders in the OrderBook with insert, delete, and find operations in O(logN)
     sellPriority increasing order set recreating the functionality of a minimum heap for storing the sell orders in the OrderBook with insert, delete, and find operations in O(logN)
     */
    set<Order,greater<Order>> buyPriority;
    set<Order> sellPriority;
public:
    /*
     Function to insert element at buyPriority maximum heap
     @param currOrder: Order object reference to be inserted in buyPriority, pass by reference to avoid copying
     */
    void addToBuyQueue(Order &currOrder)
    {
        buyPriority.insert(currOrder);
    }
    /*
     Function to insert element at sellPriority minimum heap
     @param currOrder: Order object reference to be inserted in sellPriority, pass by reference to avoid copying
     */
    void addToSellQueue(Order &currOrder)
    {
        sellPriority.insert(currOrder);
    }
    /*
     Function to delete element from both priority queues, deciding to call the proper function depending on the orderType
     @param currOrder: Order object reference to be deleted in the queue, pass by reference to avoid copying
     */
    void deleteFromQueue(Order &currOrder)
    {
        string orderType=currOrder.getOrderType();
        if(orderType=="BUY")
        {
            Delete(buyPriority,currOrder);
        }
        else if(orderType=="SELL")
        {
            Delete(sellPriority,currOrder);
        }
    }
    /*
     Overloaded Function to delete element from sellPriority queue
     @param currOrder: Order object reference to be deleted in the sellPriority queue, pass by reference to avoid copying
     */
    void Delete(set<Order> &currQueue,Order &currOrder)
    {
        currQueue.erase(currOrder);
    }
    /*
     Overloaded Function to delete element from buyPriority queue
     @param currOrder: Order object reference to be deleted in the buyPriority queue, pass by reference to avoid copying
     */
    void Delete(set<Order,greater<Order>> &currQueue,Order &currOrder)
    {
        currQueue.erase(currOrder);
    }
    friend class OrderBook;
};

/*
 Class to represent an OrderBook, handling the order book actions (methods) such as insert, delete, cancel, modify,and update(reminder) order. It also handles trades (matches) and prints the reminding orders in the order book as well as the trades. It assigns a turn to each order in both orderBooks, this value is used when printing the trades to know which order came first
 */

class OrderBook
{
private:
    /*
     Fields
     orderMap: unordered_map of all orders buy and sell orders. Internally unordered_maps work as hash tables, which are great for storing unordered records with insert, delete, and find operations in O(1)
     turn: variable general to all the order book that assigns turns to all orders, buy and sell orders. This value is used when determing print order between buy and sell orders
     QH: object of class Queue Handler that works as described above
     */
    unordered_map<string, Order> orderMap;
    int turn=0;
    QueueHandler QH;
    
public:
    /*
     Function to insert element at OrderBook checking if all the parameters passed are valid, it distinguishes between GFD and IOC execution types since they have different behaviours and life spans. It inserts the order in the hash map and creates the order with the input parameters
     @param orderType: Type of the order, can be BUY or SELL
     @param executionType: Execution type of the order, can be GFD (Good for Day) or IOC (Insert or Cancel)
     @param priceBid: Integer value representing the price bid related to the order
     @param quantity: Integer value representing the quantity of the order
     @param orderID: ID of the order, can be any sequence of printable characters
     */
    void InsertOrder(string orderType,string executionType,int priceBid,int quantity,string orderID)
    {
        if(orderID!="" && priceBid>0 && quantity>0 && (orderType=="BUY"||orderType=="SELL") && (executionType=="IOC"||executionType=="GFD"))
        {
            orderMap[orderID].createOrder(orderID,orderType,executionType,priceBid,quantity,++turn);
            if(executionType=="GFD")
            {
                InsertOrderGFD(orderID);
            }
            else if(executionType=="IOC")
            {
                InsertOrderIOC(orderID);
            }
        }
    }
    /*
     Function to insert Good For Day order at corresponding buy or sell priority queue, the order remains there until traded. After inserting any order it checks the order book for a match (trade)
     @param orderID: ID of the order, can be any sequence of printable characters
     */
    void InsertOrderGFD(string orderID)
    {
        string orderType=orderMap[orderID].getOrderType();
        if(orderType=="BUY")
        {
            QH.addToBuyQueue(orderMap[orderID]);
        }
        else if(orderType=="SELL")
        {
            QH.addToSellQueue(orderMap[orderID]);
        }
        Match();
    }
    /*
     Function to insert Immediate or Cancel order at corresponding buy or sell priority queue, the order requests to be traded (if possible) immediately, then its canceled. It gets removed from the corresponding priority queue and then from the hash table
     @param orderID: ID of the order, can be any sequence of printable characters
     */
    void InsertOrderIOC(string orderID)
    {
        string orderType=orderMap[orderID].getOrderType();
        if(orderType=="BUY")
        {
            QH.addToBuyQueue(orderMap[orderID]);
            if(orderID==QH.buyPriority.begin()->getOrderID())
            {
                Match();
            }
        }
        else if(orderType=="SELL")
        {
            QH.addToSellQueue(orderMap[orderID]);
            if(orderID==QH.sellPriority.begin()->getOrderID())
            {
                Match();
            }
        }
        QH.deleteFromQueue(orderMap[orderID]);
        orderMap.erase(orderID);
    }
    /*
     Function to modify element at OrderBook checking if all the parameters passed are valid. It searches the order in the hash map in O(1) and proceed to modifying the order if it is found. It deletes the order from the priority queue since the turn based priority will be lost, then modifies the parameters of the order , and finally inserts the new order into the corresponging queue
     @param orderID: ID of the order, can be any sequence of printable characters
     @param orderType: Type of the order, can be BUY or SELL
     @param priceBid: Integer value representing the price bid related to the order
     @param quantity: Integer value representing the quantity of the order
     */
    void ModifyOrder(string orderID,string orderType,int priceBid,int quantity)
    {
        auto fitr=orderMap.find(orderID);
        if(fitr!=orderMap.end())
        {
            if(priceBid>0 && quantity>0 && (orderType=="BUY"||orderType=="SELL"))
            {
                QH.deleteFromQueue(fitr->second);
                orderMap[orderID].modifyOrder(orderType,priceBid,quantity,++turn);
                if(orderType=="BUY")
                {
                    QH.addToBuyQueue(orderMap[orderID]);
                }
                else if(orderType=="SELL")
                {
                    QH.addToSellQueue(orderMap[orderID]);
                }
                Match();
            }
        }
    }
    /*
     Function to update an orders quantity in the order book and priority queue. It is called when an order that was traded has a reminder quantity, this order needs to be updated with the new quantity, but wont loose the turn priority, and the other parameters dont get modified.
     @param orderID: ID of the order, can be any sequence of printable characters
     @param newQuantity: Integer value representing the quantity of the order
     */
    void UpdateOrder(string orderID,int newQuantity)
     {
        orderMap[orderID].setQuantity(newQuantity);
        string orderType=orderMap[orderID].getOrderType();
        QH.deleteFromQueue(orderMap[orderID]);
        if(orderType=="BUY")
        {
            QH.addToBuyQueue(orderMap[orderID]);
        }
        else if(orderType=="SELL")
        {
            QH.addToSellQueue(orderMap[orderID]);
        }
     }
    /*
     Function to cancel an order in the order book and priority queue. It searches the order in the hash map in O(1) and proceed to cancel the order if it is found deleting it from the priority queue and erasing it from the hash table
     @param orderID: ID of the order, can be any sequence of printable characters
     @param newQuantity: Integer value representing the quantity of the order
     */
    void CancelOrder(string orderID)
    {
        auto fitr=orderMap.find(orderID);
        if(fitr!=orderMap.end())
        {
            QH.deleteFromQueue(fitr->second);
            orderMap.erase(fitr);
        }
    }
    /*
     Function to check for matches in the orderbook, it compares than neither priority queue is empty and then compares the top most element of each queue (buy and sell) everytime the buy price is greater than or equal to the sell price, it crosses a match and does a trade. It calculates the minimum quantity between the buy quantity and sell quantity since that will be the traded quantity, and prints the TRADE message. Then it calculates the reminder quantity as the difference between buy and sell traded orders, it updates the order with the greater quantity with the reminder, and deletes the other order. If both orders have the same quantity both get deleted. It keeps doing this process until no trades can be made, this function gets called everytime an order is inserted
     */
    void Match()
    {
        bool keepMatching=true;
        while(keepMatching)
        {
            if((!QH.buyPriority.empty()&&!QH.sellPriority.empty())&&(QH.buyPriority.begin()->getPriceBid()>=QH.sellPriority.begin()->getPriceBid()))
            {
                int buyQty=QH.buyPriority.begin()->getQuantity();
                int sellQty=QH.sellPriority.begin()->getQuantity();
                int minQty=min(buyQty,sellQty);
                PrintMatch(minQty);
                if(buyQty!=sellQty)
                {
                    int nwqty=0;
                    if(buyQty>sellQty)
                    {
                        nwqty=buyQty-sellQty;
                        UpdateOrder(QH.buyPriority.begin()->getOrderID(),nwqty);
                        CancelOrder(QH.sellPriority.begin()->getOrderID());
                    }
                    else
                    {
                        nwqty=sellQty-buyQty;
                        UpdateOrder(QH.sellPriority.begin()->getOrderID(),nwqty);
                        CancelOrder(QH.buyPriority.begin()->getOrderID());
                    }
                }
                else
                {
                    CancelOrder(QH.buyPriority.begin()->getOrderID());
                    CancelOrder(QH.sellPriority.begin()->getOrderID());
                }
            }
            else
            {
                keepMatching=false;
            }
        }
    }
    /*
     Function to print the output of a trade. It is called everytime a trade happens quantity, It assigns the printing order depending on the turn and outputs the TRADE message
     @param minQty: Minimum quantity between the buy quantity and sell quantity since that is the traded quantity
     */
    void PrintMatch(int minQty)
    {
        int buyTurn= orderMap[QH.buyPriority.begin()->getOrderID()].getTurn();
        int sellTurn= orderMap[QH.sellPriority.begin()->getOrderID()].getTurn();
        if(buyTurn<sellTurn)
        {
            cout<<"TRADE " + QH.buyPriority.begin()->getOrderID() + " "<<QH.buyPriority.begin()->getPriceBid()<<" "<<minQty<<" " + QH.sellPriority.begin()->getOrderID() + " "<<QH.sellPriority.begin()->getPriceBid()<<" "<<minQty<<endl;
        }
        else
        {
            cout<<"TRADE " + QH.sellPriority.begin()->getOrderID() + " "<<QH.sellPriority.begin()->getPriceBid()<<" "<< minQty<<" " + QH.buyPriority.begin()->getOrderID() + " "<<QH.buyPriority.begin()->getPriceBid()<<" "<< minQty<<endl;
        }
    }
    /*
     Function to print the OrderBook, buy and sell priority queues, it decides what function to call depending if buy or sell
     */
    void PrintOrderBook()
    {
        cout<<"SELL: "<<endl;
        PrintSell(QH.sellPriority);
        cout<<"BUY: "<<endl;
        PrintBuy(QH.buyPriority);
    }
    /*Overloaded Function to print from buyPriority queue, it creates a decreasing order map to store the values in the buyPriority queue and add up the quantities with the same price bid. It passes the value by reference to avoid copying
    @param currQueue: buyPriority queue
    */
    void PrintBuy(set<Order,greater<Order>> &currQueue)
    {
        map<int,int,greater<int>> temp;
        for(Order currOrder:currQueue)
        {
            temp[currOrder.getPriceBid()]+=currOrder.getQuantity();
        }
        for(auto itr=temp.begin();itr!=temp.end();itr++)
        {
            cout<<itr->first<<" "<<itr->second<<endl;
        }
    }
    /*Overloaded Function to print from sellPriority queue, it creates a increasing order map to store the values in the sellPriority queue and add up the quantities with the same price bid. It passes the value by reference to avoid copying
    @param currQueue: sellPriority queue
    */
    void PrintSell(set<Order> &currQueue)
    {
        map<int,int,greater<int>> temp;
        for(Order currOrder:currQueue)
        {
            temp[currOrder.getPriceBid()]+=currOrder.getQuantity();
        }
        for(auto itr=temp.begin();itr!=temp.end();itr++)
        {
            cout<<itr->first<<" "<<itr->second<<endl;
        }
    }
};

/*
 Class to handle user input, it has an OrderBook as a parameter and calls different order book functionalities depending on the user input
 */
class InputMachine
{
private:
    /*
     Fields
     OB: The order book that will be used in the application
     */
    OrderBook OB;
public:
    /* Funtion that depending on the user input, it call a different functionality from the order book, it takes a vector of strings (the space separated sentence provided by the user) and depending on the first letter of the first word determines which functionality of the order book will be called, using a switch
    @param inputLine: Vector of string with the input of the space separated input sentence
    */
    void HandleInput(vector<string> &inputLine)
    {
        char operationToExecute=inputLine[0][0];
        switch(operationToExecute)
        {
            case 'B':
                OB.InsertOrder(inputLine[0],inputLine[1],stoi(inputLine[2]),stoi(inputLine[3]),inputLine[4]);
                break;
            case 'S':
                OB.InsertOrder(inputLine[0],inputLine[1],stoi(inputLine[2]),stoi(inputLine[3]),inputLine[4]);
                break;
            case 'M':
                OB.ModifyOrder(inputLine[1], inputLine[2],stoi(inputLine[3]),stoi(inputLine[4]));
                break;
            case 'C':
                OB.CancelOrder(inputLine[1]);
                break;
            case 'P':
                OB.PrintOrderBook();
                break;
        }
    }
};
int main(int argc, const char * argv[]) {
    
    string input;
    InputMachine IM;
    while(getline(cin, input)){
        vector<string> inputLine;
        stringstream ss(input);
        while (ss.good())
        {
            string inputWord;
            getline(ss, inputWord, ' ');
            inputLine.push_back(inputWord);
        }
       IM.HandleInput(inputLine);
    }
    return 0;
}
