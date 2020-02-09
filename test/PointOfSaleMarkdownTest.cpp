#include "gtest/gtest.h"
#include "PointOfSale.h"

class MarkdownTestFixture : public ::testing::Test { 
    
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

TEST_F (MarkdownTestFixture, setMarkdownNegativePrice){

    ASSERT_EQ( INVALID_PRICE, pSale->setMarkdown( "Bananas", -1.0 ));

}

TEST_F (MarkdownTestFixture, settingMarkdownBeforePrice){

    ASSERT_EQ( NO_PRICE_DEFINED, pSale->setMarkdown( "Steak", 0.2));
}

TEST_F (MarkdownTestFixture, setMarkdownTooLargeMarkdown){

    ASSERT_EQ( INVALID_PRICE, pSale->setMarkdown( "Bananas", 1.0 ));

}

TEST_F (MarkdownTestFixture, setMarkdownMultipleTimes){

    ASSERT_EQ( OK, pSale->setMarkdown( "Bananas", 0.20 ));
    ASSERT_EQ( OK, pSale->setMarkdown( "Bananas", 0.30 ));

}

TEST_F (MarkdownTestFixture, setMarkdownAfterItemToCart){

    ASSERT_EQ( OK, pSale->setMarkdown( "Bananas", 0.20 ));
    ASSERT_EQ( OK, pSale->addToCart( "Bananas", 2.34));
    ASSERT_EQ( PRICE_UPDATE_NOT_AVAILABLE, pSale->setMarkdown( "Bananas", 0.30 ));

}

TEST_F (MarkdownTestFixture, setMarkdownFixedPrice){

    ASSERT_EQ( OK, pSale->setMarkdown( "Soup", 0.20 ));

    ASSERT_EQ( OK, pSale->addToCart( "Cookies", 1 ));
    ASSERT_EQ( OK, pSale->addToCart( "Cookies", 1 ));
    ASSERT_EQ( OK, pSale->addToCart( "Soup", 1 ));

    ASSERT_NEAR( pSale->getPreTaxTotal(), 5.3, .01);

}

TEST_F (MarkdownTestFixture, setMarkdownFixedPriceDualItems){

    ASSERT_EQ( OK, pSale->setMarkdown( "Cookies", 0.26 ));

    ASSERT_EQ( OK, pSale->addToCart( "Cookies", 1 ));
    ASSERT_EQ( OK, pSale->addToCart( "Cookies", 1 ));
    ASSERT_EQ( OK, pSale->addToCart( "Soup", 1 ));

    ASSERT_NEAR( pSale->getPreTaxTotal(), 4.98, .01);

}

TEST_F (MarkdownTestFixture, setMarkdownWeightBasedMarkdownSingle){

    ASSERT_EQ( OK, pSale->setMarkdown( "Apples", 0.41 ));
    ASSERT_EQ( OK, pSale->addToCart( "Apples", 2.0 ));
    ASSERT_NEAR( pSale->getPreTaxTotal(), 2.0, .01);

}

TEST_F (MarkdownTestFixture, setMarkdownMixedItemsMarkdown){

    ASSERT_EQ( OK, pSale->setMarkdown( "Apples",  0.41 ));
    ASSERT_EQ( OK, pSale->setMarkdown( "Cookies", 0.26 ));

    ASSERT_EQ( OK, pSale->addToCart( "Apples", 2.0 )); // 2.0
    ASSERT_EQ( OK, pSale->addToCart( "Cookies", 1 )); // 2.0
    ASSERT_EQ( OK, pSale->addToCart( "Cookies", 1 )); // 2.0
    ASSERT_EQ( OK, pSale->addToCart( "Soup", 1 ));

    ASSERT_NEAR( pSale->getPreTaxTotal(), 6.98, .01);

}