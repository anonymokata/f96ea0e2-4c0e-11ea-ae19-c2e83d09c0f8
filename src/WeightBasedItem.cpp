#include "Types.h"
#include "WeightBasedItem.h"

WeightBasedItem::WeightBasedItem() : Item()
{
    weight_in_cart = 0.0;
}

WeightBasedItem::~WeightBasedItem()
{

}

ReturnCode_t WeightBasedItem::addToCart( double weight )
{
    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    if(weight < 0)
    {
        return INVALID_WEIGHT;
    }

    weight_in_cart += weight;

    return OK;
}

ReturnCode_t WeightBasedItem::removeFromCart( double weight )
{
    if(weight < 0)
    {
        return INVALID_WEIGHT;
    }

    if(weight > weight_in_cart)
    {
        return ITEM_NOT_IN_CART;
    }

    weight_in_cart -= weight;

    return OK;
}

ReturnCode_t WeightBasedItem::computePreTax( double *pTaxAmount )
{
    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    double cost = price;
    if(is_markdown_set)
    {
        cost -= markdown;
    }

    // *pTaxAmount = cost * ;

    return OK;
}