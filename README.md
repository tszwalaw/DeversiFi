# DeversiFi
DeversiFi coding challenge

I used a market making strategies: "Behind mode"

It will used if the market maker forced to keep quoting while market in a high volatility / unpredictable status.
The quote bid and ask will set behind the best bid/ask, with the maximum spread
(The spread is a market maker's requirement set by the exchange, if the spread between quote bid and ask larger than the maximum spread, the quote will not be count)

Now I calculate the quote bid/ask as best Bid - 5% / best Ask + 5%, and change the quotes while best bid/ask changed

