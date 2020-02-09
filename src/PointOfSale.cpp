#include <string>

#include "Types.h"
#include "PointOfSale.h"
#include "CartItem.h"

PointOfSale::PointOfSale()
{

}

PointOfSale::~PointOfSale()
{
    
}

ReturnCode_t PointOfSale::setItemPrice( std::string sku, double price )
{
    map<string, CartItem<int>*>::iterator f_it;
    map<string, CartItem<double>*>::iterator w_it;

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    // perform search in both maps for the given sku
    f_it = fixed_items.find(sku);
    w_it = weight_items.find(sku);

    // ensure that item hasn't already been added to system as a weight based
    if(w_it != weight_items.end())
    {
        return ITEM_CONFLICT;
    }

    // if the item is already registered then call function to update price
    if(f_it != fixed_items.end())
    {
        return f_it->second->setPrice(price);
    }
    else
    {
        CartItem<int>* fixed = new CartItem<int>();
        ReturnCode_t code = fixed->setPrice(price);
        fixed_items[sku] = fixed;
        return code;
    }

}

ReturnCode_t PointOfSale::setPerPoundPrice( std::string sku, double price )
{
    map<string, CartItem<int>*>::iterator f_it;
    map<string, CartItem<double>*>::iterator w_it;

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    // perform search in both maps for the given sku
    f_it = fixed_items.find(sku);
    w_it = weight_items.find(sku);

    // ensure that item hasn't already been added to system as a fixed based
    if(f_it != fixed_items.end())
    {
        return ITEM_CONFLICT;
    }

    // if the item is already registered then call function to update price
    if(w_it != weight_items.end())
    {
        return w_it->second->setPrice(price);
    }
    else
    {
        CartItem<double>* weight = new CartItem<double>();
        ReturnCode_t code = weight->setPrice(price);
        weight_items[sku] = weight;
        return code;
    }
}

ReturnCode_t PointOfSale::addToCart( std::string sku, int count )
{
    map<string, CartItem<int>*>::iterator f_it;
    map<string, CartItem<double>*>::iterator w_it;

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    // perform search in both maps for the given sku
    f_it = fixed_items.find(sku);
    w_it = weight_items.find(sku);

    // check to see if price for this sku has already been added as a weight based item
    if(w_it != weight_items.end())
    {
        return ITEM_CONFLICT;
    }

    // An item won't be added to the system if not given a valid price. As such, the existence
    // of the item in the map means that a price has been defined
    if(f_it == fixed_items.end())
    {
        return NO_PRICE_DEFINED;
    }

    return f_it->second->addToCart( 1 );
}

ReturnCode_t PointOfSale::addToCart( std::string sku, double pounds )
{
    map<string, CartItem<int>*>::iterator f_it;
    map<string, CartItem<double>*>::iterator w_it;

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    // perform search in both maps for the given sku
    f_it = fixed_items.find(sku);
    w_it = weight_items.find(sku);

    // check to see if price for this sku has already been added as a fixed price item
    if(f_it != fixed_items.end())
    {
        return ITEM_CONFLICT;
    }

    // An item won't be added to the system if not given a valid price. As such, the existence
    // of the item in the map means that a price has been defined
    if(w_it == weight_items.end())
    {
        return NO_PRICE_DEFINED;
    }

    return w_it->second->addToCart( pounds );
}

ReturnCode_t PointOfSale::removeFromCart( std::string sku, int count )
{
    map<string, CartItem<int>*>::iterator f_it;
    map<string, CartItem<double>*>::iterator w_it;

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    // perform search in both maps for the given sku
    f_it = fixed_items.find(sku);
    w_it = weight_items.find(sku);

    // check to see if item was registered as a weight based item
    if(w_it != weight_items.end())
    {
        return ITEM_CONFLICT;
    }

    // check to see that it was registered as a fixed item
    if(f_it == fixed_items.end())
    {
        return ITEM_NOT_IN_CART;
    }

    return f_it->second->removeFromCart( 1 );
}

ReturnCode_t PointOfSale::removeFromCart( std::string sku, double pounds )
{
    map<string, CartItem<int>*>::iterator f_it;
    map<string, CartItem<double>*>::iterator w_it;

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    // perform search in both maps for the given sku
    f_it = fixed_items.find(sku);
    w_it = weight_items.find(sku);

    // check to see if item was registered as a fixed price item
    if(f_it != fixed_items.end())
    {
        return ITEM_CONFLICT;
    }

    // check to see that it was registered as a fixed item
    if(w_it == weight_items.end())
    {
        return ITEM_NOT_IN_CART;
    }

    return w_it->second->removeFromCart( pounds );
}

double PointOfSale::getPreTaxTotal()
{
    double total = 0.0;
    double price = 0.0;

    map<string, CartItem<int>*>::iterator f_it;
    map<string, CartItem<double>*>::iterator w_it;

    // calculate totals for each of the fixed price items in cart
    f_it = fixed_items.begin();
    while( f_it != fixed_items.end() )
    {
        ReturnCode_t code = f_it->second->computePreTax( &price );
        if(code != OK)
        {
            // TODO - How to handle
        }

        // increment the total based on this item
        total += price;
        f_it++;
    }

    // calculate totals for each of the weight based item in cart
    w_it = weight_items.begin();
    while( w_it != weight_items.end() )
    {
        ReturnCode_t code = w_it->second->computePreTax( &price );
        if(code != OK)
        {
            // TODO - How to handle
        }

        // increment the total based on this item
        total += price;
        w_it++;
    }

    return total;
}

ReturnCode_t PointOfSale::setMarkdown( std::string sku, double price )
{
    map<string, CartItem<int>*>::iterator f_it;
    map<string, CartItem<double>*>::iterator w_it;

    if(sku.length() == 0)
    {
        return INVALID_SKU;
    }

    // perform search in both maps for the given sku
    f_it = fixed_items.find(sku);
    w_it = weight_items.find(sku);

    // check to see that this item has been defined and that a price has been set
    if(f_it == fixed_items.end() && w_it == weight_items.end())
    {
        return NO_PRICE_DEFINED;
    }
    else if(f_it != fixed_items.end())
    {
        return f_it->second->applyMarkdown( price );
    }
    else
    {
        return w_it->second->applyMarkdown( price );
    }

}
        
ReturnCode_t PointOfSale::applyGetXForYDiscount  ( std::string sku, int buy_x, double amount )
{
    return ERROR;
}

ReturnCode_t PointOfSale::applyGetXForYDiscount  ( std::string sku, int buy_x, double amount, int limit )
{
    return ERROR;
}

ReturnCode_t PointOfSale::applyBuyXGetYAtDiscount( std::string sku, int buy_x, int get_y, double percent_off )
{
    return ERROR;
}

ReturnCode_t PointOfSale::applyBuyXGetYAtDiscount( std::string sku, double buy_x, double get_y, double percent_off )
{
    return ERROR;
}

ReturnCode_t PointOfSale::applyBuyXGetYAtDiscount( std::string sku, int buy_x, int get_y, double percent_off, int limit )
{
    return ERROR;
}

ReturnCode_t PointOfSale::applyBuyXGetYAtDiscount( std::string sku, double buy_x, double get_y, double percent_off, double limit )
{
    return ERROR;
}