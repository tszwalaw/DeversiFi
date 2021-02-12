#ifndef const_h
#define const_h

enum TRADE_TYPE{
    TRADE_TYPE_BUY,
    TRADE_TYPE_SELL
};

enum ACTION{
    ACTION_WAIT = 0,
    ACTION_OB_UPDATE,
    ACTION_OB_N_PNL_UPDATE,
    ACTION_END
};

static int WAKE_TIME = 5000;
static int RUNNING_MAX_COUNT = 100;
static int WAKE_PNL_COUNT = 6;

// Quotes Related Const
static double QUOTE_QTY = 1;
static double QUOTE_STEP_SIZE = 1;
static double QUOTE_LIMIT_PRECENTAGE = 5;

static double QUOTES_ON_EACH_SIDE = 5;



#endif /* const_h */
