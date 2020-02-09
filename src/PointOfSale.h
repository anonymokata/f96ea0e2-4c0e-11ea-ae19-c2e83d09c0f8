#ifndef POINT_OF_SALE_H
#define POINT_OF_SALE_H

#include <string>
#include <map>

#include "Types.h"
#include "CartItem.h"
// #include "FixedPriceItem.h"
// #include "WeightBasedItem.h"

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

        PointOfSale();
        ~PointOfSale();

        /// \fn setMarkdown
        /// \brief Provides ability to enable a marked down price on an item
        ///
        /// The PointOfSale system supports the ability to register an item with a marked down price. In this situation,
        /// the price of a fixed price item will be reduced by the amount. For a weight based item, the markdown
        /// price will be applied to each pound of the iem that is sold.
        ///
        /// \param sku Represents the itemt hat is being added
        /// \param price Amount of discount to apply to the item
        ReturnCode_t setMarkdown( std::string sku, double price );

        /// \fn getPreTaxTotal
        /// \brief Calculates the pre-tax total for all items within cart
        ///
        /// The PointOfSale system keep track of the items in the shopping cart. The items in the cart and
        /// any assoicated discounts and/or markdowns are taken into account when calculating the total
        /// cost of the cart.
        double getPreTaxTotal();

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

        /// \fn addToCart
        /// \brief Adds fixed price items to the cart
        ///
        /// This function adds the specified amount of the SKu to the shopping cart. The provided SKU must have
        /// already been configured as a weight based item via the setPerPoundPrice function. Multiple calls
        /// can be made to this function for a given SKU. The amounts for all calls will be added when the
        /// total is calculated.
        ///
        /// \param sku Represents the item that is being added
        /// \param pound Amount of the item that should be added to the cart, in pounds
        ReturnCode_t addToCart( std::string sku, int count );

        /// \fn addToCart
        /// \brief Adds weight to an item in the cart
        ///
        /// This function adds the specified amount of the SKu to the shopping cart. The provided SKU must have
        /// already been configured as a weight based item via the setPerPoundPrice function. Multiple calls
        /// can be made to this function for a given SKU. The amounts for all calls will be added when the
        /// total is calculated.
        ///
        /// \param sku Represents the item that is being added
        /// \param pound Amount of the item that should be added to the cart, in pounds
        ReturnCode_t addToCart( std::string sku, double weight );

        /// \fn removeFromCart
        /// \brief Removes fixed price items from cart
        ///
        /// To account for possible mistakes that can take place at the register, the PointOfSale system supports
        /// the ability to remove items that may have been scanned in error. This function handles the process of
        /// removing a portion of a weight based item that was scanned in error. Once removed, the weight will no
        /// longer be factored into the calculation of the pre-tax price.
        ///
        /// \param sku Represents the item that is being added
        ReturnCode_t removeFromCart( std::string sku, int count );

        /// \fn removeFromCart
        /// \brief Removes portion of weight based item from shopping cart
        ///
        /// To account for possible mistakes that can take place at the register, the PointOfSale system supports
        /// the ability to remove items that may have been scanned in error. This function handles the process of
        /// removing a portion of a weight based item that was scanned in error. Once removed, the weight will no
        /// longer be factored into the calculation of the pre-tax price.
        ///
        /// \param sku Represents the item that is being added
        ReturnCode_t removeFromCart( std::string sku, double weight );

        /// \fn applyGetXForYDiscount
        ///
        /// Only applicable for fixed price items
        ReturnCode_t applyGetXForYDiscount  ( std::string sku, int buy_x, double amount );
        ReturnCode_t applyGetXForYDiscount  ( std::string sku, int buy_x, double amount, int limit );

        /// Only applicable for fixed price items
        ReturnCode_t applyBuyXGetYAtDiscount( std::string sku, int buy_x, int get_y, double percent_off );
        ReturnCode_t applyBuyXGetYAtDiscount( std::string sku, int buy_x, int get_y, double percent_off, int limit );

        /// Only applicable for weight based items
        ReturnCode_t applyBuyXGetYAtDiscount( std::string sku, double buy_x, double get_y, double percent_off );
        ReturnCode_t applyBuyXGetYAtDiscount( std::string sku, double buy_x, double get_y, double percent_off, double limit );

        
    protected:

    private:
        // map<string, FixedPriceItem*>  fixed_items;
        map<string, CartItem<int>*>  fixed_items;
        map<string, CartItem<double>*> weight_items;

};

#endif