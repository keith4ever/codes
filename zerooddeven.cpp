#include <iostream>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std; 

void _printNumber(int num)
{
    cout << num;
}

class ZeroEvenOdd {
private:
    int n, _count = 1;
    mutex _mutex; 
    condition_variable _cv; 
    bool _zero_printed = false; 

public:
    ZeroEvenOdd(int n) {
        this->n = n;
    }

    // printNumber(x) outputs "x", where x is an integer.
    void zero(function<void(int)> printNumber) {
        while(_count <= n){
            unique_lock<mutex> lock(_mutex);
            _cv.wait(lock, [this]{
                if (_count > n) return true; 
                return !_zero_printed; 
            });
            if(_count > n) break;

            printNumber(0);
            _zero_printed = true; 
            _cv.notify_all(); 
        }
    }

    void even(function<void(int)> printNumber) {
        while(_count <= n){
            unique_lock<mutex> lock(_mutex);
            _cv.wait(lock, [this]{
                if (_count > n) return true; 
                if (!_zero_printed) return false; 
                return (_count % 2 == 0);
            });
            if(_count > n) break;

            printNumber(_count);
            _count++; _zero_printed = false; 
            _cv.notify_all(); 
        }        
    }

    void odd(function<void(int)> printNumber) {
        while(_count <= n){
            unique_lock<mutex> lock(_mutex);
            _cv.wait(lock, [this]{
                if (_count > n) return true; 
                if (!_zero_printed) return false; 
                return (_count % 2 == 1);
            });
            if(_count > n) break;

            printNumber(_count);
            _count++; _zero_printed = false; 
            _cv.notify_all(); 
        }        
    }
};


int main(){
    auto zerooddeven = make_unique<ZeroEvenOdd>(9);

    cout << "";
    auto zero_thread = thread(&ZeroEvenOdd::zero, zerooddeven.get(), &_printNumber);
    auto odd_thread = thread(&ZeroEvenOdd::odd, zerooddeven.get(), &_printNumber);
    auto even_thread = thread(&ZeroEvenOdd::even, zerooddeven.get(), &_printNumber);

    zero_thread.join();
    odd_thread.join();
    even_thread.join();
    cout << "" << endl; 
}

