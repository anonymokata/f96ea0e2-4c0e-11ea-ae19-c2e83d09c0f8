#ifndef CART_ITEM_H
#define CART_ITEM_H

#include "Types.h"

/// \class CartItem
/// \brief Implements the logic of a single item in the cart
///
/// The PointOfSale class maintains a CartItem for each SKU within the cart. This class
/// is utilized to handle both fixed price items and weight based items. The use of templates
/// makes it possible to re-use all code associated with all items. The only difference in
/// the logic is whether a whole number of items is maintained or a floating point weight.
template <class T>
class CartItem { 
   
   public:

        CartItem();
        ~CartItem();

        /// \brief Allows for setting price for the item in the cart
        ///
        /// Each item in the cart has a price that is defined. This price will either apply to a single
        /// item or to a pound of the item in the cart.
        ///
        /// \param price Cost per item/pound of the item
        ReturnCode_t setPrice( double price );

        /// \brief Allows for setting a reduction in the price of the item
        ///
        /// Each item in the cart can be configured with a marked down price to support specials. The marked
        /// down value is subtracted from the configured price before all computations are performed
        /// on the cost. In the event of a discount being applied, the marked down price for an item will be
        /// utilized rather than the original price.
        ///
        /// \param amount Amount to take off the normal price for the special
        ReturnCode_t applyMarkdown( double amount );

        /// \brief Allows for discounts where the customer is able to buy multiples of an item for a single lower price
        ///
        /// This discount allows a customer to buy multiple items for a single given price. For instance, this function
        /// would allow for the creation of a discount such as "Buy 5 Chips for the only 3.99" event though the chips price
        /// may be 1.00 for each.
        ///
        /// \param buy_amount The number of items, or pounds, that the customer is allowed for the specified price
        /// \param price The cost for acquiring all the items, or pounds
        ReturnCode_t applyGetXforPriceDiscount( T buy_amount, double price );

        /// \brief Allows for discounts where the customer is able to buy multiples of an item for a single lower price
        ///
        /// This discount allows a customer to buy multiple items for a single given price. For instance, this function
        /// would allow for the creation of a discount such as "Buy 5 Chips for the only 3.99" event though the chips price
        /// may be 1.00 for each. It's possible to limit the number of items that a customer is allowed to purchase using the
        /// discount. This function allows for the specification of a limit that the customer is not allowed to go beyond.
        ///
        /// \param buy_amount The number of items, or pounds, that the customer is allowed for the specified price
        /// \param price The cost for acquiring all the items, or pounds
        /// \param limit The maximum number of items, or pounds, that the customer is able to buy using the discount
        ReturnCode_t applyGetXforPriceDiscount( T buy_amount, double price, T limit );

        /// \brief Allows for discounts where the customer is able to get items at a discounted rate if other qualifying purchases are performed
        ///
        /// This discount allows a customer to purchase a specified number of items at a discounted rate. To get this benefit,
        /// the customer must purchase the specified number of items at full price. For instance, a discount such as 
        /// "Buy 5 Cookies at full price and get the next 2 at %50 off" is possible with this function.
        ///
        /// \param buy_x The number of items, or pounds, that must be purchased at full price
        /// \param get_y The number of items, or pounds, that are able to be purchased at a discounted rate
        /// \param percent_off The amount of savings given to the customer, must be between 0 and 1
        ReturnCode_t applyBuyXGetYDiscount( T buy_x, T get_y, double percent_off );

        /// \brief Allows for discounts where the customer is able to get items at a discounted rate if other qualifying purchases are performed
        ///
        /// This discount allows a customer to purchase a specified number of items at a discounted rate. To get this benefit,
        /// the customer must purchase the specified number of items at full price. For instance, a discount such as 
        /// "Buy 5 Cookies at full price and get the next 2 at %50 off" is possible with this function.
        ///
        /// \param buy_x The number of items, or pounds, that must be purchased at full price
        /// \param get_y The number of items, or pounds, that are able to be purchased at a discounted rate
        /// \param percent_off The amount of savings given to the customer, must be between 0 and 1
        /// \param limit Allows for placing a limit on the number of items that an be purchased with this discount
        ReturnCode_t applyBuyXGetYDiscount( T buy_x, T get_y, double percent_off, T limit );

        /// \brief Allows for adding items, or pounds of a good, to the shopping cart
        ///
        /// The PointOfSale system supports fixed price and weight based items. These items are then
        /// added to the cart to be purchased. This function provides the mechanism by which items are
        /// added to the cart to be purchased.
        ///
        /// \param amount The number of item, or pounds, that should be added to the cart
        ReturnCode_t addToCart( T amount );

        /// \brief Allows for removing items, or pounds, from the shopping cart
        ///
        /// Tellers sometimes make mistakes when checking out a customer. When this happens, it becomes necessary
        /// to remove items from the shopping cart. This function provides the ability to remove those items, or weight,
        /// from the cart.
        ///
        /// \param amount The number of items, or pounds, that should be removed
        ReturnCode_t removeFromCart( T amount );

        /// \brief Calculates the pre-tax cost of the item
        ///
        /// This function will compute the pre-tax cost of the item for the customer. This calcution
        /// will take into affect any markdowns and/or discounts that have been applied.
        ///
        /// \param pTaxAmount Location that the computed pre-tax figure should be stored
        ReturnCode_t computePreTax( double *pTaxAmount );

    private:

        typedef enum
        {
            NO_DISCOUNT,
            X_FOR_FLAT,
            BUY_X_GET_Y_FOR_Z_LIMIT_W,
        } DiscountType_t;

        DiscountType_t discount_type;
        T amount_in_cart;  // maintain count of item in the cart

        // All related to the price and markdown
        double price;      // configured full price for the item
        bool is_price_set; // keep track of when price has been set
        double markdown;   // amount of markdown that is programmed, defaults to 0

        // All related to the discounts associated with the code
        T discount_x;
        T discount_y;
        T discount_limit;
        bool is_discount_set;
        double discount_percent;
        double discount_price;
        bool is_discount_limited;
        
}; 

#endif