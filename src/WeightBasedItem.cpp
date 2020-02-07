#include "Types.h"
#include "WeightBasedItem.h"

WeightBasedItem::WeightBasedItem()
{
    weight_in_cart = 0.0;
    price = 0.0;
    is_price_set = false;
    markdown = 0.0;
}

WeightBasedItem::~WeightBasedItem()
{

}

ReturnCode_t WeightBasedItem::setPrice( double amount )
{

    if(amount <= 0.0)
    {
        return INVALID_PRICE;
    }

    if(weight_in_cart > 0)
    {
        return PRICE_UPDATE_NOT_AVAILABLE;
    }

    price = amount;
    is_price_set = true;

    return OK;
}

ReturnCode_t WeightBasedItem::applyMarkdown( double amount )
{
    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    if(amount < 0 || amount > price)
    {
        return INVALID_PRICE;
    }

    if(weight_in_cart > 0)
    {
        return PRICE_UPDATE_NOT_AVAILABLE;
    }

    markdown = amount;

    return OK;
}


ReturnCode_t WeightBasedItem::addToCart( double weight )
{
    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    if(weight <= 0)
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

    // compute the adjusted cost based on markdown
    double normalized_cost = price - markdown;

    // compute the based on item count in the cart
    *pTaxAmount = normalized_cost * weight_in_cart;

    return OK;
}