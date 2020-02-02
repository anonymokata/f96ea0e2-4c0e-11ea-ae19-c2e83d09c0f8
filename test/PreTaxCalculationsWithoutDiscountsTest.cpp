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
    pSale->addFixedPriceItem("Cookies");
    pSale->addFixedPriceItem("Milk");
    pSale->addFixedPriceItem("Soup");

    // add the weight based items
    pSale->addItemWeight( "Apples", 3.0 );
    pSale->addItemWeight( "Beef", 2.34 );
    pSale->addItemWeight( "Bananas", 12.0 );

    // add the same item again to increase amount in cart
    pSale->addFixedPriceItem( "Apples", 2.3 );
    pSale->addFixedPriceItem( "Cookies" );

    // add more content of the weight based items to show it increments correctly
    pSale->addItemWeight( "Beef", 2.34 );
    pSale->addItemWeight( "Bananas", 12.0 );

    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 24.30 );
}


// TODO - Add tests mixing fixed and by weight items in single checkout
// TODO - Add tests associatd with removing items from checkout and ensure correct result is calculated
// TODO - Add test for removing an item that hasn't been added to the cart
