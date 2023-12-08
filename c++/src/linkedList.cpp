#include "linkedList.hpp"

LinkedList::LinkedList(Rect rect, Velocity2D velocity){
    std::shared_ptr<Node> newNode = std::make_shared<Node>(rect, velocity);
    this->start = newNode;
    this->end = newNode;
}

LinkedList::LinkedList(){
    std::shared_ptr<Node> newNode = std::make_shared<Node>();
    this->start = newNode;
    this->end = newNode;
}

Node& LinkedList::getItem(size_t __i){
    if (__i > this->length){
        throw "Out of Bounds";
    }
    std::shared_ptr<Node> ref = this->start;

    for (size_t i = 0; i < __i; i++){
        ref = ref->next;
    }
    return *ref;
}

void LinkedList::append(Node __item){
    std::shared_ptr<Node> newNode = std::make_shared<Node>(__item); 
    this->end->next = newNode;
    this->end = newNode;
    this->length++;
}

void LinkedList::remove(size_t __i){
    if (__i > this->length){
        throw "Out of Bounds";
    }
    std::shared_ptr<Node> ref = this->start;
    if (__i == 0){
        this->start = ref->next;
        return;
    }
    for (size_t i = 0; ref != nullptr && i < __i - 1; i++){
        ref = ref->next;
    }
    std::shared_ptr<Node> next = ref->next->next;
    ref->next = next;
    this->length--;
}

void LinkedList::prepend(Node __item){
    std::shared_ptr<Node> newNode = std::make_shared<Node>(__item); 
    newNode->next = this->start;    
    this->start = newNode;
    this->length++;
}

void LinkedList::insert(size_t __i, Node __item){
    if (__i > this->length){
        throw "Out of Bounds";
    }
    if (__i == 0){
        this->prepend(__item);
        return;
    }
    if (__i == this->length){
        this->append(__item);
        return;
    }
    std::shared_ptr<Node> ref = this->start;
    std::shared_ptr<Node> newNode = std::make_shared<Node>(__item); 
    for (size_t i = 0; ref != nullptr && i < __i - 1; i++){
        ref = ref->next;
    }
    newNode->next = ref->next;
    ref->next = newNode;
}

Node& LinkedList::operator[](size_t __i){
    return this->getItem(__i);
}
