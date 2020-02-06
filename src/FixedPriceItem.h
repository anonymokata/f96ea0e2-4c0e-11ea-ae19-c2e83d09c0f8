#ifndef FIXED_PRICE_ITEM_H
#define FIXED_PRICE_ITEM_H

#include "Types.h"
#include "Item.h"

class FixedPriceItem : public Item
{

    public:

        FixedPriceItem();
        ~FixedPriceItem();

        ReturnCode_t addToCart( int amount );
        ReturnCode_t removeFromCart( int amount );

        ReturnCode_t computePreTax( double *pTaxAmount );

    protected:

    private:
        int count_in_cart;

};

#endif