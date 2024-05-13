#include <vector>
#include <mutex>
#include <thread>
#include <barrier>
#include <list>

using namespace std; 

class ImageProcessorService{
private:
    vector<thread> _imageThreads; 
    mutex _mutex; 
    condition_variable _cv; 
    int max_threads = 8; 
    int curr_threads = 0;
    list<vector<float>> _delayed_images; 
    vector<vector<float>> _images; 

public:
    virtual int NNloop(int index){
        while(true){
            unique_lock<mutex> lock(_mutex);
            _cv.wait(lock, [this, index]{
                return  !_images.at(index).empty();
            });
            curr_threads++; 
            neuralNetwork(_images.at(index));
            _images.at(index).clear();
            curr_threads--; 
        }
    }

    virtual int neuralNetwork(std::vector<float> image);

    void init(){
        for(int i; i < max_threads; i++){
            _imageThreads.at(i) = thread(&NNloop, this, i);
        }
        _images.resize(8);
    }

    int find_next_availble_thread(){
        for(int i; i < max_threads; i++)
        {
            unique_lock<mutex> lock(_mutex);
            if(_images.at(i).empty())
                return i; 
        }
        return -1; 
    }

    void processImage(std::vector<float> image)
    {
        if(curr_threads >= max_threads){
            _delayed_images.push_back(image);
            return;
        }

        if(curr_threads <= max_threads - 2){
            while(true){
                int index = find_next_availble_thread();
                if(index < 0 || curr_threads == max_threads - 1) 
                    break; 

                auto old_image = _delayed_images.front();
                // check the timestamp to see if it is too old.. then discard it
                unique_lock<mutex> lock(_mutex);
                _images.at(index)= old_image;
                _delayed_images.pop_front();
                _cv.notify_all();
            }
            auto index = find_next_availble_thread();
            unique_lock<mutex> lock(_mutex);
            _images.at(index) = image;
            _cv.notify_all();
        }
    }
};


