#include "Types.h"
#include "CartItem.h"

template <class T>
CartItem<T>::CartItem()
{
    amount_in_cart = 0;
    price = 0.0;
    is_price_set = false;
    markdown = 0.0;

    discount_type = NO_DISCOUNT;
    is_discount_limited = false;

    discount_x = 0;
    discount_y = 0;
    discount_percent = 0.0;
    discount_price = 0.0;
    discount_limit = 0;
}

template <class T>
CartItem<T>::~CartItem()
{

}

template <class T>
ReturnCode_t CartItem<T>::setPrice( double amount )
{
    if(amount <= 0.0)
    {
        return INVALID_PRICE;
    }

    if(amount_in_cart > 0)
    {
        return PRICE_UPDATE_NOT_AVAILABLE;
    }

    price = amount;
    is_price_set = true;

    return OK;
}

template <class T>
ReturnCode_t CartItem<T>::applyMarkdown( double amount )
{
    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    if(amount < 0 || amount > price)
    {
        return INVALID_PRICE;
    }

    if(amount_in_cart > 0)
    {
        return PRICE_UPDATE_NOT_AVAILABLE;
    }

    markdown = amount;

    return OK;
}

template <class T>
ReturnCode_t applyDiscount( T buy_amount, double price )
{
    if(buy_amount < 0 || price < 0)
    {
        return INVALID_DISCOUNT;
    }

    discount_type = X_FOR_FLAT;

    discount_x = buy_amount;
    discount_y = 0;
    discount_percent = 0.0;
    discount_price = price;
    is_discount_limited = false;
    discount_limit = 0;

    return OK;
}

template <class T>
ReturnCode_t CartItem<T>::applyDiscount( T buy_amount, double price, T limit )
{
    if(buy_amount < 0 || price < 0 || limit <= 0)
    {
        return INVALID_DISCOUNT;
    }

    discount_type = X_FOR_FLAT;
    
    discount_x = buy_amount;
    discount_y = 0;
    discount_percent = 0.0;
    discount_price = price;
    is_discount_limited = true;
    discount_limit = limit;

    return OK;
}

template <class T>
ReturnCode_t CartItem<T>::applyDiscount( T buy_x, T get_y, double percent_off )
{
    if(buy_x < 0 || get_y < 0 || percent_off < 0)
    {
        return INVALID_DISCOUNT;
    }
    if(percent_off > 1.0)
    {
        return INVALID_DISCOUNT;
    }

    discount_type = BUY_X_GET_Y_FOR_Z_LIMIT_W;
    
    discount_x = buy_x;
    discount_y = get_y;
    discount_limit = 0;
    is_discount_set = true;
    discount_percent = percent_off;
    discount_price = 0.0;
    is_discount_limited = false;
    
    return OK;
}

template <class T>
ReturnCode_t CartItem<T>::applyDiscount( T buy_x, T get_y, double percent_off, T limit )
{
    if(buy_x < 0 || get_y < 0 || percent_off < 0 || limit <= 0)
    {
        return INVALID_DISCOUNT;
    }
    if(percent_off > 1.0)
    {
        return INVALID_DISCOUNT;
    }

    discount_type = BUY_X_GET_Y_FOR_Z_LIMIT_W;
    
    discount_x = buy_x;
    discount_y = get_y;
    discount_limit = limit;
    is_discount_set = true;
    discount_percent = percent_off;
    discount_price = 0.0;
    is_discount_limited = true;
    
    return OK;
}

template <class T>
ReturnCode_t CartItem<T>::addToCart( T amount )
{
    if(!is_price_set)
    {
        return NO_PRICE_DEFINED;
    }

    if(amount <= 0)
    {
        return ERROR;
    }

    amount_in_cart += amount;

    return OK;
}

template <class T>
ReturnCode_t CartItem<T>::removeFromCart( T amount )
{
    if(amount_in_cart < amount || amount_in_cart == 0)
    {
        return ITEM_NOT_IN_CART;
    }

    if(amount <= 0)
    {
        return ERROR;
    }

    amount_in_cart -= amount;

    return OK;
}

template <class T>
ReturnCode_t CartItem<T>::computePreTax( double *pTaxAmount )
{
    double total = 0.0;
    T items_discounted = 0;
    T items_remain = amount_in_cart;
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
                T items_to_discount = discount_y;
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