#include <stdio.h>
#include "MarketMakingEngine.h"
#include "DvfSimulator.h"
#include "const.h"


MarketMakingEngine::MarketMakingEngine(){
    sim_ = DvfSimulator::Create();
    last_bb = 0;
    last_ba = 0;
}

void MarketMakingEngine::SetInitCash(double cash){
    account_.SetCash(cash);
}

void MarketMakingEngine::AddTrade(double price, double qty, int buy_or_sell){
    account_.AddTrade(price, qty, buy_or_sell);
}

void MarketMakingEngine::RecevieActions(int action){
    switch(action){
        case ACTION_OB_UPDATE:
            ReviewQuotes();
            break;
        case ACTION_OB_N_PNL_UPDATE:
            ReviewQuotes();
            PrintPNLReport();
            break;
        default:
            break;
    }
}

void MarketMakingEngine::ReviewQuotes(){
    if(sim_ == NULL){
        return;
    }

    auto ob = sim_->GetOrderBook();
    
    double market_bb = 0;
    double market_ba = 0;
    
    // Find best bid and best ask
    std::sort(ob.begin(), ob.end(), [](auto &left, auto &right) {
        return left.first < right.first;
    });
    
    int pos = 0;
    for(unsigned int u {0}; u < ob.size(); ++u){
        if(ob.at(u).second < 0){
            pos = u;
            break;
        }
    }
    
    if(pos-1 < 0 || pos == ob.size()){
        // If market has no bid or no ask
        CancelAllQuotes();
        return;
    }
    
    market_bb = ob.at(pos-1).first;
    market_ba = ob.at(pos).first;
    
    std::cout << "OB BB: " << market_bb << ", OB BA: " << market_ba << std::endl;
    
    // Bid Side quotes
    if(last_bb != market_bb){
        last_bb = market_bb;
        
        for(auto &it : bid_orders_){
            // Check order becomes trade, otherwise cancel order
            if(it.second.first > last_bb){
                account_.AddTrade(it.second.first, it.second.second, TRADE_TYPE_BUY);
            }else{
                sim_->CancelOrder(it.first);
            }
        }
        bid_orders_.clear();
        
        // Send out new quotes
        SendBidQuotes(last_bb, QUOTE_QTY);
    }
    
    // Ask Side quotes
    if(last_ba != market_ba){
        last_ba = market_ba;
        
        for(auto &it : ask_orders_){
            // Check order becomes trade, otherwise cancel order
            if(it.second.first < last_ba){
                account_.AddTrade(it.second.first, it.second.second, TRADE_TYPE_SELL);
            }else{
                sim_->CancelOrder(it.first);
            }
        }
        ask_orders_.clear();
        
        // Send out new quotes
        SendAskQuotes(last_ba, QUOTE_QTY);
    }


}

void MarketMakingEngine::CancelAllQuotes(){
    if(sim_ == NULL){
        return;
    }
    for(auto &it: bid_orders_){
        sim_->CancelOrder(it.first);
    }
    
    for(auto &it : ask_orders_){
        sim_->CancelOrder(it.first);
    }
}

void MarketMakingEngine::SendBidQuotes(double base_price, double qty){
    if(sim_ == NULL){
        return;
    }
    double quote_price = 0;
    base_price *= (100-QUOTE_LIMIT_PRECENTAGE) / 100;
    base_price = ceil(base_price * 10) / 10; //Round up for making sure not exceed 5% limit
    
    using order = std::pair<DvfSimulator::OrderID, std::pair<double,double>>;

    for(int i {0}; i < QUOTES_ON_EACH_SIDE; ++i){
        quote_price = base_price + i * QUOTE_STEP_SIZE;
        auto ord_id = sim_->PlaceOrder(quote_price, QUOTE_QTY);
        if(ord_id.has_value()){
            bid_orders_.push_back(order(ord_id.value(), std::pair<double, double>(quote_price, QUOTE_QTY)));
        }else{
            account_.AddTrade(quote_price, qty, TRADE_TYPE_BUY);
        }
    }
}

void MarketMakingEngine::SendAskQuotes(double base_price, double qty){
    if(sim_ == NULL){
        return;
    }
    double quote_price = 0;
    base_price *= (100+QUOTE_LIMIT_PRECENTAGE) / 100;
    base_price = floor(base_price * 10) / 10; //Round down for making sure not exceed 5% limit
    
    using order = std::pair<DvfSimulator::OrderID, std::pair<double,double>>;

    for(int i {0}; i < QUOTES_ON_EACH_SIDE; ++i){

        quote_price = base_price - i * QUOTE_STEP_SIZE;
        auto ord_id = sim_->PlaceOrder(quote_price, -1 * QUOTE_QTY);
        if(ord_id.has_value()){
            ask_orders_.push_back(order(ord_id.value(), std::pair<double, double>(quote_price, QUOTE_QTY)));
        }else{
            account_.AddTrade(quote_price, qty, TRADE_TYPE_SELL);
        }
    }
}

void MarketMakingEngine::PrintPNLReport(){
    account_.ShowPNLReport(last_bb, last_ba);
}

