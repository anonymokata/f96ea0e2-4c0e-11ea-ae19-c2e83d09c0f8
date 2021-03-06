#ifndef POINT_OF_SALE_H
#define POINT_OF_SALE_H

#include <string>
#include <map>

#include "Types.h"
#include "CartItem.h"

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
/// The PointOfSale class provides methods for applying discounts to items that are added to the shopping cart. There are two variants
/// of discounts that can be applied to items in the cart. The first discount is where the customer buys a specified amount of
/// of an item and they get a discounted rate on a specified number of items. For sale, the customer may get 5.00 lbs of beef 
/// at 75% off if 10 pounds are purchased at full price. This discount can apply to both fixed price items as well as items
/// sold by the pound. The second discount is a bundling where the customer gets X number of items for a set price. Each of the
/// discounts also support a limit whereby the customer is now allowed to get the discount on more than the specified number of items,
/// or pounds.
class PointOfSale
{
    public:

        PointOfSale();
        ~PointOfSale();

        /// \brief Provides ability to enable a marked down price on an item
        ///
        /// The PointOfSale system supports the ability to register an item with a marked down price. In this situation,
        /// the price of a fixed price item will be reduced by the amount. For a weight based item, the markdown
        /// price will be applied to each pound of the iem that is sold.
        ///
        /// \param sku Represents the itemt hat is being added
        /// \param price Amount of discount to apply to the item
        ReturnCode_t setMarkdown( std::string sku, double price );

        /// \brief Calculates the pre-tax total for all items within cart
        ///
        /// The PointOfSale system keep track of the items in the shopping cart. The items in the cart and
        /// any assoicated discounts and/or markdowns are taken into account when calculating the total
        /// cost of the cart.
        double getPreTaxTotal();

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

        /// \brief Adds fixed price items to the cart
        ///
        /// This function adds the specified amount of the SKU to the shopping cart. The provided SKU must have
        /// already been configured with a fixed price before it can be added to the cart. Multiple copies that item
        /// can be added to the cart at once.
        ///
        /// \param sku Represents the item that is being added
        /// \param pound Amount of the item that should be added to the cart
        ReturnCode_t addToCart( std::string sku, int count );

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

        /// \brief Removes fixed price items from cart
        ///
        /// To account for possible mistakes that can take place at the register, the PointOfSale system supports
        /// the ability to remove items that may have been scanned in error. This function handles the process of
        /// removing the specified number of items from the cart. The items must have already been added to the cart
        /// or this function will return an error.
        ///
        /// \param sku Represents the item that is being added
        /// \param count The number of items that need to removed from the cart
        ReturnCode_t removeFromCart( std::string sku, int count );

        /// \brief Removes portion of weight based item from shopping cart
        ///
        /// To account for possible mistakes that can take place at the register, the PointOfSale system supports
        /// the ability to remove items that may have been scanned in error. This function handles the process of
        /// removing a portion of a weight based item that was scanned in error. Once removed, the weight will no
        /// longer be factored into the calculation of the pre-tax price.
        ///
        /// \param sku Represents the item that is being added
        /// \param weight Amount of the item that should be removed from the cart, in pounds
        ReturnCode_t removeFromCart( std::string sku, double weight );

        /// \brief Applys a buy X items for the Z price
        ///
        /// The following function allows for the application of a discount in which the customer is allowed
        /// to buy x items for a bundled price. For instance, the customer may be allowed to buy 4 bags of 
        /// chips for 5.00.
        ///
        /// \param sku Represents the item that is being added
        /// \param buy_x Number of items that must be purchased for discount to apply
        /// \param amount Cost for purchasing the number of specified items
        ReturnCode_t applyGetXForYDiscount  ( std::string sku, int buy_x, double amount );

        /// \brief Applys a buy X items for the Z price
        ///
        /// The following function allows for the application of a discount in which the customer is allowed
        /// to buy x items for a bundled price. For instance, the customer may be allowed to buy 4 bags of 
        /// chips for 5.00. This function allows for placing a limit on the number of items in which this
        /// discount may apply.
        ///
        /// \param sku Represents the item that is being added
        /// \param buy_x Number of items that must be purchased for discount to apply
        /// \param amount Cost for purchasing the number of specified items
        ReturnCode_t applyGetXForYDiscount  ( std::string sku, int buy_x, double amount, int limit );

        /// \brief Applies a discount that allows the customer to purchae items at discounted rate after purchasing a qualified number of items
        ///
        /// The function allows for registering a discount that allows the customer to purchase items at a discounted
        /// rate given that they have purchased a qualifying number of items. For instance, a discount of this form
        /// may say something like "Buy 5 Cookies and Get 2 more at 70% off".
        ///
        /// \param sku Represents the item that is being added
        /// \param buy_x The number of items that must be purchased at full price to receive discount
        /// \param buy_y The number of items that customer is allowed to buy at discounted rate
        /// \param percent_off The percentage of discount on the y items. Must be between 0 and 1.0
        ReturnCode_t applyBuyXGetYAtDiscount( std::string sku, int buy_x, int get_y, double percent_off );

        /// \brief Applies a discount that allows the customer to purchae items at discounted rate after purchasing a qualified number of items
        ///
        /// The function allows for registering a discount that allows the customer to purchase items at a discounted
        /// rate given that they have purchased a qualifying number of items. For instance, a discount of this form
        /// may say something like "Buy 5 Cookies and Get 2 more at 70% off". A limit can be applied to the number f items
        /// that can be purchased with this discount
        ///
        /// \param sku Represents the item that is being added
        /// \param buy_x The number of items that must be purchased at full price to receive discount
        /// \param buy_y The number of items that customer is allowed to buy at discounted rate
        /// \param percent_off The percentage of discount on the y items. Must be between 0 and 1.0
        /// \param limit The number of items that are allowed to be purchased with this discount
        ReturnCode_t applyBuyXGetYAtDiscount( std::string sku, int buy_x, int get_y, double percent_off, int limit );

        /// \brief Applies a discount that allows the customer to purchae items at discounted rate after purchasing a qualified number of items
        ///
        /// The function allows for registering a discount that allows the customer to purchase items at a discounted
        /// rate given that they have purchased a qualifying number of items. For instance, a discount of this form
        /// may say something like "Buy 5 Cookies and Get 2 more at 70% off". A limit can be applied to the number f items
        /// that can be purchased with this discount
        ///
        /// \param sku Represents the item that is being added
        /// \param buy_x The number of pounds that must be purchased at full price to receive discount
        /// \param buy_y The number of pounds that customer is allowed to buy at discounted rate
        /// \param percent_off The percentage of discount on the y pounds. Must be between 0 and 1.0
        ReturnCode_t applyBuyXGetYAtDiscount( std::string sku, double buy_x, double get_y, double percent_off );

        /// \brief Applies a discount that allows the customer to purchae items at discounted rate after purchasing a qualified number of items
        ///
        /// The function allows for registering a discount that allows the customer to purchase items at a discounted
        /// rate given that they have purchased a qualifying number of items. For instance, a discount of this form
        /// may say something like "Buy 5 Cookies and Get 2 more at 70% off". A limit can be applied to the number f items
        /// that can be purchased with this discount
        ///
        /// \param sku Represents the item that is being added
        /// \param buy_x The number of pounds that must be purchased at full price to receive discount
        /// \param buy_y The number of pounds that customer is allowed to buy at discounted rate
        /// \param percent_off The percentage of discount on the y pounds. Must be between 0 and 1.0
        /// \param limit The number of pounds that are allowed to be purchased with this discount
        ReturnCode_t applyBuyXGetYAtDiscount( std::string sku, double buy_x, double get_y, double percent_off, double limit );
    protected:

    private:
        map<string, CartItem<int>*>  fixed_items;
        map<string, CartItem<double>*> weight_items;

};

#endif