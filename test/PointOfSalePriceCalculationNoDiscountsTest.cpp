#include "gtest/gtest.h"
#include "PointOfSale.h"

class PreDefinedPricesFixture : public ::testing::Test { 
    
protected: 

   void SetUp( ) override
   {
       pSale = new PointOfSale();

       // Add prices for all the fixed price items that will be utilized in the tests
       pSale->setItemPrice( "Soup",    0.98 );
       pSale->setItemPrice( "Chips",   3.98 );
       pSale->setItemPrice( "Cookies", 2.26 );
       pSale->setItemPrice( "Milk",    2.53 );

       // Add prices for all the items that are sold on a per pound basis
       pSale->setPerPoundPrice( "Apples",  1.41 );
       pSale->setPerPoundPrice( "Bananas", 0.99 );
       pSale->setPerPoundPrice( "Beef",    3.50 );
   }

   void TearDown( ) override
   {
       delete pSale;
       pSale = 0;
   }

   // This pointer will be allocated as part of the SetUp function and released as part of the TearDown function
   PointOfSale *pSale;
};

TEST_F( PreDefinedPricesFixture, MultipleFixedPriceItemsNoRemovals ){

    int index = 0;
    for( index = 0; index < 3; index++ ){
        pSale->addToCart( "Soup", 1 );
    }

    pSale->addToCart( "Cookies", 1 );
    ASSERT_NEAR( pSale->getPreTaxTotal(), 5.20, 0.01 );
}

TEST_F( PreDefinedPricesFixture, MultipleWeightBasedItemsNoRemovals ){

    pSale->addToCart( "Apples", 3.0 );
    pSale->addToCart( "Beef", 2.34 );
    pSale->addToCart( "Bananas", 12.0 );
    ASSERT_NEAR( pSale->getPreTaxTotal(), 24.30, .01 );
}

TEST_F( PreDefinedPricesFixture, MultipleMixedItemsWithDuplicatesNoRemovals ){

    // add the fixed price items
    pSale->addToCart("Cookies", 1);     // 2.26
    pSale->addToCart("Milk",    1);        // 2.53
    pSale->addToCart("Soup",    1);        // 0.98

    // add the weight based items
    pSale->addToCart( "Apples", 3.0 );   // 4.23
    pSale->addToCart( "Beef", 2.34 );    // 8.19
    pSale->addToCart( "Bananas", 12.0 ); // 11.88

    // add the same item again to increase amount in cart
    pSale->addToCart( "Apples", 2.3 );   // 3.24
    pSale->addToCart( "Cookies", 1 );   // 2.26

    // add more content of the weight based items to show it increments correctly
    pSale->addToCart( "Beef", 2.34 );    // 8.19
    pSale->addToCart( "Bananas", 12.0 ); // 11.88

    ASSERT_NEAR( pSale->getPreTaxTotal(), 55.64, .01 );
}

TEST_F( PreDefinedPricesFixture, fixedPriceItemsWithRemoval ){

    ASSERT_EQ( OK, pSale->addToCart( "Cookies", 1 ));
    ASSERT_EQ( OK, pSale->addToCart( "Cookies", 1 ));
    ASSERT_EQ( OK, pSale->addToCart( "Cookies", 1 )); // 6.78

    ASSERT_EQ( OK, pSale->addToCart( "Soup", 1 ));
    ASSERT_EQ( OK, pSale->addToCart( "Soup", 1 )); // 1.96

    ASSERT_NEAR( pSale->getPreTaxTotal(), 8.74, .01 );

    ASSERT_EQ( OK, pSale->removeFromCart( "Cookies", 1 ));
    ASSERT_NEAR( pSale->getPreTaxTotal(), 6.48, .01 );

    ASSERT_EQ( OK, pSale->removeFromCart( "Soup", 1 ));
    ASSERT_NEAR( pSale->getPreTaxTotal(), 5.50, .01 );
}

TEST_F( PreDefinedPricesFixture, weightBasedItemsWithRemoval ){

    ASSERT_EQ( OK, pSale->addToCart( "Apples", 5.43 )); // 7.66
    ASSERT_EQ( OK, pSale->addToCart( "Beef", 6.8 ));    // 23.8
    ASSERT_EQ( OK, pSale->addToCart( "Bananas", 12.2 ));// 12.08
    ASSERT_NEAR( pSale->getPreTaxTotal(), 43.54, .01 );

    ASSERT_EQ( OK, pSale->removeFromCart( "Apples", 1.43 )); // 5.64
    ASSERT_NEAR( pSale->getPreTaxTotal(), 41.51, .01 );

    ASSERT_EQ( OK, pSale->removeFromCart( "Beef", 5.0 )); // 6.30
    ASSERT_NEAR( pSale->getPreTaxTotal(), 24.01, .01);
}

TEST_F( PreDefinedPricesFixture, mixedItemsWithRemovals ){

    ASSERT_EQ( OK, pSale->addToCart( "Cookies", 1 ));
    ASSERT_EQ( OK, pSale->addToCart( "Cookies", 1 ));
    ASSERT_EQ( OK, pSale->addToCart( "Cookies", 1 )); // 6.78
    ASSERT_EQ( OK, pSale->addToCart( "Soup", 1 ));
    ASSERT_EQ( OK, pSale->addToCart( "Soup", 1 )); // 1.96

    ASSERT_EQ( OK, pSale->addToCart( "Apples", 5.43 )); // 7.66
    ASSERT_EQ( OK, pSale->addToCart( "Beef", 6.8 ));    // 23.8
    ASSERT_EQ( OK, pSale->addToCart( "Bananas", 12.2 ));// 12.08
    ASSERT_NEAR( pSale->getPreTaxTotal(), 52.28, .01 );

    ASSERT_EQ( OK, pSale->removeFromCart( "Cookies", 1 ));
    ASSERT_EQ( OK, pSale->removeFromCart( "Apples", 1.43 )); // 5.64
    ASSERT_NEAR( pSale->getPreTaxTotal(), 47.99, .01);
}
