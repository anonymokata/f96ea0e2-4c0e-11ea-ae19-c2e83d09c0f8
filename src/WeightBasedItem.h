#ifndef WEIGHT_BASED_ITEM_H
#define WEIGHT_BASED_ITEM_H

#include "Types.h"
#include "Item.h"

class WeightBasedItem : public Item
{

    public:
        WeightBasedItem();
        ~WeightBasedItem();

        ReturnCode_t computePreTax( double *pTaxAmount );

        ReturnCode_t addToCart( double weight );
        ReturnCode_t removeFromCart( double weight );

    protected:
        double weight_in_cart;

    private:

};

#endif