//
// Created by keith on 11/10/22.
//

#include <map>

// 1. Design class / structure here
template <class T>
using SparseVector = std::map<size_t, T>;

// 2. Write a sparse dot product method/function
template <class T>
T dot_product(SparseVector<T> &input1, SparseVector<T> &input2) {
    T result{};

    SparseVector<T> &first{};
    SparseVector<T> &second{};
    if(input1.size() < input2.size()){
        first = input1;
        second = input2;
    } else {
        first = input2;
        second = input1;
    }
    //for(SparseVector<T>::iterator iter1 = input1.begin(); iter1 != input1.end(); iter1++) {
    for (auto & [key, val] : first) {
        // key: input1->first
        // value: input1->second
        // key2 will return an iterator --> first, second
        auto key2 = second.find(key); // std::find(input1.begin(), input1.end(), ...);
        // key2->first, key2->second
        if ( key2 != second.end())
            result += key2->second * val;
    }
}
