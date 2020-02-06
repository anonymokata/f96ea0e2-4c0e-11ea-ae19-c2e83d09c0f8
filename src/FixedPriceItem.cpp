#include "Types.h"
#include "FixedPriceItem.h"

FixedPriceItem::FixedPriceItem() : Item()
{
    count_in_cart = 0;
}

FixedPriceItem::~FixedPriceItem()
{
    
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

    double cost = price;
    if(is_markdown_set)
    {
        cost -= markdown;
    }

    *pTaxAmount = cost * count_in_cart;

    return OK;
}