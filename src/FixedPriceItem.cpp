#include "Types.h"
#include "FixedPriceItem.h"

FixedPriceItem::FixedPriceItem()
{
    count_in_cart = 0;
    markdown = 0.0;
    price = 0.0;
    is_price_set = false;
}

FixedPriceItem::~FixedPriceItem()
{
    
}

ReturnCode_t FixedPriceItem::setPrice( double amount )
{

    if(amount <= 0.0)
    {
        return INVALID_PRICE;
    }

    if(count_in_cart > 0)
    {
        return PRICE_UPDATE_NOT_AVAILABLE;
    }

    price = amount;
    is_price_set = true;

    return OK;
}

ReturnCode_t FixedPriceItem::applyMarkdown( double amount )
{
    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    if(amount < 0 || amount > price)
    {
        return PRICING_CONFLICT;
    }

    if(count_in_cart > 0)
    {
        return PRICE_UPDATE_NOT_AVAILABLE;
    }

    markdown = amount;

    return OK;
}


ReturnCode_t FixedPriceItem::addToCart( int amount )
{
    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    if(amount < 0)
    {
        return ERROR;
    }

    count_in_cart += amount;

    return OK;
}

ReturnCode_t FixedPriceItem::removeFromCart( int amount )
{

    if(count_in_cart < amount || count_in_cart == 0)
    {
        return ITEM_NOT_IN_CART;
    }

    if(amount < 0)
    {
        return ERROR;
    }

    count_in_cart -= amount;

    return OK;
}

ReturnCode_t FixedPriceItem::computePreTax( double *pTaxAmount )
{
    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    // compute the adjusted cost based on markdown
    double normalized_cost = price - markdown;

    // compute the based on item count in the cart
    *pTaxAmount = normalized_cost * count_in_cart;

    return OK;
}