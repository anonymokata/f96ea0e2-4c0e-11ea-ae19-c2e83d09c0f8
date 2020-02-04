#include "gtest/gtest.h"
#include "PointOfSale.h"

// Pull in the test fixture that contains already programmed prices for items in cart
#include "PreDefinedPricesFixture.h"

TEST_F (PreDefinedPricesFixture, setMarkdownNegativePrice){

    ASSERT_EQ( PointOfSale::INVALID_PRICE, pSale->setMarkdown( "Bananas", -1.0 ));

}

TEST_F (PreDefinedPricesFixture, setMarkdownTooLargeMarkdown){

    ASSERT_EQ( PointOfSale::INVALID_PRICE, pSale->setMarkdown( "Bananas", 1.0 ));

}

TEST_F (PreDefinedPricesFixture, setMarkdownMultipleTimes){

    ASSERT_EQ( PointOfSale::OK, pSale->setMarkdown( "Bananas", 0.20 ));
    ASSERT_EQ( PointOfSale::OK, pSale->setMarkdown( "Bananas", 0.30 ));

}

TEST_F (PreDefinedPricesFixture, setMarkdownAfterItemToCart){

    ASSERT_EQ( PointOfSale::OK, pSale->setMarkdown( "Bananas", 0.20 ));
    ASSERT_EQ( PointOfSale::OK, pSale->addItemWeight( "Bananas", 2.34));
    ASSERT_EQ( PointOfSale::PRICE_UPDATE_NOT_AVAILABLE, pSale->setMarkdown( "Bananas", 0.30 ));

}

TEST_F (PreDefinedPricesFixture, setMarkdownFixedPrice){

    ASSERT_EQ( PointOfSale::OK, pSale->setMarkdown( "Soup", 0.20 ));

    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" ));
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" ));
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Soup" ));

    ASSERT_NEAR( pSale->getPreTaxTotal(), 5.3, .01);

}

TEST_F (PreDefinedPricesFixture, setMarkdownFixedPriceDualItems){

    ASSERT_EQ( PointOfSale::OK, pSale->setMarkdown( "Cookies", 0.26 ));

    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" ));
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" ));
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Soup" ));

    ASSERT_NEAR( pSale->getPreTaxTotal(), 4.98, .01);

}

TEST_F (PreDefinedPricesFixture, setMarkdownWeightBasedMarkdownSingle){

    ASSERT_EQ( PointOfSale::OK, pSale->setMarkdown( "Apples", 0.41 ));
    ASSERT_EQ( PointOfSale::OK, pSale->addItemWeight( "Apples", 2.0 ));
    ASSERT_NEAR( pSale->getPreTaxTotal(), 2.0, .01);

}

TEST_F (PreDefinedPricesFixture, setMarkdownMixedItemsMarkdown){

    ASSERT_EQ( PointOfSale::OK, pSale->setMarkdown( "Apples",  0.41 ));
    ASSERT_EQ( PointOfSale::OK, pSale->setMarkdown( "Cookies", 0.26 ));

    ASSERT_EQ( PointOfSale::OK, pSale->addItemWeight( "Apples", 2.0 )); // 2.0
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" )); // 2.0
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Cookies" )); // 2.0
    ASSERT_EQ( PointOfSale::OK, pSale->addFixedPriceItem( "Soup" ));

    ASSERT_NEAR( pSale->getPreTaxTotal(), 6.98, .01);

}