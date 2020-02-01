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
    return ERROR;
}

ReturnCode_t PointOfSale::setPerPoundPrice( std::string sku, double price )
{
    return ERROR;
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