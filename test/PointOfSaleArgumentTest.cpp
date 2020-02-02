#include "gtest/gtest.h"
#include "PointOfSale.h"

// Pull in the test fixture that contains already programmed prices for items in cart
#include "PreDefinedPricesFixture.h"

TEST (PointOfSaleArgumentTest, setNegativePrice){

    PointOfSale sale;
    ASSERT_EQ( PointOfSale::INVALID_PRICE, sale.setItemPrice( "bananas", -1.0 ) );
    ASSERT_EQ( PointOfSale::INVALID_PRICE, sale.setPerPoundPrice( "Ground Beef", -1.0 ) );

}

TEST (PointOfSaleArgumentTest, setZeroPrice){

    PointOfSale sale;
    ASSERT_EQ( PointOfSale::INVALID_PRICE, sale.setItemPrice( "bananas", 0.0 ) );
    ASSERT_EQ( PointOfSale::INVALID_PRICE, sale.setPerPoundPrice( "Ground Beef", 0.0 ) );

}

TEST (PointOfSaleArgumentTest, setValidPrice){

    PointOfSale sale;
    ASSERT_EQ( PointOfSale::OK, sale.setItemPrice( "bananas", 1.89 ) );
    ASSERT_EQ( PointOfSale::OK, sale.setPerPoundPrice( "Ground Beef", 2.50 ) );

}

TEST (PointOfSaleArgumentTest, setConflictingPrices){

    PointOfSale fixed_sale_first;
    PointOfSale weight_sale_first;
    
    ASSERT_EQ( PointOfSale::OK, fixed_sale_first.setItemPrice( "bananas", 2.50 ) );
    ASSERT_EQ( PointOfSale::PRICING_CONFLICT, fixed_sale_first.setPerPoundPrice( "bananas", 4.50 ) );

    ASSERT_EQ( PointOfSale::OK, weight_sale_first.setPerPoundPrice( "bananas", 4.50 ) );
    ASSERT_EQ( PointOfSale::PRICING_CONFLICT, weight_sale_first.setItemPrice( "bananas", 2.50 ) );

}

// TODO - Ensure that a test is created that has prices being updated in middle of adding items to cart. Need to ensure that this
//        is not allowed per the project writeup that states that prices will be configured before adding items to cart.
TEST (PointOfSaleArgumentTest, updateItemPrice){

    PointOfSale sale;
    ASSERT_EQ( PointOfSale::OK, sale.setItemPrice( "bananas", 1.89 ) );
    ASSERT_EQ( PointOfSale::OK, sale.setItemPrice( "bananas", 2.50 ) );

    ASSERT_EQ( PointOfSale::OK, sale.setPerPoundPrice( "Ground Beef", 2.50 ) );
    ASSERT_EQ( PointOfSale::OK, sale.setPerPoundPrice( "Ground Beef", 3.89 ) );

}

TEST (PointOfSaleArgumentTest, updateItemPriceAfterItemInCart){

    PointOfSale sale;
    ASSERT_EQ( PointOfSale::OK, sale.setItemPrice( "bananas", 1.89 ) );
    ASSERT_EQ( PointOfSale::OK, sale.addFixedPriceItem( "bananas" ) );
    ASSERT_EQ( PointOfSale::PRICE_UPDATE_NOT_AVAILABLE, sale.setItemPrice( "bananas", 1.89 ) );

    ASSERT_EQ( PointOfSale::OK, sale.setPerPoundPrice( "Ground Beef", 2.50 ) );
    ASSERT_EQ( PointOfSale::OK, sale.addItemWeight( "Ground Beef", 2.3 ) );
    ASSERT_EQ( PointOfSale::PRICE_UPDATE_NOT_AVAILABLE, sale.setPerPoundPrice( "Ground Beef", 3.89 ) );

}

TEST (PointOfSaleArgumentTest, setPriceInvalidSku){

    PointOfSale sale;
    ASSERT_EQ( PointOfSale::INVALID_SKU, sale.setItemPrice( "", 1.89 ) );
    ASSERT_EQ( PointOfSale::INVALID_SKU, sale.setPerPoundPrice( "", 2.50 ) );

}

TEST (PointOfSaleArgumentTest, addItemInvalidSku){

    PointOfSale sale;
    ASSERT_EQ( PointOfSale::INVALID_SKU, sale.addFixedPriceItem( "" ) );
    ASSERT_EQ( PointOfSale::INVALID_SKU, sale.addItemWeight( "", 2.50 ) );

}

TEST (PointOfSaleArgumentTest, addItemNegativeWeight){

    PointOfSale sale;
    sale.setItemPrice( "bananas", 2.50 );
    ASSERT_EQ( PointOfSale::INVALID_WEIGHT, sale.addItemWeight( "bananas", -1.0 ) );

}

TEST (PointOfSaleArgumentTest, addItemZeroWeight){

    PointOfSale sale;
    sale.setPerPoundPrice( "bananas", 3.50 );
    ASSERT_EQ( PointOfSale::INVALID_WEIGHT, sale.addItemWeight( "bananas", 0.0 ) );

}

TEST (PointOfSaleArgumentTest, setPriceThenAddItemOfConflictingType){

    PointOfSale fixed_then_weight_sale;

    // Set a price as if item is a fixed price
    ASSERT_EQ( PointOfSale::OK, fixed_then_weight_sale.setItemPrice( "bananas", 1.0 ) );

    // attempt to item to cart as if its a weight based item
    ASSERT_EQ( PointOfSale::ITEM_CONFLICT, fixed_then_weight_sale.addItemWeight( "bananas", 1.0 ) );

}

TEST (PointOfSaleArgumentTest, addItemBeforeSetPrice){

    PointOfSale fixed_sale;
    PointOfSale weight_sale;

    // add bananas as if they are fixed price item
    ASSERT_EQ( PointOfSale::NO_PRICE_DEFINED, fixed_sale.addFixedPriceItem( "bananas" ) );

    // add item as if they are a weight based item
    ASSERT_EQ( PointOfSale::NO_PRICE_DEFINED, weight_sale.addItemWeight( "Ground Beef", 1.0 ) );

}

TEST_F (PreDefinedPricesFixture, removeItemFromEmptyCart ){

    ASSERT_EQ( PointOfSale::ITEM_NOT_IN_CART, pSale->removeItem( "Cookies" ) );
    ASSERT_EQ( PointOfSale::ITEM_NOT_IN_CART, pSale->removeItemWeight( "Bananas", 1.0 ) );

}

TEST_F (PreDefinedPricesFixture, removeMoreItemsThanAddedToCart ){

    // add some initial items into the cart
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" ));
    ASSERT_EQ( PointOfSale::OK, pSale->addItemWeight("Bananas", 27.0 ));

    // remove the fixed price item from the cart
    ASSERT_EQ( PointOfSale::OK, pSale->removeItem( "Cookies" ) );
    ASSERT_EQ( PointOfSale::ITEM_NOT_IN_CART, pSale->removeItem( "Cookies" ) );

    // remove content from the weighted item
    ASSERT_EQ( PointOfSale::OK, pSale->removeItemWeight( "Bananas", 20.0 ) );
    ASSERT_EQ( PointOfSale::OK, pSale->removeItemWeight( "Bananas", 5.0 ) );
    ASSERT_EQ( PointOfSale::ITEM_NOT_IN_CART, pSale->removeItemWeight( "Bananas", 3.0 ) );
}

TEST_F (PreDefinedPricesFixture, removeItemsInvalidSKU ){

    ASSERT_EQ( PointOfSale::INVALID_SKU, pSale->removeItem( "" ) );
    ASSERT_EQ( PointOfSale::INVALID_SKU, pSale->removeItemWeight( "", 20.0 ) );

}

TEST_F (PreDefinedPricesFixture, removeItemsConflictingTypes ){

    ASSERT_EQ( PointOfSale::ITEM_CONFLICT, pSale->removeItem( "Bananas" ) );
    ASSERT_EQ( PointOfSale::ITEM_CONFLICT, pSale->removeItemWeight( "Cookies", 20.0 ) );

}

TEST_F (PreDefinedPricesFixture, removeItemNegativeWeight){

    ASSERT_EQ( PointOfSale::OK, pSale->addItemWeight("Bananas", 5.0));
    ASSERT_EQ( PointOfSale::INVALID_WEIGHT, pSale->removeItemWeight( "Bananas", -1.0 ));

}

// TODO - Need to add tests covering the removeItem functions