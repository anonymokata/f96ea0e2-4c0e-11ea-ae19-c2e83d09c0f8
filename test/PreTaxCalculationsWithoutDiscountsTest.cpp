#include "gtest/gtest.h"
#include "PointOfSale.h"

// Pull in the test fixture that contains already programmed prices for items in cart
#include "PreDefinedPricesFixture.h"

TEST_F( PreDefinedPricesFixture, MultipleFixedPriceItemsNoRemovals ){

    int index = 0;
    for( index = 0; index < 3; index++ ){
        pSale->addFixedPriceItem( "Soup" );
    }

    pSale->addFixedPriceItem( "Cookies" );
    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 5.20 );
}

TEST_F( PreDefinedPricesFixture, MultipleWeightBasedItemsNoRemovals ){

    pSale->addItemWeight( "Apples", 3.0 );
    pSale->addItemWeight( "Beef", 2.34 );
    pSale->addItemWeight( "Bananas", 12.0 );
    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 24.30 );
}

TEST_F( PreDefinedPricesFixture, MultipleMixedItemsWithDuplicatesNoRemovals ){

    // add the fixed price items
    pSale->addFixedPriceItem("Cookies");     // 2.26
    pSale->addFixedPriceItem("Milk");        // 2.53
    pSale->addFixedPriceItem("Soup");        // 0.98

    // add the weight based items
    pSale->addItemWeight( "Apples", 3.0 );   // 4.23
    pSale->addItemWeight( "Beef", 2.34 );    // 8.19
    pSale->addItemWeight( "Bananas", 12.0 ); // 11.88

    // add the same item again to increase amount in cart
    pSale->addItemWeight( "Apples", 2.3 );   // 3.24
    pSale->addFixedPriceItem( "Cookies" );   // 2.26

    // add more content of the weight based items to show it increments correctly
    pSale->addItemWeight( "Beef", 2.34 );    // 8.19
    pSale->addItemWeight( "Bananas", 12.0 ); // 11.88

    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 55.64 );
}

TEST_F( PreDefinedPricesFixture, fixedPriceItemsWithRemoval ){

    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" ));
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" ));
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" )); // 6.78

    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Soup" ));
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Soup" )); // 1.96

    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 8.74 );

    ASSERT_EQ( PointOfSale::OK, pSale->removeItem( "Cookies" ));
    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 6.48 );

    ASSERT_EQ( PointOfSale::OK, pSale->removeItem( "Soup" ));
    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 5.50 );
}

TEST_F( PreDefinedPricesFixture, weightBasedItemsWithRemoval ){

    ASSERT_EQ( PointOfSale::OK, pSale->addItemWeight( "Apples", 5.43 )); // 7.65
    ASSERT_EQ( PointOfSale::OK, pSale->addItemWeight( "Beef", 6.8 ));    // 23.8
    ASSERT_EQ( PointOfSale::OK, pSale->addItemWeight( "Bananas", 12.2 ));// 12.07
    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 43.52 );

    ASSERT_EQ( PointOfSale::OK, pSale->removeItemWeight( "Apples", 1.43 )); // 5.64
    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 41.51 );

    ASSERT_EQ( PointOfSale::OK, pSale->removeItemWeight( "Beef", 5.0 )); // 6.30
    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 24.01);
}

TEST_F( PreDefinedPricesFixture, mixedItemsWithRemovals ){

    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" ));
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" ));
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" )); // 6.78
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Soup" ));
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Soup" )); // 1.96

    ASSERT_EQ( PointOfSale::OK, pSale->addItemWeight( "Apples", 5.43 )); // 7.65
    ASSERT_EQ( PointOfSale::OK, pSale->addItemWeight( "Beef", 6.8 ));    // 23.8
    ASSERT_EQ( PointOfSale::OK, pSale->addItemWeight( "Bananas", 12.2 ));// 12.07
    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 52.26 );

    ASSERT_EQ( PointOfSale::OK, pSale->removeItem( "Cookies" ));
    ASSERT_EQ( PointOfSale::OK, pSale->removeItemWeight( "Apples", 1.43 )); // 5.64
    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 47.99);
}
