#ifndef _LinkedList_
#define _LinkedList_

#include <memory>

// Include the necessary header for the Node type
#include "nodes.hpp"

/**
 * @file linkedList.hpp
 * @brief The LinkedList class.
 * @paragraph
 *  The linkedList class contains an easy way to create a linked list 
 *  with a start and end ptr, append and prepend methods, and an easy way to access data.
 *  
 * @author Sagi Or
*/  
template<typename _Node> 
class LinkedList {

public: 
    std::shared_ptr<_Node> start;
    std::shared_ptr<_Node> end;
    size_t length = 0;

///////////////////////////////////////// Constructors //////////////////////////////////////////////////

    LinkedList(_Node node) {
        std::shared_ptr<_Node> newNode = std::make_shared<_Node>(node);
        this->start = newNode;
        this->end = newNode;
        this->length++;
    }

    LinkedList() {
        std::shared_ptr<_Node> newNode = std::make_shared<_Node>();
        this->start = newNode;
        this->end = newNode;
        this->length++;
    }

    _Node& getItem(size_t __i) {
        if (__i > this->length) {
            throw std::runtime_error("Out of Bounds Index is \"" + std::to_string(__i) + "\" and the length is \"" + std::to_string(this->length) + "\"");
        }
        std::shared_ptr<_Node> ref = this->start;

        for (size_t i = 0; i < __i; i++) {
            ref = ref->next;
        }
        return *ref;
    }

    void append(_Node node) {
        std::shared_ptr<_Node> newNode = std::make_shared<_Node>(node); 
        this->end->next = newNode;
        this->end = newNode;
        this->length++;
    }

    void remove(size_t __i) {
        if (__i >= this->length) {
            throw "Out of Bounds";
        }
        std::shared_ptr<_Node> ref = this->start;
        if (__i == 0) {
            this->start = ref->next;
            return;
        }
        for (size_t i = 0; ref != nullptr && i < __i - 1; i++) {
            ref = ref->next;
        }
        std::shared_ptr<_Node> next = ref->next->next;
        ref->next = next;
        this->length--;
    }

    void prepend(_Node node) {
        std::shared_ptr<_Node> newNode = std::make_shared<_Node>(node); 
        newNode->next = this->start;    
        this->start = newNode;
        this->length++;
    }

    void insert(size_t __i, _Node node) {
        if (__i >= this->length) {
            throw "Out of Bounds";
        }
        if (__i == 0) {
            this->prepend(node);
            return;
        }
        if (__i == this->length - 1) {
            this->append(node);
            return;
        }
        std::shared_ptr<_Node> ref = this->start;
        std::shared_ptr<_Node> newNode = std::make_shared<_Node>(node); 
        for (size_t i = 0; ref != nullptr && i < __i - 1; i++) {
            ref = ref->next;
        }
        newNode->next = ref->next;
        ref->next = newNode;
        this->length++;
    }

    _Node& operator[](size_t __i) {
        return this->getItem(__i);
    }
};

#endif
