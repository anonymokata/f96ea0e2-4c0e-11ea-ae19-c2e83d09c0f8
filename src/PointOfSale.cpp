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
    if(price <= 0.0)
    {
        return INVALID_PRICE;
    }

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    // check to see if price for this sku has already been added as a weight based item
    map<string,double>::iterator it = weight_prices.find(sku);
    if(it != weight_prices.end())
    {
        return PRICING_CONFLICT;
    }

    // add the provided price to the fixed price maps
    fixed_prices[sku] = price;

    return OK;
}

PointOfSale::ReturnCode_t PointOfSale::setPerPoundPrice( std::string sku, double price )
{
    if(price <= 0.0)
    {
        return INVALID_PRICE;
    }

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    // check to see if price for this sku has already been added as a weight based item
    map<string,double>::iterator it = fixed_prices.find(sku);
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
    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    // check to see if price for this sku has already been added as a weight based item
    map<string,double>::iterator it = weight_prices.find(sku);
    if(it != weight_prices.end())
    {
        return ITEM_CONFLICT;
    }

    // Check to see that a price has been configured for this item
    it = fixed_prices.find(sku);
    if(it == fixed_prices.end())
    {
        return NO_PRICE_DEFINED;
    }

    return OK;
}

PointOfSale::ReturnCode_t PointOfSale::addItemWeight( std::string sku, double pounds )
{
    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    if(pounds <= 0.0)
    {
        return INVALID_WEIGHT;
    }

    // check to see if price for this sku has already been added as a weight based item
    map<string,double>::iterator it = fixed_prices.find(sku);
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

    return OK;
}

PointOfSale::ReturnCode_t PointOfSale::removeItem( std::string sku )
{
    return ERROR;
}

PointOfSale::ReturnCode_t PointOfSale::removeItemWeight( std::string sku, double pounds )
{
    return ERROR;
}

double PointOfSale::getPreTaxTotal()
{
    return 0;
}