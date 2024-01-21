#ifndef _VectorFuncs_
#define _VectorFuncs_

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
template<typename T> void removeAtIndex(vector<T> &__vect, size_t __i){  
    if (__i >= __vect.size() || __i < 0) { 
    throw std::invalid_argument("index must be in range 0 < index < vector.size()\nYour Vector Size: "\
                                 + std::to_string(__vect.size()) + "\nindex: " + std::to_string(__i)); 
    }
    __vect.erase(__vect.begin() + __i);
}

/**
 * @file vectorFuncs.hpp
 * @brief Calls the << operator on each element in the vector.
 * @param __vect The vector to print.
 * @author Sagi Or
*/
template <typename T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& __vect){
    os << "[";
    for (T element : __vect) {
        os << element << " ";   
    }
    os << "\b]";
    return os;
}

#endif