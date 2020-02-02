#include "gtest/gtest.h"
#include "PointOfSale.h"

// : public ::testing::test
class PreDefinedPricesFixture : public ::testing::Test { 
    
protected: 

   void SetUp( ) override { 
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

   void TearDown( ) override { 
       delete pSale;
       pSale = 0;
   }

   // put in any custom data members that you need 
   PointOfSale *pSale;
};

TEST_F( PreDefinedPricesFixture, MultipleFixedPriceItemsNoRemovals ){

    int index = 0;
    for( index = 0; index < 3; index++ ){
        pSale->addItem( "Soup" );
    }

    pSale->addItem( "Cookies" );
    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 5.20 );
}

TEST_F( PreDefinedPricesFixture, MultipleWeightBasedItemsNoRemovals ){

    pSale->addItem( "Apples", 3.0 );
    pSale->addItem( "Beef", 2.34 );
    pSale->addItem( "Bananas", 12.0 );
    ASSERT_DOUBLE_EQ( pSale->getPreTaxTotal(), 24.30 );
}

// TODO - Add tests mixing fixed and by weight items in single checkout
// TODO - Add tests associatd with removing items from checkout and ensure correct result is calculated
// TODO - Add test for removing an item that hasn't been added to the cart
