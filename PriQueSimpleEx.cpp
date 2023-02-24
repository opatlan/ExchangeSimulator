#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class Order
{
private:
    string orderID;
    string orderType;
    string executionType;
    int priceBid;
    int quantity;
    int turn;
public:
    Order():orderID(""),orderType(""),executionType(""),priceBid(0),quantity(0),turn(0){};
    void createOrder(string ordID, string ordType, string execType, int priceB,int qty,int tn)
    {
        orderID=ordID;
        orderType=ordType;
        executionType=execType;
        priceBid=priceB;
        quantity=qty;
        turn=tn;
    }
    void modifyOrder(string ordType, int priceB, int qty, int tn)
    {
        orderType=ordType;
        priceBid=priceB;
        quantity=qty;
        turn=tn;
    }
    string getOrderID() const
    {
        return orderID;
    }
    string getOrderType() const
    {
        return orderType;
    }
    int getPriceBid() const
    {
        return priceBid;
    }
    int getQuantity() const
    {
        return quantity;
    }
    int getTurn() const
    {
        return turn;
    }
    void setQuantity(int qty)
    {
        quantity=qty;
    }
};

bool operator<(const Order& order1, const Order& order2)
{
    if(order1.getPriceBid()==order2.getPriceBid())
    {
        return order1.getTurn()>order2.getTurn();
    }
    return order1.getPriceBid() < order2.getPriceBid();
}
bool operator>(const Order& order1, const Order& order2)
{
    if(order1.getPriceBid()==order2.getPriceBid())
    {
        return order1.getTurn()>order2.getTurn();
    }
    return order1.getPriceBid() > order2.getPriceBid();
}

class QueueHandler
{
private:
    priority_queue<Order> buyPriority;
    priority_queue<Order,vector<Order>,greater<Order>> sellPriority;
public:
    void addToBuyQueue(Order &currOrder)
    {
        buyPriority.push(currOrder);
    }
    void addToSellQueue(Order &currOrder)
    {
        sellPriority.push(currOrder);
    }
    
    void deleteFromQueue(Order &currOrder,string orderID)
    {
        string orderType=currOrder.getOrderType();
        if(orderType=="BUY")
        {
            Delete(buyPriority,orderID);
        }
        else if(orderType=="SELL")
        {
            Delete(sellPriority,orderID);
        }
    }

    void Delete(priority_queue<Order> &currQueue,string orderID)
    {
        vector<Order> temp;
        while(!currQueue.empty())
        {
            if(orderID!=currQueue.top().getOrderID())
            {
                temp.push_back(currQueue.top());
                currQueue.pop();
            }
            else
            {
                currQueue.pop();
                break;
            }
        }
        for(Order currOrder:temp)
        {
            currQueue.push(currOrder);
        }
    }
    void Delete(priority_queue<Order,vector<Order>,greater<Order>> &currQueue,string orderID)
    {
        vector<Order> temp;
        while(!currQueue.empty())
        {
            if(orderID!=currQueue.top().getOrderID())
            {
                temp.push_back(currQueue.top());
                currQueue.pop();
            }
            else
            {
                currQueue.pop();
                break;
            }
        }
        for(Order currOrder:temp)
        {
            currQueue.push(currOrder);
        }
    }
    friend class OrderBook;
};

class OrderBook
{
private:
    unordered_map<string, Order> orderMap;
    int turn=0;
    QueueHandler QH;
    
public:
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
    
    void InsertOrderIOC(string orderID)
    {
        string orderType=orderMap[orderID].getOrderType();
        if(orderType=="BUY")
        {
            QH.addToBuyQueue(orderMap[orderID]);
            if(orderID==QH.buyPriority.top().getOrderID())
            {
                Match();
            }
        }
        else if(orderType=="SELL")
        {
            QH.addToSellQueue(orderMap[orderID]);
            if(orderID==QH.sellPriority.top().getOrderID())
            {
                Match();
            }
        }
        QH.deleteFromQueue(orderMap[orderID],orderID);
        orderMap.erase(orderID);
    }
    
    void ModifyOrder(string orderID,string orderType,int priceBid,int quantity)
    {
        auto fitr=orderMap.find(orderID);
        if(fitr!=orderMap.end())
        {
            if(priceBid>0 && quantity>0 && (orderType=="BUY"||orderType=="SELL"))
            {
                QH.deleteFromQueue(fitr->second,orderID);
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
    
    void UpdateOrder(string orderID,int newQuantity)
     {
        orderMap[orderID].setQuantity(newQuantity);
        string orderType=orderMap[orderID].getOrderType();
        QH.deleteFromQueue(orderMap[orderID],orderID);
        if(orderType=="BUY")
        {
            QH.addToBuyQueue(orderMap[orderID]);
        }
        else if(orderType=="SELL")
        {
            QH.addToSellQueue(orderMap[orderID]);
        }
     }
    
    void CancelOrder(string orderID)
    {
        auto fitr=orderMap.find(orderID);
        if(fitr!=orderMap.end())
        {
            QH.deleteFromQueue(fitr->second,orderID);
            orderMap.erase(fitr);
        }
    }
    void Match()
    {
        bool keepMatching=true;
        while(keepMatching)
        {
            if((!QH.buyPriority.empty()&&!QH.sellPriority.empty())&&(QH.buyPriority.top().getPriceBid()>=QH.sellPriority.top().getPriceBid()))
            {
                int buyQty=QH.buyPriority.top().getQuantity();
                int sellQty=QH.sellPriority.top().getQuantity();
                int minQty=min(buyQty,sellQty);
                PrintMatch(minQty);
                if(buyQty!=sellQty)
                {
                    int nwqty=0;
                    if(buyQty>sellQty)
                    {
                        nwqty=buyQty-sellQty;
                        UpdateOrder(QH.buyPriority.top().getOrderID(),nwqty);
                        CancelOrder(QH.sellPriority.top().getOrderID());
                    }
                    else
                    {
                        nwqty=sellQty-buyQty;
                        UpdateOrder(QH.sellPriority.top().getOrderID(),nwqty);
                        CancelOrder(QH.buyPriority.top().getOrderID());
                    }
                }
                else
                {
                    CancelOrder(QH.buyPriority.top().getOrderID());
                    CancelOrder(QH.sellPriority.top().getOrderID());
                }
            }
            else
            {
                keepMatching=false;
            }
        }
    }
    
    void PrintMatch(int minQty)
    {
        int buyTurn= orderMap[QH.buyPriority.top().getOrderID()].getTurn();
        int sellTurn= orderMap[QH.sellPriority.top().getOrderID()].getTurn();
        if(buyTurn<sellTurn)
        {
            cout<<"TRADE " + QH.buyPriority.top().getOrderID() + " "<<QH.buyPriority.top().getPriceBid()<<" "<<minQty<<" " + QH.sellPriority.top().getOrderID() + " "<<QH.sellPriority.top().getPriceBid()<<" "<<minQty<<endl;
        }
        else
        {
            cout<<"TRADE " + QH.sellPriority.top().getOrderID() + " "<<QH.sellPriority.top().getPriceBid()<<" "<< minQty<<" " + QH.buyPriority.top().getOrderID() + " "<<QH.buyPriority.top().getPriceBid()<<" "<< minQty<<endl;
        }
    }
    void PrintOrderBook()
    {
        cout<<"SELL: "<<endl;
        PrintSell(QH.sellPriority);
        cout<<"BUY: "<<endl;
        PrintBuy(QH.buyPriority);
    }
    void PrintSell(priority_queue<Order,vector<Order>,greater<Order>> currQueue)
    {
        map<int,int,greater<int>> temp;
        while(!currQueue.empty())
        {
            temp[currQueue.top().getPriceBid()]+=currQueue.top().getQuantity();
            currQueue.pop();
        }
        for(auto itr=temp.begin();itr!=temp.end();itr++)
        {
            cout<<itr->first<<" "<<itr->second<<endl;
        }
    }
    void PrintBuy(priority_queue<Order> currQueue)
    {
        map<int,int,greater<int>> temp;
        while(!currQueue.empty())
        {
            temp[currQueue.top().getPriceBid()]+=currQueue.top().getQuantity();
            currQueue.pop();
        }
        for(auto itr=temp.begin();itr!=temp.end();itr++)
        {
            cout<<itr->first<<" "<<itr->second<<endl;
        }
    }
};

class InputMachine
{
private:
    OrderBook OB;
public:
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
