#ifndef VECTOR_FUNCS__HPP
#define VECTOR_FUNCS__HPP

#include <iostream>
#include <vector>

using std::cout;
using std::vector;


/**
 * @file vectorFuncs.hpp
 * @brief Removes the item at an index in vector.
 * @param __vect The vector to remove the item from.
 * @param __i The index to remove from.
 * @author Sagi Or
*/
template<typename T> void removeAtIndex(vector<T> &__Vect, uint16_t __i){    
    //TODO add exception on size.
    vect.erase(vect.begin() + index);
}

/**
 * @file vectorFuncs.hpp
 * @brief Calls the << operator on each element in the vector.
 * @param __vect The vector to print.
 * @author Sagi Or
*/
template <typename T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& __Vect){
    os << "[";
    for (auto element : vector) {
        os << element << " ";   
    }
    os << "\b]";
    return os;
}

#endif