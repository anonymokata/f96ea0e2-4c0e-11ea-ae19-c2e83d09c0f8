#include "gtest/gtest.h"
#include "PointOfSale.h"

TEST (CartManagementTest, addItemNegativeWeight){

    PointOfSale sale;
    sale.setPerPoundPrice( "bananas", 2.50 );
    ASSERT_EQ( INVALID_WEIGHT, sale.addItemWeight( "bananas", -1.0 ) );

}

TEST (CartManagementTest, addItemZeroWeight){

    PointOfSale sale;
    sale.setPerPoundPrice( "bananas", 3.50 );
    ASSERT_EQ( INVALID_WEIGHT, sale.addItemWeight( "bananas", 0.0 ) );

}

TEST (CartManagementTest, setPriceThenAddItemOfConflictingType){

    PointOfSale fixed_then_weight_sale;

    // Set a price as if item is a fixed price
    ASSERT_EQ( OK, fixed_then_weight_sale.setItemPrice( "bananas", 1.0 ) );

    // attempt to item to cart as if its a weight based item
    ASSERT_EQ( ITEM_CONFLICT, fixed_then_weight_sale.addItemWeight( "bananas", 1.0 ) );

}

TEST (CartManagementTest, addItemBeforeSetPrice){

    PointOfSale fixed_sale;
    PointOfSale weight_sale;

    // add bananas as if they are fixed price item
    ASSERT_EQ( NO_PRICE_DEFINED, fixed_sale.addFixedPriceItem( "bananas" ) );

    // add item as if they are a weight based item
    ASSERT_EQ( NO_PRICE_DEFINED, weight_sale.addItemWeight( "Ground Beef", 1.0 ) );

}

TEST (CartManagementTest, removeItemFromEmptyCart ){

    PointOfSale sale;

    sale.setItemPrice( "Cookies", 1.0 );
    sale.setPerPoundPrice( "Bananas", 1.0 );

    ASSERT_EQ( ITEM_NOT_IN_CART, sale.removeItem( "Cookies" ) );
    ASSERT_EQ( ITEM_NOT_IN_CART, sale.removeItemWeight( "Bananas", 1.0 ) );

}

TEST (CartManagementTest, removeMoreItemsThanAddedToCart ){

    PointOfSale sale;

    sale.setItemPrice("Cookies", 1.0);
    sale.setPerPoundPrice( "Bananas", 2.3 );

    // add some initial items into the cart
    ASSERT_EQ( OK, sale.addFixedPriceItem( "Cookies" ));
    ASSERT_EQ( OK, sale.addItemWeight("Bananas", 27.0 ));

    // remove the fixed price item from the cart
    ASSERT_EQ( OK, sale.removeItem( "Cookies" ) );
    ASSERT_EQ( ITEM_NOT_IN_CART, sale.removeItem( "Cookies" ) );

    // remove content from the weighted item
    ASSERT_EQ( OK, sale.removeItemWeight( "Bananas", 20.0 ) );
    ASSERT_EQ( OK, sale.removeItemWeight( "Bananas", 5.0 ) );
    ASSERT_EQ( ITEM_NOT_IN_CART, sale.removeItemWeight( "Bananas", 3.0 ) );
}

TEST (CartManagementTest, removeItemsInvalidSKU ){

    PointOfSale sale;

    ASSERT_EQ( INVALID_SKU, sale.removeItem( "" ) );
    ASSERT_EQ( INVALID_SKU, sale.removeItemWeight( "", 20.0 ) );

}

TEST (CartManagementTest, removeItemsConflictingTypes ){

    PointOfSale sale;

    sale.setItemPrice( "Cookies", 1.2 );
    sale.setPerPoundPrice("Bananas", 2.4);

    ASSERT_EQ( ITEM_CONFLICT, sale.removeItem( "Bananas" ) );
    ASSERT_EQ( ITEM_CONFLICT, sale.removeItemWeight( "Cookies", 20.0 ) );

}

TEST (CartManagementTest, removeItemNegativeWeight){

    PointOfSale sale;

    sale.setPerPoundPrice( "Bananas", 2.3 );

    ASSERT_EQ( OK, sale.addItemWeight("Bananas", 5.0));
    ASSERT_EQ( INVALID_WEIGHT, sale.removeItemWeight( "Bananas", -1.0 ));

}

