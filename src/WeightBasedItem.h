#ifndef WEIGHT_BASED_ITEM_H
#define WEIGHT_BASED_ITEM_H

#include "Types.h"

class WeightBasedItem
{

    public:
        WeightBasedItem();
        ~WeightBasedItem();

        ReturnCode_t setPrice( double price );
        ReturnCode_t applyMarkdown( double amount );

        ReturnCode_t applyDiscount( double buy_x, double get_y, double percent_off );
        ReturnCode_t applyDiscount( double buy_x, double get_y, double percent_off, double limit );

        ReturnCode_t addToCart( double weight );
        ReturnCode_t removeFromCart( double weight );

        ReturnCode_t computePreTax( double *pTaxAmount );

    protected:
        

    private:
        double weight_in_cart;

        double price;
        bool is_price_set;

        double markdown;

        bool is_discount_set;
        bool is_discount_limited;

        double discount_x;
        double discount_y;
        double discount_percent;
        double discount_limit;
};

#endif