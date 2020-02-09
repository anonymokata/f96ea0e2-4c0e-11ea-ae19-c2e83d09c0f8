#ifndef CART_ITEM_H
#define CART_ITEM_H

#include "Types.h"

template <class T>
class CartItem { 
   
   public:

        CartItem();
        ~CartItem();

        ReturnCode_t setPrice( double price );
        ReturnCode_t applyMarkdown( double amount );

        ReturnCode_t applyGetXforPriceDiscount( T buy_amount, double price );
        ReturnCode_t applyGetXforPriceDiscount( T buy_amount, double price, T limit );

        ReturnCode_t applyBuyXGetYDiscount( T buy_x, T get_y, double percent_off );
        ReturnCode_t applyBuyXGetYDiscount( T buy_x, T get_y, double percent_off, T limit );

        ReturnCode_t addToCart( T amount );
        ReturnCode_t removeFromCart( T amount );

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