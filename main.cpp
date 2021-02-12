

#include <iostream>
#include <thread>
#include <chrono>
#include "DvfSimulator.h"
#include "MarketMakingEngine.h"
#include "const.h"

std::mutex  mutex_;
std::condition_variable  con_var_;
int action_;

/* Timer
 * - simulate order come every 5 second,
 * - 30 seconds will wake the market making engine to print out the posistions report
 * - counter add every 5seconds, Timer stop when counter reach RUNNING_MAX_COUNT
 */

void Timer(){
    
    std::cout << "Timer Started" << std::endl;

    int wake_counter = 0;
    
    while(action_ != ACTION_END){
        std::this_thread::sleep_for(std::chrono::milliseconds(WAKE_TIME));
        if(wake_counter >= RUNNING_MAX_COUNT){
            action_ = ACTION_END;
        }else{
            if(wake_counter % WAKE_PNL_COUNT == 0){
                action_ = ACTION_OB_N_PNL_UPDATE;
            }else{
                action_ = ACTION_OB_UPDATE;
            }
        }
        con_var_.notify_all();
        wake_counter++;
    }
    
    std::cout << "Timer END" << std::endl;
}

/* RunMMEngine
 * - Run the MM Engine
 * - End if the action become "END" status
 */

void RunMMEngine(MarketMakingEngine* mm){
    
    if(mm == NULL){
        return;
    }

    std::unique_lock<std::mutex> lock(mutex_);
    
    while(true){
        con_var_.wait(lock);
        
        if(action_ == ACTION_END){
            break;
        }else{
            std::cout << "-----------------------" << std::endl;
            mm->RecevieActions(action_);
            action_ = ACTION_WAIT;
        }
    }
}


int main(int argc, const char * argv[]) {
    
    MarketMakingEngine mm;
    //init account
    mm.SetInitCash(2000);
    mm.AddTrade(0, 10, TRADE_TYPE_BUY);

    action_ = ACTION_WAIT;
    
    std::thread market_making_th(RunMMEngine, &mm);
    std::thread timer_th(Timer);
    
    
    timer_th.join();
    market_making_th.join();
    return 0;
}
