#include <stdio.h>
#include <iostream>
#include "Account.h"
#include "const.h"

Account::Account(){
    cash_ = 0;
}

void Account::SetCash(double cash){
    cash_ = cash;
}

void Account::ShowPNLReport(double best_bid, double best_ask){
    double tmp;
    std::cout << "======================" << std::endl;
    std::cout << "==    PNL Report    ==" << std::endl;
    std::cout << "- CASH: " << cash_ << std::endl;
    tmp = CalUnrealizedPNL(best_bid, best_ask);
    std::cout << "- Unreailzed PNL: " << tmp << " ,cal best bid/ask " << best_bid << ", " << best_ask << std::endl;
    for(auto & it : buy_position_){
        std::cout << " BUY, PRICE: " << it.first << ", QTY: " << it.second << std:: endl;
    }
    for(auto & it : sell_position_){
        std::cout << " SELL, PRICE: " << it.first << ", QTY: " << it.second << std:: endl;
    }
    std::cout << "- Total: " << tmp + cash_ << std::endl;
    std::cout << "======================" << std::endl;
}

double Account::CalUnrealizedPNL(double best_bid, double best_ask){
    double result = 0;
    
    for(auto &it : buy_position_){
        result += (best_bid - it.first) * it.second;
        result += it.first * it.second;
    }
    
    for(auto &it : sell_position_){
        result += (it.first - best_ask) * it.second;
    }

    return result;
}

void Account::AddTrade(double price, double qty, int buy_or_sell){
    if(buy_or_sell == TRADE_TYPE_BUY){
        buy_position_.push_back(std::pair<double,double>(price, qty));
        cash_ -= price*qty;
    }else{
        sell_position_.push_back(std::pair<double,double>(price, qty));
    }
    CheckClosePosition();
}

void Account::CheckClosePosition(){
    
    double closed_pos = 0;
    
    while(true){
        if(buy_position_.size() == 0 || sell_position_.size() == 0){
            break;
        }
        
        auto buy = buy_position_.front();
        buy_position_.pop_front();
        
        while(true){
            if(sell_position_.size() == 0){
                break;
            }
            auto sell = sell_position_.front();
            sell_position_.pop_front();
            
            if(buy.second >= sell.second){
                closed_pos = sell.second;
                cash_ += closed_pos * sell.first;
                buy.second -= sell.second;
                sell.second = 0;
            }else{
                closed_pos = buy.second;
                cash_ += sell.first * closed_pos;
                sell.second -= buy.second;
                buy.second = 0;
            }
            
            if(sell.second > 0){
                sell_position_.push_front(sell);
            }
            
            if(buy.second == 0){
                break;
            }
        }
        
        if(buy.second > 0){
            buy_position_.push_front(buy);
        }
    }
}

