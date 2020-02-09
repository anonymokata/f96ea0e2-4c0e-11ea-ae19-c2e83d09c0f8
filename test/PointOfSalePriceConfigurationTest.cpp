#include "gtest/gtest.h"
#include "PointOfSale.h"

TEST (PriceConfigurationTest, setNegativePrice){

    PointOfSale sale;
    ASSERT_EQ( INVALID_PRICE, sale.setItemPrice( "bananas", -1.0 ) );
    ASSERT_EQ( INVALID_PRICE, sale.setPerPoundPrice( "Ground Beef", -1.0 ) );

}

TEST (PriceConfigurationTest, setZeroPrice){

    PointOfSale sale;
    ASSERT_EQ( INVALID_PRICE, sale.setItemPrice( "bananas", 0.0 ) );
    ASSERT_EQ( INVALID_PRICE, sale.setPerPoundPrice( "Ground Beef", 0.0 ) );

}

TEST (PriceConfigurationTest, setValidPrice){

    PointOfSale sale;
    ASSERT_EQ( OK, sale.setItemPrice( "bananas", 1.89 ) );
    ASSERT_EQ( OK, sale.setPerPoundPrice( "Ground Beef", 2.50 ) );

}

TEST (PriceConfigurationTest, setConflictingPrices){

    PointOfSale fixed_sale_first;
    PointOfSale weight_sale_first;
    
    ASSERT_EQ( OK, fixed_sale_first.setItemPrice( "bananas", 2.50 ) );
    ASSERT_EQ( ITEM_CONFLICT, fixed_sale_first.setPerPoundPrice( "bananas", 4.50 ) );

    ASSERT_EQ( OK, weight_sale_first.setPerPoundPrice( "bananas", 4.50 ) );
    ASSERT_EQ( ITEM_CONFLICT, weight_sale_first.setItemPrice( "bananas", 2.50 ) );

}

TEST (PriceConfigurationTest, updateItemPrice){

    PointOfSale sale;
    ASSERT_EQ( OK, sale.setItemPrice( "bananas", 1.89 ) );
    ASSERT_EQ( OK, sale.setItemPrice( "bananas", 2.50 ) );

    ASSERT_EQ( OK, sale.setPerPoundPrice( "Ground Beef", 2.50 ) );
    ASSERT_EQ( OK, sale.setPerPoundPrice( "Ground Beef", 3.89 ) );

}

TEST (PriceConfigurationTest, updateItemPriceAfterItemInCart){

    PointOfSale sale;
    ASSERT_EQ( OK, sale.setItemPrice( "bananas", 1.89 ) );
    ASSERT_EQ( OK, sale.addToCart( "bananas", 1 ) );
    ASSERT_EQ( PRICE_UPDATE_NOT_AVAILABLE, sale.setItemPrice( "bananas", 1.89 ) );

    ASSERT_EQ( OK, sale.setPerPoundPrice( "Ground Beef", 2.50 ) );
    ASSERT_EQ( OK, sale.addToCart( "Ground Beef", 2.3 ) );
    ASSERT_EQ( PRICE_UPDATE_NOT_AVAILABLE, sale.setPerPoundPrice( "Ground Beef", 3.89 ) );

}

TEST (PriceConfigurationTest, setPriceInvalidSku){

    PointOfSale sale;
    ASSERT_EQ( INVALID_SKU, sale.setItemPrice( "", 1.89 ) );
    ASSERT_EQ( INVALID_SKU, sale.setPerPoundPrice( "", 2.50 ) );

}

TEST (PriceConfigurationTest, addItemInvalidSku){

    PointOfSale sale;
    ASSERT_EQ( INVALID_SKU, sale.addToCart( "", 1 ) );
    ASSERT_EQ( INVALID_SKU, sale.addToCart( "", 2.50 ) );

}
