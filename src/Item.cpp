#include "Types.h"
#include "Item.h"

Item::Item()
{
    price = 0.0;
    is_price_set = false;

    markdown = 0.0;
    is_markdown_set = false;
}

Item::~Item()
{

}

ReturnCode_t Item::setPrice( double amount )
{

    if(amount <= 0.0)
    {
        return INVALID_PRICE;
    }

    price = amount;
    is_price_set = true;

    return OK;
}

ReturnCode_t Item::applyMarkdown( double amount )
{
    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    if(amount < 0 || amount > price)
    {
        return PRICING_CONFLICT;
    }

    markdown = amount;
    is_markdown_set = true;

    return OK;
}
