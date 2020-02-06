#include "Types.h"
#include "FixedPriceItem.h"

FixedPriceItem::FixedPriceItem()
{
    price = 0.0;
    is_price_set = false;

    markdown = 0.0;
    is_markdown_set = false;

    count_in_cart = 0;
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
    is_markdown_set = true;

    return OK;
}

ReturnCode_t FixedPriceItem::addToCart( unsigned int amount )
{
    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    count_in_cart += amount;

    return OK;
}

ReturnCode_t FixedPriceItem::removeFromCart( unsigned int amount )
{



    return ERROR;
}