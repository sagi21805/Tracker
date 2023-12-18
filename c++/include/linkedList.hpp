#ifndef _LinkedList_
#define _LinkedList_

#include "rect.hpp"
#include <memory>

typedef struct Node Node;
typedef cv::Point_<uint16_t> Velocity2D;

/**
 * @file linkedList.hpp
 * @brief The Node of the LinkedList.
 * @paragraph 
 *  The Node of the LinkedList which contains the 
 *  BoundingRect, velocity, and the ptr to the next Node. 
 *  
 * @author Sagi Or
*/  
struct Node{
    
    const Rect rect;
    Velocity2D velocity;
    std::shared_ptr<struct Node> next;

///////////////////////////////////////// Constructors //////////////////////////////////////////////////

    /**
     * @file linkedList.hpp
     * @brief Node constructor from raw data.
     * @author Sagi Or
    */
    Node(Rect rect, Velocity2D velocity): rect(rect), velocity(velocity), next(nullptr) {}

    /**
     * @file linkedList.hpp
     * @brief Node defualt constructor.
     * @author Sagi Or
    */
    Node(): rect(Rect()), next(nullptr) {}

};

/**
 * @file linkedList.hpp
 * @brief The LinkedList class.
 * @paragraph 
 *  The linkedList class contains an easy way to create a linked list 
 *  with a start and end ptr, append and prepend methods, and easy way to access data.
 *  
 * @author Sagi Or
*/  
class LinkedList{

    public: 
        std::shared_ptr<Node> start;
        std::shared_ptr<Node> end;
        size_t length = 0;


///////////////////////////////////////// Constructors //////////////////////////////////////////////////

        /**
         * @file linkedList.cpp
         * @brief LinkedList constructor from raw data.
         * @author Sagi Or
        */
        LinkedList(Rect rect, Velocity2D velocity);

        /**
         * @file linkedList.cpp
         * @brief LinkedList defualt constructor.
         * @author Sagi Or
        */
        LinkedList();

        /**
         * @file linkedList.cpp
         * @brief Gives access to items in the List.
         * @param __i The index of the item to get from the list.
         * @returns Reference to the node in the given index.
         * @author Sagi Or
        */
        Node& getItem(size_t __i);

        /**
         * @file linkedList.cpp
         * @brief appends (adds to end) item to the end of the list.
         * @param __item The item to append.
         * @author Sagi Or
        */
        void append(Node __item);

        /**
         * @file linkedList.cpp
         * @brief Removes an item in a ceratin index.
         * @param __i The index to remove the item from.
         * @author Sagi Or
        */
        void remove(size_t __i);

        /**
         * @file linkedList.cpp
         * @brief prepends (adds to start) item to the end of the list.
         * @param __item The item to prepend.
         * @author Sagi Or
        */
        void prepend(Node __item);

        /**
         * @file linkedList.cpp
         * @brief Insert certain item in a certain index.
         * @param __i The index of the item to get from the list.
         * @param __item The item to insert.
         * @returns Reference to the node in the given index.
         * @author Sagi Or
        */
        void insert(size_t  __i, Node __item);

        /**
         * @file linkedList.cpp
         * @brief Gives access to items in the List.
         * @param __i The index of the item to get from the list.
         * @returns Reference to the node in the given index.
         * @author Sagi Or
        */
        Node& operator[](size_t __i);

};

#endif