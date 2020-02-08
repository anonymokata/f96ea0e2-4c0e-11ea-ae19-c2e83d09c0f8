#include "Types.h"
#include "WeightBasedItem.h"

WeightBasedItem::WeightBasedItem()
{
    weight_in_cart = 0.0;
    price = 0.0;
    is_price_set = false;
    markdown = 0.0;

    is_discount_set = false;
    is_discount_limited = false;

    discount_x = 0.0;
    discount_y = 0.0;
    discount_percent = 0.0;
    discount_limit = 0.0;
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

ReturnCode_t WeightBasedItem::applyDiscount( double buy_x, double get_y, double percent_off )
{

    if(buy_x < 0 || get_y < 0 || percent_off < 0)
    {
        return INVALID_DISCOUNT;
    }
    if(percent_off > 1.0)
    {
        return INVALID_DISCOUNT;
    }

    is_discount_set = true;
    is_discount_limited = false;

    discount_x = buy_x;
    discount_y = get_y;
    discount_percent = percent_off;
    discount_limit = 0;

    return OK;
}

ReturnCode_t WeightBasedItem::applyDiscount( double buy_x, double get_y, double percent_off, double limit )
{

    if(buy_x < 0 || get_y < 0 || percent_off < 0 || limit <= 0)
    {
        return INVALID_DISCOUNT;
    }
    if(percent_off > 1.0)
    {
        return INVALID_DISCOUNT;
    }

    is_discount_set = true;
    is_discount_limited = true;

    discount_x = buy_x;
    discount_y = get_y;
    discount_percent = percent_off;
    discount_limit = limit;

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
    double total = 0.0;
    double items_discounted = 0;
    double items_remain = weight_in_cart;
    double normalized_cost = price - markdown;

    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    while(is_discount_set)
    {
        // check to see if the limit has been reached for particular discount
        if((is_discount_limited) && (items_discounted >= discount_limit))
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

    // compute cost for rest of the items that weren't covered by discount
    total += (normalized_cost * items_remain);
    *pTaxAmount = total;

    return OK;
}