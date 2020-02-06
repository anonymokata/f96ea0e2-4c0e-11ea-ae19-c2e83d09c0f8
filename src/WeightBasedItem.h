#ifndef WEIGHT_BASED_ITEM_H
#define WEIGHT_BASED_ITEM_H

#include "Types.h"
#include "FixedPriceItem.h"

class WeightBasedItem : public FixedPriceItem
{

    public:
        WeightBasedItem();
        ~WeightBasedItem();

        ReturnCode_t addToCart( int weight );
        ReturnCode_t addToCart( double weight );

        ReturnCode_t removeFromCart( int weight );
        ReturnCode_t removeFromCart( double weight );

    protected:
        double weight_in_cart;

    private:

};

#endif