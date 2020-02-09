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

///////////////////////////////////////////////////////////////////////////////
//                            ARGUMENT Checking
///////////////////////////////////////////////////////////////////////////////

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

TEST_F (PriceCalculationWithDiscountsTest, applyDiscountsItemTypeConflicts){

    ASSERT_EQ( ITEM_CONFLICT, pSale->applyBuyXGetYAtDiscount( "Bananas", 4, 2, 0.5 ) );
    ASSERT_EQ( ITEM_CONFLICT, pSale->applyBuyXGetYAtDiscount( "Chips", 4.0, 2.0, 0.5 ) );

}

///////////////////////////////////////////////////////////////////////////////
//                        Buy X Items Get Y Items Off
///////////////////////////////////////////////////////////////////////////////

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffPerfectRoundMixedItemTypesNoMarkdown){

    pSale->addToCart( "Chips", 6 );
    pSale->addToCart( "Beef", 6.0 );

    // 4 * 2 = 8, 2 * 2 / 2 = 2
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Chips", 4, 2, 0.5 ));

    // 4 * 3.5 = 14, 2 * 3.5 = 7 / 2 = 3.5
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Beef", 4.0, 2.0, 0.5 ));

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 27.5, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffPerfectRoundMixedItemTypesAllMarkdown){

    pSale->setMarkdown( "Chips", 1.0 );
    pSale->setMarkdown( "Beef",  1.5 );

    pSale->addToCart( "Chips", 6 );
    pSale->addToCart( "Beef", 6.0 );

    // 4 * 1.0 = 4, 2 * 1.0 / 2, 1.0  (5.0)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Chips", 4, 2, 0.5 ) );

    // 4 * 2.0 = 8, 2 * 2.0 / 2 = 2 (10.0)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Beef", 4.0, 2.0, 0.5 ) );

    ASSERT_NEAR( pSale->getPreTaxTotal(), 15.0, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffPerfectRoundMixedItemTypesSomeMarkdown){

    pSale->setMarkdown( "Beef",  1.5 );

    pSale->addToCart( "Chips", 6 );
    pSale->addToCart( "Beef", 6.0 );

    // 4 * 2.0 = 8, 2 * 2.0 / 2, 2.0  (10.0)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Chips", 4, 2, 0.5 ) );

    // 4 * 2.0 = 8, 2 * 2.0 / 2 = 2 (10.0)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Beef", 4.0, 2.0, 0.5 ) );

    ASSERT_NEAR( pSale->getPreTaxTotal(), 20.0, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffMixedItemTypesNoMarkdown){

    pSale->addToCart( "Chips", 7 );
    pSale->addToCart( "Beef", 7.5 );

    // 4 * 2 = 8, 2 * 2 / 2 = 2, 1 * 2.0 (12.0)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Chips", 4, 2, 0.5 ));

    // 4 * 3.5 = 14, 2 * 3.5 = 7 / 2 = 3.5, 1.5 * 3.5 = 5.25 (22.75)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Beef", 4.0, 2.0, 0.5 ));

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 34.75, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffMixedItemTypesSomeMarkdown){

    ASSERT_EQ( OK, pSale->setMarkdown( "Beef", 0.5));

    // 4 * 2 = 8, 2 * 2 / 2 = 2, 1 * 2.0 (12.0)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Chips", 4, 2, 0.5 ));

    // 4 * 3.0 = 12, 2 * 3.0 = 6 / 2 = 3.0, 1.5 * 3.0 = 4.50 (19.50)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Beef", 4.0, 2.0, 0.5 ));

    pSale->addToCart( "Chips", 7 );
    pSale->addToCart( "Beef", 7.5 );

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 31.50, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffMixedItemTypesAllMarkdown){

    ASSERT_EQ( OK, pSale->setMarkdown( "Chips", 0.5 ));
    ASSERT_EQ( OK, pSale->setMarkdown( "Beef", 0.5 ));

    // 4 * 1.5 = 6, 2 * 1.5 / 2 = 1.5, 1 * 1.5 (9.0)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Chips", 4, 2, 0.5 ));

    // 4 * 3.0 = 12, 2 * 3.0 = 6 / 2 = 3.0, 1.5 * 3.0 = 4.50 (19.50)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Beef", 4.0, 2.0, 0.5 ));

    pSale->addToCart( "Chips", 7 );
    pSale->addToCart( "Beef", 7.5 );

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 28.50, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffInvalidLimit){

    // attempt to apply discount where limit is smaller than number item requied for discount to apply
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Chips", 4, 2, 0.5, 3 ));
    ASSERT_EQ( INVALID_DISCOUNT, pSale->applyBuyXGetYAtDiscount( "Beef", 4.0, 2.0, 0.5, 3.0 ));
}

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffMixedItemTypesSomeMarkdownWithLimit){

    ASSERT_EQ( OK, pSale->setMarkdown( "Beef", 0.5));

    // 4 * 2 = 8
    // 1 * 2.0 / 2 = 1
    // 2 * 2.0 = 4
    //        (13.0)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Chips", 4, 2, 0.5, 5 ));

    // 4 * 3.0 = 12
    // 1.0 * 3.0 / 2 = 1.5
    // 2.5 * 3.0 = 7.5
    //       (21.0)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Beef", 4.0, 2.0, 0.5, 5.0 ));

    pSale->addToCart( "Chips", 7 );
    pSale->addToCart( "Beef", 7.5 );

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 34.00, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffMixedItemTypesSomeMarkdownWithLimitOnBoundary){

    ASSERT_EQ( OK, pSale->setMarkdown( "Beef", 0.5));

    // 4 * 2 = 8
    // 2 * 2.0 / 2 = 2
    // 1 * 2.0 = 2
    //        (12.0)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Chips", 4, 2, 0.5, 6 ));

    // 4 * 3.0 = 12
    // 2.0 * 3.0 / 2 = 3
    // 1.5 * 3.0 = 4.5
    //       (19.5)
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Beef", 4.0, 2.0, 0.5, 6.0 ));

    pSale->addToCart( "Chips", 7 );
    pSale->addToCart( "Beef", 7.5 );

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 31.50, .01 );
}

///////////////////////////////////////////////////////////////////////////////
//                        Buy X Items for Z Dollars
///////////////////////////////////////////////////////////////////////////////

TEST_F (PriceCalculationWithDiscountsTest, buyNForZPerfectRoundMixedItemTypesNoMarkdown){

    pSale->addToCart( "Chips", 6 );
    pSale->addToCart( "Beef", 6.0 ); // 6.0 * 3.5 = 21

    // 6 / 3 = 2 * 1.5 = 3.0
    ASSERT_EQ( OK, pSale->applyGetXForYDiscount( "Chips", 3, 1.5 ));

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 24.0, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNForZPerfectRoundMixedItemTypesSomeMarkdown){

    // ensures taht markdown has no affect on price given by discount
    ASSERT_EQ( OK, pSale->setMarkdown( "Chips", 0.5 ) );

    pSale->addToCart( "Chips", 6 );
    pSale->addToCart( "Beef", 6.0 ); // 6.0 * 3.5 = 21

    // 6 / 3 = 2 * 1.5 = 3.0
    ASSERT_EQ( OK, pSale->applyGetXForYDiscount( "Chips", 3, 1.5 ));

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 24.0, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNForZPerfectRoundMixedItemTypesAllMarkdown){

    ASSERT_EQ( OK, pSale->setMarkdown( "Chips", 0.5 ) );
    ASSERT_EQ( OK, pSale->setMarkdown( "Beef", 0.5 ) );

    pSale->addToCart( "Chips", 6 );
    pSale->addToCart( "Beef", 6.0 ); // 6.0 * 3.0 = 18

    // 6 / 3 = 2 * 1.5 = 3.0
    ASSERT_EQ( OK, pSale->applyGetXForYDiscount( "Chips", 3, 1.5 ));

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 21.0, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNForZMixedItemTypesNoMarkdown){

    pSale->addToCart( "Chips", 8 );
    pSale->addToCart( "Beef", 6.5 ); // 6.5 * 3.5 = 22.75

    // 6 / 3 = 2 * 1.5 = 3.0, 2 * 2.0 = 4.0 (7.0)
    ASSERT_EQ( OK, pSale->applyGetXForYDiscount( "Chips", 3, 1.5 ));

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 29.75, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNForZMixedItemTypesSomeMarkdown){

    // ensures taht markdown has no affect on price given by discount
    ASSERT_EQ( OK, pSale->setMarkdown( "Chips", 0.5 ) );

    pSale->addToCart( "Chips", 8 );
    pSale->addToCart( "Beef", 6.5 ); // 6.5 * 3.5 = 22.75

    // 6 / 3 = 2 * 1.5 = 3.0, 2 * 1.5 = 3 (6.0)
    ASSERT_EQ( OK, pSale->applyGetXForYDiscount( "Chips", 3, 1.5 ));

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 28.75, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNForZMixedItemTypesAllMarkdown){

    ASSERT_EQ( OK, pSale->setMarkdown( "Chips", 0.5 ) );
    ASSERT_EQ( OK, pSale->setMarkdown( "Beef", 0.5 ) );

    pSale->addToCart( "Chips", 8 );
    pSale->addToCart( "Beef", 6.5 ); // 6.5 * 3.0 = 19.50

    // 6 / 3 = 2 * 1.5 = 3.0, 2 * 1.5 = 3 (6.0)
    ASSERT_EQ( OK, pSale->applyGetXForYDiscount( "Chips", 3, 1.5 ));

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 25.50, .01 );
}

TEST_F (PriceCalculationWithDiscountsTest, buyNForZPerfectRoundMixedItemTypesSomeMarkdownWithLimit){

    // ensures taht markdown has no affect on price given by discount
    ASSERT_EQ( OK, pSale->setMarkdown( "Chips", 0.5 ) );

    pSale->addToCart( "Chips", 6 );
    pSale->addToCart( "Beef", 6.0 ); // 6.0 * 3.5 = 21

    // 3 for 1.5
    // 3 * 1.5 = 4.5
    //    (6.0)
    ASSERT_EQ( OK, pSale->applyGetXForYDiscount( "Chips", 3, 1.5, 5 ));

    // verify correct price was generated
    ASSERT_NEAR( pSale->getPreTaxTotal(), 27.0, .01 );
}


///////////////////////////////////////////////////////////////////////////////
//                        Combined Discounts
///////////////////////////////////////////////////////////////////////////////

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffMultipleDiscounts){

    pSale->addToCart( "Chips", 6 );
    pSale->addToCart( "Beef", 6.0 );
    ASSERT_EQ( OK, pSale->applyGetXForYDiscount( "Chips", 3, 3.00 ) ); // 6
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Beef", 4.0, 2.0, 0.5 )); // 14 + 2
    ASSERT_NEAR( pSale->getPreTaxTotal(), 23.5, .01 );

}

TEST_F (PriceCalculationWithDiscountsTest, buyNGetMOffMultipleDiscountsMultipleDiscounts){

    ASSERT_EQ( OK, pSale->setMarkdown ("Beef", 0.5 ) );

    ASSERT_EQ( OK, pSale->addToCart( "Chips", 6 ) );
    ASSERT_EQ( OK, pSale->addToCart( "Beef", 6.0 ) );
    
    ASSERT_EQ( OK, pSale->applyGetXForYDiscount( "Chips", 3, 3.00 ) ); // 6
    ASSERT_EQ( OK, pSale->applyBuyXGetYAtDiscount( "Beef", 4.0, 2.0, 0.5 )); // 12 + 3
    ASSERT_NEAR( pSale->getPreTaxTotal(), 21.0, .01 );

}
