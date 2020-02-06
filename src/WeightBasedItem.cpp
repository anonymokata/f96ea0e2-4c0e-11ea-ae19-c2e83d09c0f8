#include "Types.h"
#include "WeightBasedItem.h"

WeightBasedItem::WeightBasedItem() : FixedPriceItem()
{
    weight_in_cart = 0.0;
}

WeightBasedItem::~WeightBasedItem()
{

}

ReturnCode_t WeightBasedItem::addToCart( double weight )
{
    return ERROR;
}

ReturnCode_t WeightBasedItem::addToCart( int weight )
{
    return ERROR;
}

ReturnCode_t WeightBasedItem::removeFromCart( double weight )
{
    return ERROR;
}

ReturnCode_t WeightBasedItem::removeFromCart( int weight )
{
    return ERROR;
}