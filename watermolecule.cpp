#include <barrier>
#include <vector>
#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <thread>

using namespace std; 

barrier _barrier{ 3 };
char* water = "HHOHOOHHOHOHHHH";

void _releaseOxygen()
{
    _barrier.arrive_and_wait(); 
}

void _releaseHydrogen()
{
    _barrier.arrive_and_wait(); 
}

class H2O{
private:
    vector<char> _input;
    vector<char> _elements; 
    mutex _mutex; 

public:
    H2O()
    {
        _input.insert(_input.begin(), water, water + strlen(water));
        cout << "input: ";
        for( auto c : _input)
            cout << c << ":" ;
        cout << endl; 
    }

    ~H2O(){
        _input.clear();
    }

    void print_input(){
        for( auto c : _input)
            cout << c << " " ;
        cout <<endl;  
    }

    void print_output(){
        cout << "\"";
        for( auto c : _elements)
            cout << c;
        cout << "\"" << endl;  
    }

    void oxygen(function<void()> releaseOxygen){
        while(true){
            unique_lock<mutex> lock(_mutex);
            auto next_O = find(_input.begin(), _input.end(), 'O');
            if (next_O == _input.end()) 
                break;
            _elements.push_back(*next_O);
            _input.erase(next_O); 
            lock.unlock();
            releaseOxygen();
        }
    }

    void hydrogen(function<void()> releaseHydrogen){
        while(true){
            unique_lock<mutex> lock(_mutex);
            auto next_H = find(_input.begin(), _input.end(), 'H');
            if (next_H == _input.end()) 
                break;
            _elements.push_back(*next_H); 
            _input.erase(next_H);
            lock.unlock();
            releaseHydrogen(); 
        }
    }
};

int main(){
    auto h2o = make_unique<H2O>();

    auto oxy_thread = thread(&H2O::oxygen, h2o.get(), &_releaseOxygen);
    auto hyd_thread1 = thread(&H2O::hydrogen, h2o.get(), &_releaseHydrogen);
    auto hyd_thread2 = thread(&H2O::hydrogen, h2o.get(), &_releaseHydrogen);

    oxy_thread.join();
    hyd_thread1.join();
    hyd_thread2.join();

    h2o->print_output(); 
}

