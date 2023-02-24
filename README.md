# ExchangeSimulator
Simple Exchange simulator with matching engine, handling buy an sell orders (IOC and GFD) using the STL in C++

PriQue: Implements the order book dynamics through an STL priority queue aiming to do the CRUD operations in O(1), in reality the find, update and delete operations can take O(N)
CuPriQue: Implements the order book dynamics through an custom priority queue that extends STL priority queue in an attempt to solve the const reference problem with STL priority queues in order to modify the elements of the queue
Set: Implements the order book dynamics through a set that works as a BST (Binary Search Tree) which keeps the order and can run insertion, modification and deletion in O(logN)
