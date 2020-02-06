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
        // ReturnCode_t applyDiscount();

        ReturnCode_t addToCart( unsigned int amount );
        ReturnCode_t removeFromCart( unsigned int amount );

    protected:
        double price;
        bool is_price_set;

        double markdown;
        bool is_markdown_set;

    private:
        unsigned int count_in_cart;

};

#endif