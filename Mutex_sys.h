#pragma
#include <mutex>
#include <thread>

using namespace std;

class mutex_rule{
    mutex m;

public:
mutex_rule(){
    
}
void start_mutex_rule(){
    m.lock();
}
void end_mutex_rule(){
    m.unlock();
}


};