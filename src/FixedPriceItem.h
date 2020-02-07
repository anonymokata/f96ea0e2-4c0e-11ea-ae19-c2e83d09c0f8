#ifndef FIXED_PRICE_ITEM_H
#define FIXED_PRICE_ITEM_H

#include "Types.h"

class FixedPriceItem
{

    public:

        FixedPriceItem();
        ~FixedPriceItem();

        ReturnCode_t setPrice( double price );
        ReturnCode_t applyMarkdown( double amount );
        ReturnCode_t applyDiscount( int buy_x, int get_y, double percent_off );

        ReturnCode_t addToCart( int amount );
        ReturnCode_t removeFromCart( int amount );

        ReturnCode_t computePreTax( double *pTaxAmount );

    protected:

    private:
        int count_in_cart;

        double price;
        bool is_price_set;

        double markdown;

};

#endif