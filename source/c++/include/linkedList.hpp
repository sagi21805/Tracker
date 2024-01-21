#ifndef _LinkedList_
#define _LinkedList_

#include <memory>

/**
 * @file linkedList.hpp
 * @brief The LinkedList class.
 * @paragraph
 *  The linkedList class contains an easy way to create a linked list 
 *  with a start and end ptr, append and prepend methods, and an easy way to access data.
 *  
 * @author Sagi Or
*/  

template <class _Type>
struct Node{

    _Type item;
    std::shared_ptr<Node<_Type>> next;

    Node(_Type __item) : item(__item), next(nullptr) {}

    Node() : item(_Type()), next(nullptr) {}

};

template<typename NodeType> 
class LinkedList {

public: 
    std::shared_ptr<Node<NodeType>> start;
    std::shared_ptr<Node<NodeType>> end;
    size_t length = 0;

///////////////////////////////////////// Constructors //////////////////////////////////////////////////

    LinkedList(Node<NodeType> node) {
        std::shared_ptr<Node<NodeType>> newNode = std::make_shared<Node<NodeType>>(node);
        this->start = newNode;
        this->end = newNode;
        this->length++;
    }

    LinkedList() {
        this->start = nullptr;
        this->end = nullptr;
    }

    NodeType& getItem(size_t __i) {
        checkBounds(__i);

        if (__i == thie->length-1){ return this->end->item; }

        std::shared_ptr<Node<NodeType>> ref = this->start;

        for (size_t i = 0; i < __i; i++) {
            ref = ref->next;
        }
        return ref->item;
    }

    void append(NodeType __item) {
        std::shared_ptr<Node<NodeType>> newNode = std::make_shared<Node<NodeType>>(__item); 
        if (end == nullptr){
            this->end = newNode;
            this->start = newNode;
        }

        this->end->next = newNode;
        this->end = newNode;
        this->length++;
    }

    void remove(size_t __i) {
        checkBounds(__i);

        std::shared_ptr<Node<NodeType>> ref = this->start;
        if (__i == 0) {
            this->start = ref->next;
            return;
        }
        for (size_t i = 0; ref != nullptr && i < __i - 1; i++) {
            ref = ref->next;
        }
        std::shared_ptr<Node<NodeType>> next = ref->next->next;
        ref->next = next;
        this->length--;
    }

    void prepend(NodeType __item) {
        std::shared_ptr<Node<NodeType>> newNode = std::make_shared<Node<NodeType>>(__item); 
        if (start == nullptr){
            this->start = newNode;
            this->end = newNode;
        }

        newNode->next = this->start;    
        this->start = newNode;
        this->length++;
    }

    void insert(size_t __i, NodeType __item) {
        checkBounds(__i);

        if (__i == 0) { this->prepend(__item); return;}
        
        if (__i == this->length - 1) { this->append(__item); return; }

        std::shared_ptr<Node<NodeType>> ref = this->start;
        std::shared_ptr<Node<NodeType>> newNode = std::make_shared<Node<NodeType>>(__item); 
        for (size_t i = 0; ref != nullptr && i < __i - 1; i++) {
            ref = ref->next;
        }
        newNode->next = ref->next;
        ref->next = newNode;
        this->length++;
    }

    bool checkBounds(size_t __i){
        if (__i >= this->length) {throw std::runtime_error("Out of Bounds Index is \""+std::to_string(__i)+"\" length is \"" + std::to_string(this->length) + "\""); }
    }

    Node<NodeType>& operator[](size_t __i) {
        return this->getItem(__i);
    }
};

#endif
