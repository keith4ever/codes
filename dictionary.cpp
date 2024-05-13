//
// Created by keith on 11/7/22.
//
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template <class T>
class KeyValue {
private:
    vector<string> m_keys;
    vector<T> m_values;

public:

    KeyValue() {
    }

    ~KeyValue() {
        m_keys.clear();
        m_values.clear();
        print();
    }

    bool add(string key, T value) {
        if(key.length() < 2) return false;

        auto it = std::find(m_keys.begin(), m_keys.end(), key);
        if( it != m_keys.end()){
            int index = it - m_keys.begin();
            m_values[index] = value;
        }
        else{
            m_keys.push_back(key);
            m_values.push_back(value);
        }
        return true;
    }

    T get(string key){
        auto it = find(m_keys.begin(), m_keys.end(), key);
        if( it != m_keys.end()){
            int index = it - m_keys.begin();
            return m_values[index];
        }
        else{
            return 0;
        }
    }

    bool remove(string key) {
        auto it = find(m_keys.begin(), m_keys.end(), key);
        if( it != m_keys.end()){
            int index = it - m_keys.begin();
            m_keys.erase(it);
            m_values.erase(m_values.begin() + index);
        }
        else{
            return false;
        }
        return true;
    }

    void print() {
        int index = 0;

        cout << "------" << endl;
        for(vector<string>::iterator iter = m_keys.begin(); iter != m_keys.end(); iter++){
            cout << index << ": " << *iter << ": " << m_values[index++] << endl;
        }
    }
};

int main(){
    auto keyvalue = new KeyValue<float>();
    keyvalue->add("one", 1.0);
    keyvalue->add("two", 2.0);
    keyvalue->add("three", 3.0);
    keyvalue->print();
    keyvalue->add("three", 3.1);
    keyvalue->add("", 10.0);
    keyvalue->add("four", 4.0);
    keyvalue->print();
    cout << "get two: " << keyvalue->get("two") << endl;
    keyvalue->remove("two");
    keyvalue->print();
}

