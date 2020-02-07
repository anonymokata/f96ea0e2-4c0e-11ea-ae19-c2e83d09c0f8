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
        // ReturnCode_t applyDiscount();

        ReturnCode_t addToCart( double weight );
        ReturnCode_t removeFromCart( double weight );

        ReturnCode_t computePreTax( double *pTaxAmount );

    protected:
        

    private:
        double weight_in_cart;

        double price;
        bool is_price_set;

        double markdown;
};

#endif