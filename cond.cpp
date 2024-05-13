#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

using namespace std;

class FooBar {
private:
    int n;
    int idx = 0;

    mutex m_mutex;
    condition_variable m_cond;

public:
    FooBar(int n) {
        this->n = n;
    }

    void foo() {
        cout << "entry of foo" << endl;
        unique_lock<mutex> lock(m_mutex);
        cout << "after foo local lock" << endl;
        for (int i = 0; i < n; i++) {
            if(idx % 2 == 1) m_cond.wait(lock);
        	// printFoo() outputs "foo". Do not change or remove this line.
        	cout << "foo";
            idx++;
            m_cond.notify_all();
        }
    }

    void bar() {
        cout << "entry of bar" << endl;
        unique_lock<mutex> lock(m_mutex);
        cout << "after bar local lock" << endl;
        for (int i = 0; i < n; i++) {
            if(idx % 2 == 0) m_cond.wait(lock);
        	// printBar() outputs "bar". Do not change or remove this line.
        	cout << "bar ";
            idx++;
            m_cond.notify_all();
        }
    }
};

int main(){
    auto foobar = new FooBar(8);
    thread foothr(&FooBar::foo, foobar);
    thread barthr(&FooBar::bar, foobar);

    foothr.join();
    barthr.join();
    cout << endl;
}
