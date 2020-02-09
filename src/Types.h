#ifndef TYPES_H
#define TYPES_H

/// \enum ReturnCode_t
/// \brief Describes possible return codes for API's within the PoS system
///
/// The PointOfSale API's will return error codes when necessary. These codes provide
/// information regarding what failed within the system. Each of the codes given below
/// contain a description of the error.
typedef enum
{
    OK,                         ///< Operation completed successfully, no errors encountered
    ERROR,                      ///< An error was encountered that couldn't be covered by more specific errors below
    INVALID_ARG,                ///< Means an invalid argument is provided to the API
    INVALID_PRICE,              ///< Provided price and/or markdown is too large
    INVALID_DISCOUNT,           ///< An invalid combination of values was provided to the discount functions
    INVALID_SKU,                ///< Invalid SKU was provided to the API
    PRICE_UPDATE_NOT_AVAILABLE, ///< Price was updated after an item was added to the cart
    ITEM_CONFLICT,              ///< Price for SKU configured as Fixed point but item added/removed as Per Pound, or vice versa.        
    NO_PRICE_DEFINED,           ///< Prices have not been defined for all items in the cart
    ITEM_NOT_IN_CART,           ///< Removal of item not allowed without being in cart  
} ReturnCode_t;

#endif