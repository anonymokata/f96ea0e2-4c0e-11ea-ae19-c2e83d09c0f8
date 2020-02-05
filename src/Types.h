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

#endif