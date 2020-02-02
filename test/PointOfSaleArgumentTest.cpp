#include "gtest/gtest.h"
#include "PointOfSale.h"

TEST (PointOfSaleArgumentTest, setNegativePrice){

    PointOfSale sale;
    ASSERT_EQ( INVALID_PRICE, sale.setItemPrice( "bananas", -1.0 ) );
    ASSERT_EQ( INVALID_PRICE, sale.setPerPoundPrice( "Ground Beef", -1.0 ) );

}

TEST (PointOfSaleArgumentTest, setZeroPrice){

    PointOfSale sale;
    ASSERT_EQ( INVALID_PRICE, sale.setItemPrice( "bananas", 0.0 ) );
    ASSERT_EQ( INVALID_PRICE, sale.setPerPoundPrice( "Ground Beef", 0.0 ) );

}

TEST (PointOfSaleArgumentTest, setValidPrice){

    PointOfSale sale;
    ASSERT_EQ( OK, sale.setItemPrice( "bananas", 1.89 ) );
    ASSERT_EQ( OK, sale.setPerPoundPrice( "Ground Beef", 2.50 ) );

}

TEST (PointOfSaleArgumentTest, setConflictingPrices){

    PointOfSale fixed_sale_first;
    PointOfSale weight_sale_first;
    
    ASSERT_EQ( OK, fixed_sale_first.setItemPrice( "bananas", 2.50 ) );
    ASSERT_EQ( PRICING_CONFLICT, fixed_sale_first.setPerPoundPrice( "bananas", 4.50 ) );

    ASSERT_EQ( OK, weight_sale_first.setPerPoundPrice( "bananas", 4.50 ) );
    ASSERT_EQ( PRICING_CONFLICT, weight_sale_first.setItemPrice( "bananas", 2.50 ) );

}

// TODO - Ensure that a test is created that has prices being updated in middle of adding items to cart. Need to ensure that this
//        is not allowed per the project writeup that states that prices will be configured before adding items to cart.
TEST (PointOfSaleArgumentTest, updateItemPrice){

    PointOfSale sale;
    ASSERT_EQ( OK, sale.setItemPrice( "bananas", 1.89 ) );
    ASSERT_EQ( OK, sale.setItemPrice( "bananas", 2.50 ) );

    ASSERT_EQ( OK, sale.setPerPoundPrice( "Ground Beef", 2.50 ) );
    ASSERT_EQ( OK, sale.setPerPoundPrice( "Ground Beef", 3.89 ) );

}

TEST (PointOfSaleArgumentTest, setPriceInvalidSku){

    PointOfSale sale;
    ASSERT_EQ( INVALID_SKU, sale.setItemPrice( "", 1.89 ) );
    ASSERT_EQ( INVALID_SKU, sale.setPerPoundPrice( "", 2.50 ) );

}

TEST (PointOfSaleArgumentTest, addItemInvalidSku){

    PointOfSale sale;
    ASSERT_EQ( INVALID_SKU, sale.addItem( "" ) );
    ASSERT_EQ( INVALID_SKU, sale.addItem( "", 2.50 ) );

}

TEST (PointOfSaleArgumentTest, addItemNegativeWeight){

    PointOfSale sale;
    sale.setItemPrice( "bananas", 2.50 );
    ASSERT_EQ( INVALID_WEIGHT, sale.addItem( "bananas", -1.0 ) );

}

TEST (PointOfSaleArgumentTest, addItemZeroWeight){

    PointOfSale sale;
    sale.setPerPoundPrice( "bananas", 3.50 );
    ASSERT_EQ( INVALID_WEIGHT, sale.addItem( "bananas", 0.0 ) );

}

TEST (PointOfSaleArgumentTest, setPriceThenAddItemOfConflictingType){

    PointOfSale fixed_then_weight_sale;

    // Set a price as if item is a fixed price
    ASSERT_EQ( OK, fixed_then_weight_sale.setItemPrice( "bananas", 1.0 ) );

    // attempt to item to cart as if its a weight based item
    ASSERT_EQ( ITEM_CONFLICT, fixed_then_weight_sale.addItem( "bananas", 1.0 ) );

}

TEST (PointOfSaleArgumentTest, addItemBeforeSetPrice){

    PointOfSale fixed_sale;
    PointOfSale weight_sale;

    // add bananas as if they are fixed price item
    ASSERT_EQ( NO_PRICE_DEFINED, fixed_sale.addItem( "bananas" ) );

    // add item as if they are a weight based item
    ASSERT_EQ( NO_PRICE_DEFINED, weight_sale.addItem( "Ground Beef", 1.0 ) );

}