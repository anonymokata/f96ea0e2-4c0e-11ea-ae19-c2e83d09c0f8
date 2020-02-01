#include <string>

#include "PointOfSale.h"

PointOfSale::PointOfSale()
{

}

PointOfSale::~PointOfSale()
{
    
}

ReturnCode_t PointOfSale::setItemPrice( std::string sku, double price )
{
    if(price <= 0.0)
    {
        return INVALID_PRICE;
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

ReturnCode_t PointOfSale::setPerPoundPrice( std::string sku, double price )
{
    if(price <= 0.0)
    {
        return INVALID_PRICE;
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

ReturnCode_t PointOfSale::addItem( std::string sku )
{
    return ERROR;
}

ReturnCode_t PointOfSale::addItem( std::string sku, double pounds )
{
    return ERROR;
}

double PointOfSale::getPreTaxTotal()
{
    return 0;
}