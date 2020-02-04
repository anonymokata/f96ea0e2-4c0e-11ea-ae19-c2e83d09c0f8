#include <string>

#include "PointOfSale.h"

PointOfSale::PointOfSale()
{

}

PointOfSale::~PointOfSale()
{
    
}

PointOfSale::ReturnCode_t PointOfSale::setItemPrice( std::string sku, double price )
{
    map<string,int>::iterator it;
    map<string,double>::iterator it_db;

    if(price <= 0.0)
    {
        return INVALID_PRICE;
    }

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    // check to see if price for this sku has already been added as a weight based item
    it = fixed_price_cart.find(sku);
    if(it != fixed_price_cart.end())
    {
        return PRICE_UPDATE_NOT_AVAILABLE;
    }

    // check to see if price for this sku has already been added as a weight based item
    it_db = weight_prices.find(sku);
    if(it_db != weight_prices.end())
    {
        return PRICING_CONFLICT;
    }

    // add the provided price to the fixed price maps
    fixed_prices[sku] = price;

    return OK;
}

PointOfSale::ReturnCode_t PointOfSale::setPerPoundPrice( std::string sku, double price )
{
    map<string,double>::iterator it;

    if(price <= 0.0)
    {
        return INVALID_PRICE;
    }

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    it = weight_cart.find(sku);
    if(it != weight_cart.end())
    {
        return PRICE_UPDATE_NOT_AVAILABLE;
    }

    // check to see if price for this sku has already been added as a weight based item
    it = fixed_prices.find(sku);
    if(it != fixed_prices.end())
    {
        return PRICING_CONFLICT;
    }

    // add the provided price to the fixed price maps
    weight_prices[sku] = price;

    return OK;
}

PointOfSale::ReturnCode_t PointOfSale::addFixedPriceItem( std::string sku )
{
    map<string,int>::iterator it;
    map<string,double>::iterator it_db;

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    // check to see if price for this sku has already been added as a weight based item
    it_db = weight_prices.find(sku);
    if(it_db != weight_prices.end())
    {
        return ITEM_CONFLICT;
    }

    // Check to see that a price has been configured for this item
    it_db = fixed_prices.find(sku);
    if(it_db == fixed_prices.end())
    {
        return NO_PRICE_DEFINED;
    }

    // check to see if item already in cart
    it = fixed_price_cart.find(sku);
    if(it == fixed_price_cart.end())
    {
        // if not in cart then it needs to be added with zero items so that 
        // following code works regardless of current state of map
        fixed_price_cart[sku] = 0;
    }

    // increment the count of the item within the cart for fixed price items
    fixed_price_cart[sku] += 1;

    return OK;
}

PointOfSale::ReturnCode_t PointOfSale::addItemWeight( std::string sku, double pounds )
{
    map<string,double>::iterator it;

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    if(pounds <= 0.0)
    {
        return INVALID_WEIGHT;
    }

    // check to see if price for this sku has already been added as a weight based item
    it = fixed_prices.find(sku);
    if(it != fixed_prices.end())
    {
        return ITEM_CONFLICT;
    }

    // Check to see that a price has been configured for this item
    it = weight_prices.find(sku);
    if(it == weight_prices.end())
    {
        return NO_PRICE_DEFINED;
    }

    // check the cart to see if items of this type have already been added. If not, the itemm within the cart
    // should be initialized to zero. This allows the following code to work regardless of whether this is the
    // first time adding the weight based item as well as if its a repeat. 
    it = weight_cart.find(sku);
    if(it == weight_cart.end())
    {
        weight_cart[sku] = 0.0;
    }

    // increment the count for the given item in the cart
    weight_cart[sku] += pounds;

    return OK;
}

PointOfSale::ReturnCode_t PointOfSale::removeItem( std::string sku )
{
    map<string, int>::iterator it;
    map<string, double>::iterator it_db;

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    it_db = weight_prices.find(sku);
    if(it_db != weight_prices.end())
    {
        return ITEM_CONFLICT;
    }

    // verify that items of this type have been added to the cart in the past
    it = fixed_price_cart.find(sku);
    if(it == fixed_price_cart.end())
    {
        return ITEM_NOT_IN_CART;
    }

    // decrement the item count in the cart and erase the item from the cart
    // entirely if there are no current items.
    it->second -= 1;
    if(it->second == 0)
    {
        fixed_price_cart.erase(it);
    }

    return OK;
}

PointOfSale::ReturnCode_t PointOfSale::removeItemWeight( std::string sku, double pounds )
{
    map<string, double>::iterator it;

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    if(pounds <= 0)
    {
        return INVALID_WEIGHT;
    }

    it = fixed_prices.find(sku);
    if(it != fixed_prices.end())
    {
        return ITEM_CONFLICT;
    }

    // verify that items of this type have been added to the cart in the past
    it = weight_cart.find(sku);
    if(it == weight_cart.end())
    {
        return ITEM_NOT_IN_CART;
    }

    if(pounds > it->second)
    {
        return ITEM_NOT_IN_CART;
    }

    it->second -= pounds;
    if(it->second == 0.0)
    {
        weight_cart.erase(it);
    }

    return OK;
}

double PointOfSale::getPreTaxTotal()
{
    double total = 0.0;
    map<string, int>::iterator fixed_cart_it;
    map<string, double>::iterator weight_cart_it;
    map<string, double>::iterator price_it;

    // calculate totals for each of the fixed price items in cart
    fixed_cart_it = fixed_price_cart.begin();
    while( fixed_cart_it != fixed_price_cart.end() )
    {
        // retrieve iterator for looking up the price of the item
        price_it = fixed_prices.find(fixed_cart_it->first);

        // compute the cost for this fixed price item based on configured price and number items
        total += (fixed_cart_it->second * price_it->second);

        // update the progress of the iterator so that next item can be processed
        fixed_cart_it++;
    }

    // calculate totals for each of the weight based items in cart
    weight_cart_it = weight_cart.begin();
    while ( weight_cart_it != weight_cart.end() )
    {
        // retrieve iterator to lookup the price per pound for the item
        price_it = weight_prices.find(weight_cart_it->first);

        // compute the cost for the weighted items
        total += (weight_cart_it->second * price_it->second);

        // increment the iterator to walk through the cart
        weight_cart_it++;
    }

    return total;
}

PointOfSale::ReturnCode_t PointOfSale::setMarkdown( std::string sku, double price )
{
    return ERROR;
}