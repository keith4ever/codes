#include <stdio.h>
#include <thread>
#include <functional>
#include <vector>
#include <iostream>

using namespace std; 

void _printFizz()
{
    cout << "fizz,";
}

void _printBuzz()
{
    cout << "buzz,";
}

void _printFizzBuzz()
{
    cout << "fizzbuzz,";
}

void _printNumber(int num)
{
    cout << num << ",";
}


class FizzBuzz {
private:
    int n;
    condition_variable _cond; 
    mutex _mutex; 
    int  _index = 1;

public:
    FizzBuzz(int n) {
        this->n = n;
    }

    // printFizz() outputs "fizz".
    void fizz(function<void()> printFizz) {
        while(_index <= n){
            unique_lock<mutex> lLock(_mutex);
            _cond.wait(lLock, [this] {
                if((_index > n) || ((_index % 3 == 0) && (_index % 5 != 0 )))
                    return true;
                return false; 
            });
            if(_index > n) 
                break;
            printFizz();
            _index++;
            _cond.notify_all();
        }
    };

    // printBuzz() outputs "buzz".
    void buzz(function<void()> printBuzz) {
        while(_index <= n){
            unique_lock<mutex> lLock(_mutex);
            _cond.wait(lLock, [this] {
                if((_index > n) || ((_index % 3 != 0) && (_index % 5 == 0 )))
                    return true;
                return false; 
            });
            if(_index > n) 
                break;
            printBuzz();
            _index++;
            _cond.notify_all();
        }
    };

    // printFizzBuzz() outputs "fizzbuzz".
	void fizzbuzz(function<void()> printFizzBuzz) {
        while(_index <= n){
            unique_lock<mutex> lLock(_mutex);
            _cond.wait(lLock, [this] {
                if((_index > n) || ((_index % 3 == 0) && (_index % 5 == 0 )))
                    return true;
                return false; 
            });
            if(_index > n) 
                break;
            printFizzBuzz();
            _index++;
            _cond.notify_all();
        }
    };

    // printNumber(x) outputs "x", where x is an integer.
    void number(function<void(int)> printNumber) {
        while(_index <= n){
            unique_lock<mutex> lLock(_mutex);
            _cond.wait(lLock, [this] {
                if((_index > n) || ((_index % 3 != 0) && (_index % 5 != 0 )))
                    return true;
                return false; 
            });
            if(_index > n) 
                break;
            printNumber(_index);
            _index++;
            _cond.notify_all();
        }
    };
};


int main(){
    auto fizzbuzz = make_unique<FizzBuzz>(55);

    thread fizz_thread;
    thread buzz_thread;
    thread fizzbuzz_thread;
    thread number_thread;

    cout << "[";
    fizz_thread = thread(&FizzBuzz::fizz, fizzbuzz.get(), &_printFizz);
    buzz_thread = thread(&FizzBuzz::buzz, fizzbuzz.get(), &_printBuzz);
    fizzbuzz_thread = thread(&FizzBuzz::fizzbuzz, fizzbuzz.get(), &_printFizzBuzz);
    number_thread = thread(&FizzBuzz::number, fizzbuzz.get(), &_printNumber);

    fizz_thread.join();
    buzz_thread.join();
    fizzbuzz_thread.join();
    number_thread.join();
    cout << "]" << endl; 
}

