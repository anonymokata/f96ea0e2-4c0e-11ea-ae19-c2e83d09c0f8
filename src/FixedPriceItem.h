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

        ReturnCode_t applyDiscount( int buy_amount, double price );
        ReturnCode_t applyDiscount( int buy_amount, double price, int limit );
        ReturnCode_t applyDiscount( int buy_x, int get_y, double percent_off );
        ReturnCode_t applyDiscount( int buy_x, int get_y, double percent_off, int limit );

        ReturnCode_t addToCart( int amount );
        ReturnCode_t removeFromCart( int amount );

        ReturnCode_t computePreTax( double *pTaxAmount );

    protected:

    private:

        typedef enum
        {
            
            NO_DISCOUNT,
            X_FOR_FLAT,
            BUY_X_GET_Y_FOR_Z_LIMIT_W,

        } DiscountType_t;

        int count_in_cart;

        double price;
        bool is_price_set;

        double markdown;

        DiscountType_t discount_type;
        bool is_discount_limited;
        int discount_x;
        int discount_y;
        double discount_percent;
        double discount_price;
        int discount_limit;
};

#endif