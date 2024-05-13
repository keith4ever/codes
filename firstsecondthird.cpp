#include <stdio.h>
#include <thread>
#include <functional>
#include <vector>

using namespace std; 

void _printFirst() 
{ 
    printf("first"); 
};

void _printSecond() 
{ 
    printf("second"); 
};

void _printThird() 
{ 
    printf("third"); 
};

class Foo {
private:
    condition_variable _cond; 
    mutex _mutex; 
    int  _index = 0;

public:
    Foo() {
    };

    void first(function<void()> printFirst) {
        unique_lock<mutex> lLock(_mutex);
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
        ++_index; 
        _cond.notify_all(); 
    }

    void second(function<void()> printSecond) {
        unique_lock<mutex> lLock(_mutex);
        _cond.wait(lLock, [this] {return _index == 1; });
        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();
        ++_index; 
        _cond.notify_all();
    }

    void third(function<void()> printThird) {
        unique_lock<mutex> lLock(_mutex);
        _cond.wait(lLock, [this] {return _index == 2; });
        
        // printThird() outputs "third". Do not change or remove this line.
        ++_index; 
        printThird();
        _cond.notify_all();
    }
};

int main(){
    vector<int> inputs{3, 2, 1};
    auto foo = make_unique<Foo>();

    thread first_thread;
    thread second_thread;
    thread third_thread;

    for(int i : inputs)
    {
        // printf("index: %d\n", i); 
        if(i == 1)
            first_thread = thread(&Foo::first, foo.get(), &_printFirst);
        else if( i == 2)
            second_thread = thread(&Foo::second, foo.get(), &_printSecond);
        else 
            third_thread = thread(&Foo::third, foo.get(), &_printThird);
    }

    first_thread.join();
    second_thread.join();
    third_thread.join();
}

