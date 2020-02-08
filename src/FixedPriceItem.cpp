#include <cstdio>

#include "Types.h"
#include "FixedPriceItem.h"

FixedPriceItem::FixedPriceItem()
{
    count_in_cart = 0;
    markdown = 0.0;
    price = 0.0;
    is_price_set = false;

    discount_type = NO_DISCOUNT;
    discount_x = 0;
    discount_y = 0;
    discount_percent = 0.0;
    discount_limit = 0;
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

ReturnCode_t FixedPriceItem::applyDiscount( int buy_amount, double price )
{
    if(buy_amount < 0 || price < 0)
    {
        return INVALID_DISCOUNT;
    }

    discount_type = X_FOR_FLAT;
    discount_x = buy_amount;
    discount_price = price;
    discount_limit = 0;

    return OK;
}

ReturnCode_t FixedPriceItem::applyDiscount( int buy_amount, double price, int limit )
{
    if(buy_amount < 0 || price < 0 || limit < 0)
    {
        return INVALID_DISCOUNT;
    }

    discount_type = X_FOR_FLAT;
    discount_x = buy_amount;
    discount_price = price;
    discount_limit = limit;

    return OK;
}

ReturnCode_t FixedPriceItem::applyDiscount( int buy_x, int get_y, double percent_off )
{
    if(buy_x < 0 || get_y < 0 || percent_off < 0)
    {
        return INVALID_DISCOUNT;
    }
    if(percent_off > 1.0)
    {
        return INVALID_DISCOUNT;
    }

    // save off values associated with discount to be utilized by pre-tax calculate function
    discount_type = BUY_X_GET_Y_FOR_Z_LIMIT_W;
    discount_x = buy_x;
    discount_y = get_y;
    discount_percent = percent_off;
    
    return OK;
}

ReturnCode_t FixedPriceItem::applyDiscount( int buy_x, int get_y, double percent_off, int limit )
{
    if(limit < 0)
    {
        return INVALID_DISCOUNT;
    }

    discount_limit = limit;
    return applyDiscount( buy_x, get_y, percent_off );
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
    double total = 0.0;
    int items_discounted = 0;
    int items_remain = count_in_cart;
    double normalized_cost = price - markdown;

    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    if(discount_type == X_FOR_FLAT)
    {
        while(items_remain >= discount_x)
        {
            if((items_discounted >= discount_limit) && (discount_limit != 0))
            {
                break;
            }

            items_remain -= discount_x;
            items_discounted += discount_x;
            total += discount_price;
        }
    }
    else if(discount_type == BUY_X_GET_Y_FOR_Z_LIMIT_W)
    {
        while(discount_type != NO_DISCOUNT)
        {
            // check to see if the limit has been reached for particular discount
            if((discount_limit != 0) && (items_discounted >= discount_limit))
            {
                break;
            }

            // check to see if enough items remain to qualify for discount
            if(items_remain > discount_x)
            {
                // decrement items needing processed
                items_remain -= discount_x;
                items_discounted += discount_x;
                total += (discount_x * normalized_cost);

                if((discount_limit != 0) && (items_discounted >= discount_limit))
                {
                    break;
                }

                // calculate number items that should be discounted
                int items_to_discount = discount_y;
                if(items_remain == 0)
                {
                    items_to_discount = 0;
                }
                else if(items_remain < discount_y)
                {
                    items_to_discount = items_remain;
                }

                // decrement count so that the items are not counted again
                items_remain -= items_to_discount;
                items_discounted += items_to_discount;

                // compute the discounted price adn add to the running total
                double original_price = items_to_discount * normalized_cost;
                double discount_price = original_price * (1 - discount_percent);

                total += discount_price;
            }
            else
            {
                break;
            }
        }
    }

    // compute cost for rest of the items that weren't covered by discount
    total += (normalized_cost * items_remain);
    *pTaxAmount = total;

    return OK;
}