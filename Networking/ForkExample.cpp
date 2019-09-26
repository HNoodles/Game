#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <iostream>

class ForkExample
{
    bool busy;
    int i;// id
    ForkExample *other;// another thread me is waiting for
    std::mutex *_mutex;
    std::condition_variable *_condition_variable;

    public:
        ForkExample(int i, ForkExample *other, std::mutex *_mutex, std::condition_variable *_condition_variable) 
			: i(i), _mutex(_mutex), _condition_variable(_condition_variable)
        {
            if (i == 0) 
			{ 
				busy = true; 
				this->other = nullptr;
			}
            else 
			{ 
				busy = false;
				this->other = other; 
			}
        }

        bool isBusy()
        {
			// will automatically unlock after the scope expires
            std::lock_guard<std::mutex> lock(*_mutex);// _mutex->lock();// but need manually unlock _mutex->unlock(); 
            return busy;
        }

        void run()
        {
            if(i==0)
            {
                try
                {
                    std::cout << "Thread " << i << " is sleeping" << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
                    {
                        std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                        _condition_variable->notify_all();// notify threads waiting for me, no contract on the order of notifying
                        std::cout << "Thread " << i << " is notifying with busy=" << busy << std::endl;
                    }
                    std::cout << "Thread " << i << " is sleeping" << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
                    {
                        std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                        busy = !busy;
                        _condition_variable->notify_all();
                        std::cout << "Thread " << i << " is notifying with busy=" << busy << std::endl;
                    }
                }
                catch (...)
                {
                    std::cerr << "Thread " << i << " caught exception." << std::endl;
                }
            }
            else // id > 0
            {
                while(other->isBusy())
                {
                    std::cout << "Thread " << i << " is waiting!" << std::endl;
                    try 
                    {
                        std::unique_lock<std::mutex> lock(*_mutex);
                        _condition_variable->wait(lock);
                        std::cout << "Thread " << i << " inside lock scope." << std::endl;
                    }
                    catch (...)
                    {
                        std::cerr << "Thread " << i << " caught exception." << std::endl;
                    }
                }
                std::cout << "Thread " << i << " is FINISHED!" << std::endl;
            }
        }

};

void run_wrapper(ForkExample *fe)
{
    fe->run();
}
//
//int main()
//{
//    std::mutex m;
//    std::condition_variable cv;
//    ForkExample t1(0, NULL, &m, &cv);
//    ForkExample t2(1, &t1, &m, &cv);
//    std::thread first(run_wrapper, &t1);
//    std::thread second(run_wrapper, &t2);
//    first.join();
//    second.join();
//}
