#include "gtest/gtest.h"
#include "WeightBasedItem.h"

///////////////////////////////////////////////////////////////////////////////
//                           setPrice Verification
///////////////////////////////////////////////////////////////////////////////

TEST (WeightBasedItemTest, setNegativePrice){

    WeightBasedItem item;
    ASSERT_EQ( INVALID_PRICE, item.setPrice( -1.0 ) );

}

TEST (WeightBasedItemTest, setZeroPrice){

    WeightBasedItem item;
    ASSERT_EQ( INVALID_PRICE, item.setPrice( 0.0 ) );

}

TEST (WeightBasedItemTest, updatePriceAfterAddingToCart){

    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(2.3) );
    ASSERT_EQ( OK, item.addToCart( 2.0 ) );
    ASSERT_EQ( PRICE_UPDATE_NOT_AVAILABLE, item.setPrice(2.2) );

}

TEST (WeightBasedItemTest, multipleSetPriceUpdates){

    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(2.3) );
    ASSERT_EQ( OK, item.setPrice(2.2) );

}

///////////////////////////////////////////////////////////////////////////////
//                           MARKDOWN Verification
///////////////////////////////////////////////////////////////////////////////

TEST (WeightBasedItemTest, applyMarkdownAfterItemInCart){

    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(2.4));
    ASSERT_EQ( OK, item.addToCart( 4.1 ));
    ASSERT_EQ( PRICE_UPDATE_NOT_AVAILABLE, item.applyMarkdown(0.2) );

}

TEST (WeightBasedItemTest, applyMarkdownBeforePrice){

    WeightBasedItem item;
    ASSERT_EQ( NO_PRICE_DEFINED, item.applyMarkdown(0.2) );

}

TEST (WeightBasedItemTest, applyTooBigOfMarkdown){

    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( PRICING_CONFLICT, item.applyMarkdown(1.2) );

}

TEST (WeightBasedItemTest, negativeMarkdown){

    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( PRICING_CONFLICT, item.applyMarkdown(-1.0) );

}

///////////////////////////////////////////////////////////////////////////////
//                           AddToCart Verification
///////////////////////////////////////////////////////////////////////////////

TEST (WeightBasedItemTest, addToCartBeforePriceConfigured){

    WeightBasedItem item;
    ASSERT_EQ( NO_PRICE_DEFINED, item.addToCart( 1.0 ) );
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( OK, item.addToCart( 1.0 ) );

}

///////////////////////////////////////////////////////////////////////////////
//                           RemoveFromCart Verification
///////////////////////////////////////////////////////////////////////////////

TEST (WeightBasedItemTest, removeFromCartBeforeInCart){

    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( ITEM_NOT_IN_CART, item.removeFromCart( 1.0 ) );

}

TEST (WeightBasedItemTest, removeMoreThanInCart){

    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( OK, item.addToCart( 4.0 ) );
    ASSERT_EQ( ITEM_NOT_IN_CART, item.removeFromCart( 6.0 ) );

}

TEST (WeightBasedItemTest, removeItemFromCart){

    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( OK, item.addToCart( 3.1 ) );
    ASSERT_EQ( OK, item.removeFromCart( 1 ) );

}

///////////////////////////////////////////////////////////////////////////////
//                           Compute Pre-Tax Verification
///////////////////////////////////////////////////////////////////////////////

TEST (WeightBasedItemTest, computeTaxWithoutPrice){

    double tax = 0;
    WeightBasedItem item;
    ASSERT_EQ( NO_PRICE_DEFINED, item.computePreTax( &tax ) );

}

TEST (WeightBasedItemTest, computeTaxNoItemsInCarts){

    double tax = 0;
    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.computePreTax( &tax ) );
    ASSERT_NEAR( tax, 0.0, .01 );

}

TEST (WeightBasedItemTest, computeTaxNoMarkdown){

    double tax = 0;
    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart(3.0));
    ASSERT_EQ( OK, item.computePreTax( &tax ) );
    ASSERT_NEAR( tax, 10.2, .01 );

}

TEST (WeightBasedItemTest, computeTaxWithMarkdown){

    double tax = 0;
    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown(0.4));
    ASSERT_EQ( OK, item.addToCart(3.0));
    ASSERT_EQ( OK, item.computePreTax( &tax ) );
    ASSERT_NEAR( tax, 9.0, .01 );

}