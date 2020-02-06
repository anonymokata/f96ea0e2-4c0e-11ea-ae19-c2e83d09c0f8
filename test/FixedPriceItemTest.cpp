#include "gtest/gtest.h"
#include "FixedPriceItem.h"

///////////////////////////////////////////////////////////////////////////////
//                           setPrice Verification
///////////////////////////////////////////////////////////////////////////////

TEST (FixedPriceItemTest, setNegativePrice){

    FixedPriceItem item;
    ASSERT_EQ( INVALID_PRICE, item.setPrice( -1.0 ) );

}

TEST (FixedPriceItemTest, setZeroPrice){

    FixedPriceItem item;
    ASSERT_EQ( INVALID_PRICE, item.setPrice( 0.0 ) );

}

TEST (FixedPriceItemTest, updatePriceAfterAddingToCart){

    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(2.3) );
    ASSERT_EQ( OK, item.addToCart( 2 ) );
    ASSERT_EQ( PRICE_UPDATE_NOT_AVAILABLE, item.setPrice(2.2) );

}

TEST (FixedPriceItemTest, multipleSetPriceUpdates){

    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(2.3) );
    ASSERT_EQ( OK, item.setPrice(2.2) );

}

///////////////////////////////////////////////////////////////////////////////
//                           MARKDOWN Verification
///////////////////////////////////////////////////////////////////////////////

TEST (FixedPriceItemTest, applyMarkdownAfterItemInCart){

    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(2.4));
    ASSERT_EQ( OK, item.addToCart( 4 ));
    ASSERT_EQ( PRICE_UPDATE_NOT_AVAILABLE, item.applyMarkdown(0.2) );

}

TEST (FixedPriceItemTest, applyMarkdownBeforePrice){

    FixedPriceItem item;
    ASSERT_EQ( NO_PRICE_DEFINED, item.applyMarkdown(0.2) );

}

TEST (FixedPriceItemTest, applyTooBigOfMarkdown){

    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( PRICING_CONFLICT, item.applyMarkdown(1.2) );

}

TEST (FixedPriceItemTest, negativeMarkdown){

    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( PRICING_CONFLICT, item.applyMarkdown(-1.0) );

}

///////////////////////////////////////////////////////////////////////////////
//                           AddToCart Verification
///////////////////////////////////////////////////////////////////////////////

TEST (FixedPriceItemTest, addToCartBeforePriceConfigured){

    FixedPriceItem item;
    ASSERT_EQ( NO_PRICE_DEFINED, item.addToCart( 1 ) );
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( OK, item.addToCart( 1 ) );

}

///////////////////////////////////////////////////////////////////////////////
//                           RemoveFromCart Verification
///////////////////////////////////////////////////////////////////////////////

TEST (FixedPriceItemTest, removeFromCartBeforeInCart){

    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( ITEM_NOT_IN_CART, item.removeFromCart( 1 ) );

}

TEST (FixedPriceItemTest, removeMoreThanInCart){

    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( OK, item.addToCart( 4 ) );
    ASSERT_EQ( ITEM_NOT_IN_CART, item.removeFromCart( 6 ) );

}

TEST (FixedPriceItemTest, removeItemFromCart){

    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( OK, item.addToCart( 3 ) );
    ASSERT_EQ( OK, item.removeFromCart( 1 ) );

}
