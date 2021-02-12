#ifndef Account_h
#define Account_h

#include <deque>

// Not much time to implement, should be adding some risk management function,
// Like margin, short sell limit, etc
class Account{
    
private:
    double cash_;

    std::deque<std::pair<double, int>> buy_position_;
    std::deque<std::pair<double, int>> sell_position_;
    
public:
    Account();
    
    // Set a init cash into the account
    void    SetCash(double cash);

    // Show PNL Report, including CASH, Unrealized PNL, and the Total PNL
    void    ShowPNLReport(double best_bid, double best_ask);
    
    // Calculate the Unrealized PNL
    // Buy side position: PNL = best bid - buy position price
    // Sell side position: PNL = ask position price - best_ask
    double  CalUnrealizedPNL(double best_bid, double best_ask);
    
    // Add Trade into the account
    void    AddTrade(double price, double qty, int buy_or_sell);
    
    // First in first out, close the buy/sell position and become cash
    void    CheckClosePosition();

};

#endif /* Account_h */
