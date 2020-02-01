#include "gtest/gtest.h"
#include "PointOfSale.h"

TEST (PointOfSaleTest, setNegativePrice){

    PointOfSale sale;
    EXPECT_EQ( INVALID_PRICE, sale.setItemPrice( "bananas", -1.0 ) );
    EXPECT_EQ( INVALID_PRICE, sale.setPerPoundPrice( "Ground Beef", -1.0 ) );

}

TEST (PointOfSaleTest, setZeroPrice){

    PointOfSale sale;
    EXPECT_EQ( INVALID_PRICE, sale.setItemPrice( "bananas", 0.0 ) );
    EXPECT_EQ( INVALID_PRICE, sale.setPerPoundPrice( "Ground Beef", 0.0 ) );

}

TEST (PointOfSaleTest, setValidPrice){

    PointOfSale sale;
    EXPECT_EQ( OK, sale.setItemPrice( "bananas", 1.89 ) );
    EXPECT_EQ( OK, sale.setPerPoundPrice( "Ground Beef", 2.50 ) );

}

TEST (PointOfSaleTest, setConflictingPrices){

    PointOfSale fixed_sale_first;
    PointOfSale weight_sale_first;
    
    EXPECT_EQ( OK, fixed_sale_first.setItemPrice( "bananas", 2.50 ) );
    EXPECT_EQ( PRICING_CONFLICT, fixed_sale_first.setPerPoundPrice( "bananas", 4.50 ) );

    EXPECT_EQ( OK, weight_sale_first.setPerPoundPrice( "bananas", 4.50 ) );
    EXPECT_EQ( PRICING_CONFLICT, fixed_sale_first.setItemPrice( "bananas", 2.50 ) );

}

// TODO - Ensure that a test is created that has prices being updated in middle of adding items to cart. Need
//        to verify that price is updated correctly in middle of accepting items.
TEST (PointOfSaleTest, updateItemPrice){

    PointOfSale sale;
    EXPECT_EQ( OK, sale.setItemPrice( "bananas", 1.89 ) );
    EXPECT_EQ( OK, sale.setItemPrice( "bananas", 2.50 ) );

    EXPECT_EQ( OK, sale.setPerPoundPrice( "Ground Beef", 2.50 ) );
    EXPECT_EQ( OK, sale.setPerPoundPrice( "Ground Beef", 3.89 ) );

}

TEST (PointOfSaleTest, setPriceInvalidSku){

    PointOfSale sale;
    EXPECT_EQ( INVALID_SKU, sale.setItemPrice( "", 1.89 ) );
    EXPECT_EQ( INVALID_SKU, sale.setPerPoundPrice( "", 2.50 ) );

}

TEST (PointOfSaleTest, addItemInvalidSku){

    PointOfSale sale;
    EXPECT_EQ( INVALID_SKU, sale.addItem( "" ) );
    EXPECT_EQ( INVALID_SKU, sale.addItem( "", 2.50 ) );

}

TEST (PointOfSaleTest, addItemNegativeWeight){

    PointOfSale sale;
    sale.setItemPrice( "bananas", 2.50 );
    EXPECT_EQ( INVALID_WEIGHT, sale.addItem( "bananas", -1.0 ) );

}

TEST (PointOfSaleTest, addItemZeroWeight){

    PointOfSale sale;
    sale.setPerPoundPrice( "bananas", 3.50 );
    EXPECT_EQ( INVALID_WEIGHT, sale.addItem( "bananas", 0.0 ) );

}

TEST (PointOfSaleTest, setPriceThenAddItemOfConflictingType){

    PointOfSale fixed_then_weight_sale;

    // Set a price as if item is a fixed price
    EXPECT_EQ( OK, fixed_then_weight_sale.setItemPrice( "bananas", 1.0 ) );

    // attempt to item to cart as if its a weight based item
    EXPECT_EQ( ITEM_CONFLICT, fixed_then_weight_sale.addItem( "bananas", 1.0 ) );

}

TEST (PointOfSaleTest, addItemBeforeSetPrice){

    PointOfSale fixed_sale;
    PointOfSale weight_sale;

    // add bananas as if they are fixed price item
    EXPECT_EQ( NO_PRICE_DEFINED, fixed_sale.addItem( "bananas" ) );

    // add item as if they are a weight based item
    EXPECT_EQ( NO_PRICE_DEFINED, weight_sale.addItem( "Ground Beef", 1.0 ) );

}