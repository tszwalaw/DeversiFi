#ifndef MarketMakingEngine_h
#define MarketMakingEngine_h

#include "Account.h"
#include "DvfSimulator.h"

class MarketMakingEngine{
 
private:
    // Simulator
    IDvfSimulator* sim_;
    
    // Orders
    std::vector<std::pair<DvfSimulator::OrderID, std::pair<double,double>>> bid_orders_;
    std::vector<std::pair<DvfSimulator::OrderID, std::pair<double,double>>> ask_orders_;
    
    double last_bb;
    double last_ba;

    // Account
    Account account_;
    
public:
    MarketMakingEngine();
    
    // Set init cash into the account
    void SetInitCash(double cash);
    
    // Add Trade into the account
    void AddTrade(double price, double qty, int buy_or_sell);
    
    // Wake the Market Making Engine by actions (defined in const.h)
    // - ACTION_OB_UPDATE
    // - ACTION_OB_N_PNL_UPDATE
    void RecevieActions(int action);
    
    // Review the quotes, might be send new / delete quotes
    void ReviewQuotes();
    
    // Cancel All Quotes
    void CancelAllQuotes();
    
    // Sending Bid side quotes
    void SendBidQuotes(double price, double qty);
    
    // Sending Ask side quotes
    void SendAskQuotes(double price, double qty);
    
    // Print the PNL Report
    void PrintPNLReport();
};


#endif /* MarketMakingEngine_h */
