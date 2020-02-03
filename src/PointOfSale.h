#ifndef POINT_OF_SALE_H
#define POINT_OF_SALE_H

#include <string>
#include <map>

using namespace std;

/// \class PointOfSale
/// \brief Provides API for interacting with the Point of Sale system to compute the Pre Tax cost for shopping cart of items.
///
/// The PointOfSale class serves as the external interface for the Point Of Sale system. Each instantiation of the PointOfSale
/// class represents a single checkout instance with a given customer. All operations needing to be performed by the cashier
/// are supported through exported API's from this class.
///
/// Items being added, or scanned, into this interface can be done based on a fixed price or by weight. The PointOfSale object
/// will keep track of the number of items that have been scanned of each time and correctly compute the total pre-tax cost.
/// The API also supports the ability to remove items that have been scanned in error as well. Once removed, the items will
/// no longer be figured into the total pre-tax cost for the items in the cart.
///
/// TODO - Add discussion regarding the discounts and markdowns once complete.
class PointOfSale
{
    public:
        
        /// \enum ReturnCode_t
        /// \brief Describes possible return codes for API's of the Point of Sale system
        ///
        /// The PointOfSale API's will return error codes when necessary. These codes provide
        /// information regarding what failed within the system. Each of the codes given below
        /// contain a description of the error.
        typedef enum
        {
            OK               = 0,           ///< Operation completed successfully, no errors encountered
            ERROR            = 1,           ///< An error was encountered that couldn't be covered by more specific errors below
            INVALID_PRICE    = 2,           ///< Provided price is invalid
            PRICING_CONFLICT = 3,           ///< Attempted to set a fixed price and per pound price to same item
            PRICE_UPDATE_NOT_AVAILABLE = 4, ///< Price was updated after an item was added to the cart
            ITEM_CONFLICT    = 5,           ///< Price for SKU configured as Fixed point but item added/removed as Per Pound, or vice versa.
            INVALID_SKU      = 6,           ///< An Invalid SKU was provided to the API
            NO_PRICE_DEFINED = 7,           ///< Prices have not been defined for all items in the cart
            INVALID_WEIGHT   = 8,           ///< An invalid weight was provided to the API
            ITEM_NOT_IN_CART = 9,           ///< Removal of item not allowed without being in cart
        } ReturnCode_t;

        PointOfSale();
        ~PointOfSale();

        /// \fn setItemPrice
        /// \brief Provides ability to setup a fixed price for a SKU
        ///
        /// The PointOfSale class supports fixed price and weight based items being added to the cart. The
        /// fixed price items are a set price for each item. This function provides the ability to configure
        /// an item to be considered a fixed price item. In addition, the price for each unit of the item
        /// is configured via this API. The price of the item can be updated up until a item of that type
        /// has been added to the cart. No further updates will be allowed at that point. Once an item
        /// has been marked as a fixed price item then it can no longer be marked as a weight based item.
        ///
        /// \param sku Represents the item that is being added
        /// \param price The price per unit of the item within the cart
        ReturnCode_t setItemPrice( std::string sku, double price );

        /// \fn setPerPoundPrice
        /// \brief Provides ability to setup a per pound based price for a SKU
        ///
        /// The PointOfSale class supports fixed price and weight based items being added to the cart. The
        /// weight based, or per pound, are a set price per pound of the item. As such, the more of an item
        /// that is purchased the more it will cost. The price per pound of a given SKU is configured via this
        /// API. All configuration of prices should be performed before items are scanned into the system. The 
        /// price of the item can be updated up until a item of that type
        /// has been added to the cart. No further updates will be allowed at that point. Once an item
        /// has been marked as a fixed price item then it can no longer be marked as a weight based item, or vice versa.
        ///
        /// \param sku Represents the item that is being added
        /// \param price The price per pound of the unit being added to the cart
        ReturnCode_t setPerPoundPrice( std::string sku, double price );

        /// \fn addFixedPriceItem
        /// \brief Adds a fixed price item to the shopping cart
        ///
        /// This function adds an item of the designated SKU to the shopping cart. The provided SKU must have
        /// already been configured as a fixed price item via the setItemPrice function. It's possible to add
        /// several instances of the same SKU to the cart by calling this function multiple times. Attempting to
        /// add SKU's configured as a weight based product is not allowed via this function.
        ///
        /// \param sku Represents the item that is being added
        ReturnCode_t addFixedPriceItem( std::string sku );

        /// \fn addItemWeight
        /// \brief Adds a weight based item to the shopping cart
        ///
        /// This function adds the specified amount of the SKu to the shopping cart. The provided SKU must have
        /// already been configured as a weight based item via the setPerPoundPrice function. Multiple calls
        /// can be made to this function for a given SKU. The amounts for all calls will be added when the
        /// total is calculated.
        ///
        /// \param sku Represents the item that is being added
        /// \param pound Amount of the item that should be added to the cart, in pounds
        ReturnCode_t addItemWeight( std::string sku, double pounds );

        // Provide mechanism for removing an item that was previously scanned

        /// \fn removeItem
        /// \brief Removes a fixed price item from the cart
        ///
        /// To account for possible mistakes that can take place at the register, the PointOfSale system supports
        /// the ability to remove items that may have been scanned in error. This function handles the process of
        /// removing a single item of the specified SKU from the cart. The provided SKU must have already been
        /// added to the cart for this function.
        ///
        /// \param sku Represents the item that is being added
        ReturnCode_t removeItem( std::string sku );

        /// \fn removeItemWeight
        /// \brief Removes portion of weight based item from shopping cart
        ///
        /// To account for possible mistakes that can take place at the register, the PointOfSale system supports
        /// the ability to remove items that may have been scanned in error. This function handles the process of
        /// removing a portion of a weight based item that was scanned in error. Once removed, the weight will no
        /// longer be factored into the calculation of the pre-tax price.
        ///
        /// \param sku Represents the item that is being added
        ReturnCode_t removeItemWeight( std::string sku, double pounds );

        /// \fn getPreTaxTotal
        /// \brief Calculates the pre-tax total for all items within cart
        ///
        /// The PointOfSale system keep track of the items in the shopping cart. The items in the cart and
        /// any assoicated discounts and/or markdowns are taken into account when calculating the total
        /// cost of the cart.
        double getPreTaxTotal();

    protected:

    private:
        map<string, double> fixed_prices;  // Keep track of the pricess associated with fixed price items
        map<string, double> weight_prices; // keep track of the cost per pound of items related to weight

};

#endif