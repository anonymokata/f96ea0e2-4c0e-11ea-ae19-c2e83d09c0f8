#include "gtest/gtest.h"
#include "PointOfSale.h"

class PriceCalculationWithDiscountsTest : public ::testing::Test { 
    
protected: 

   void SetUp( ) override
   {
       pSale = new PointOfSale();

       // Add prices for all the fixed price items that will be utilized in the tests
       pSale->setItemPrice( "Soup",    1.50 );
       pSale->setItemPrice( "Chips",   2.00 );
       pSale->setItemPrice( "Cookies", 3.5 );

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

TEST_F (PriceCalculationWithDiscountsTest, invalidSku){

    ASSERT_EQ( INVALID_SKU, pSale->applyGetXForYDiscount( "", 1, 2.0) );
    ASSERT_EQ( INVALID_SKU, pSale->applyGetXForYDiscount( "", 1, 2.0, 2) );

    ASSERT_EQ( INVALID_SKU, pSale->applyBuyXGetYAtDiscount( "", 1, 2, .5 ) );
    ASSERT_EQ( INVALID_SKU, pSale->applyBuyXGetYAtDiscount( "", 1, 2, .5, 4 ) );
    ASSERT_EQ( INVALID_SKU, pSale->applyBuyXGetYAtDiscount( "", 1.0, 2.0, .5 ) );
    ASSERT_EQ( INVALID_SKU, pSale->applyBuyXGetYAtDiscount( "", 1.0, 2.0, .5, 1.0 ) );

}

TEST_F (PriceCalculationWithDiscountsTest, applyDiscountsNegativeValues){

    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyGetXForYDiscount( "Soup", -1, 2.0) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyGetXForYDiscount( "Soup", 1, -2.0) );

    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyGetXForYDiscount( "Soup", -1, 2.0, 2) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyGetXForYDiscount( "Soup", 1, -2.0, 2) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyGetXForYDiscount( "Soup", 1, 2.0, -2) );

    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Soup", -1, 2, .5 ) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Soup", 1, -2, .5 ) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Soup", 1, 2, -.5 ) );
    
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Soup", -1, 2, .5, 1 ) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Soup", 1, -2, .5, 1 ) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Soup", 1, 2, -.5, 1 ) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Soup", 1, 2, .5, -1 ) );

    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Apples", -1.0, 2.0, .5 ) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Apples", 1.0, -2.0, .5 ) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Apples", 1.0, 2.0, -.5 ) );
    
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Apples", -1.0, 2.0, .5, 1.0 ) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Apples", 1.0, -2.0, .5, 1.0 ) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Apples", 1.0, 2.0, -.5, 1.0 ) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Apples", 1.0, 2.0, .5, -1.0 ) );
}

TEST_F (PriceCalculationWithDiscountsTest, applyDiscountsInvalidPercentages){

    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Soup", 1, 2, 1.01 ) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Soup", 1, 2, 1.01, 2 ) );

    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Beef", 1.0, 2.0, 1.01 ) );
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Beef", 1.0, 2.0, 1.01, 2.0 ) );

}

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffPerfectRoundMixedNoMarkdown){

    pSale->addToCart( "Chips", 6 );
    pSale->addToCart( "Beef", 6.0 );
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Chips", 4, 2, 0.5 )); // 8 + 2
    ASSERT_NEAR( pSale->getPreTaxTotal(), 31.0, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffPerfectRoundMixedWithMarkdown){

    pSale->setMarkdown( "Beef", 1.5 );
    pSale->addToCart( "Chips", 6 );
    pSale->addToCart( "Beef", 6.0 );
    pSale->applyBuyXGetYAtDiscount( "Chips", 4, 2, 0.5 ); // 8 + 2
    ASSERT_NEAR( pSale->getPreTaxTotal(), 22.0, .01 );

}

TEST_F (PriceCalculationWithDiscountsTest, applyDiscountsItemTypeConflicts){

    ASSERT_EQ( ITEM_CONFLICT, pSale->applyBuyXGetYAtDiscount( "Bananas", 4, 2, 0.5 ) );
    ASSERT_EQ( ITEM_CONFLICT, pSale->applyBuyXGetYAtDiscount( "Chips", 4.0, 2.0, 0.5 ) );

}