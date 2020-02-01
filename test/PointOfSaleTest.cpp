#include "gtest/gtest.h"
#include "PointOfSale.h"

TEST (PointOfSaleTest, setNegativePrice){

    PointOfSale sale;
    EXPECT_EQ( INVALID_PRICE, sale.setItemPrice( "bananas", -1.0 ) );
    EXPECT_EQ( INVALID_PRICE, sale.setPerPoundPrice( "Ground Beef", -1.0 ) );

}

TEST (PointOfSaleTest, setZeroPrice){

    PointOfSale sale;
    EXPECT_EQ( OK, sale.setItemPrice( "bananas", 0.0 ) );
    EXPECT_EQ( OK, sale.setPerPoundPrice( "Ground Beef", 0.0 ) );

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

TEST (PointOfSaleTest, invalidSku){

    PointOfSale sale;
    EXPECT_EQ( INVALID_SKU, sale.setItemPrice( "", 1.89 ) );
    EXPECT_EQ( INVALID_SKU, sale.setPerPoundPrice( "", 2.50 ) );

}