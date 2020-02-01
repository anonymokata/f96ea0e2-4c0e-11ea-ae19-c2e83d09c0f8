#ifndef POINT_OF_SALE_H
#define POINT_OF_SALE_H

#include <string>
#include <map>

using namespace std;

typedef enum
{
    OK               = 0,
    ERROR            = 1,
    BAD_ARGUMENT     = 2,
    INVALID_PRICE    = 3,   /**< Provided price is invalid */
    PRICING_CONFLICT = 4,   /**< Attempted to set a fixed price and per pound price to same item */
    ITEM_CONFLICT    = 5,   /**< Price for SKU configured as Fixed point but item added as Per Pound, or vice versa. */
    INVALID_SKU      = 6,   /**< An Invalid SKU was provided to the API */
    NO_PRICE_DEFINED = 7,   /**< Prices have not been defined for all items in the cart */
    INVALID_WEIGHT   = 8,   /**< An invalid weight was provided to the API */
} ReturnCode_t;

typedef int Transaction_ID;

class PointOfSale
{
    public:

        PointOfSale();
        ~PointOfSale();

        // Configure the price for the fixed price items
        ReturnCode_t setItemPrice( std::string sku, double price );

        // Configure the price for per pound items
        ReturnCode_t setPerPoundPrice( std::string sku, double price );

        // Provide mechanism to scan an item into the system as point a fixed price or a by the pound
        ReturnCode_t addItem( std::string sku );
        ReturnCode_t addItem( std::string sku, double pounds );

        // TODO - Ensure that test is created where items were added and no price is defined before attempting to calculate total price
        double getPreTaxTotal();

    protected:

    private:
        map<string, double> fixed_prices;  // Keep track of the pricess associated with fixed price items
        map<string, double> weight_prices; // keep track of the cost per pound of items related to weight

};

#endif