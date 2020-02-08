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
    ASSERT_EQ( INVALID_PRICE, item.applyMarkdown(1.2) );

}

TEST (WeightBasedItemTest, negativeMarkdown){

    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( INVALID_PRICE, item.applyMarkdown(-1.0) );

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

///////////////////////////////////////////////////////////////////////////////
//                           Discount Verification
///////////////////////////////////////////////////////////////////////////////

TEST (WeightBasedItemTest, buyNGetMOffNegativeValues){

    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( -1, 2, .5 ) );
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( 4, -4, .5 ) );
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( 2, 2, -.5 ) );

}

TEST (WeightBasedItemTest, buyNGetMOffInvalidPercentage){

    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));

    // should not be able to provide a percentage greater than 100 percent
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( 1, 2, 1.1 ) );
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( 4, 2, 1.01 ) );

}

TEST (WeightBasedItemTest, buyNGetMOffPerfectRound){

    double tax = 0.0;
    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart( 6.0 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4.0, 2.0, 0.5 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 17, .01 ); // 13.6 + 3.4

}


TEST (WeightBasedItemTest, buyNGetMOffPerfectRoundWithLimit){

    double tax = 0.0;
    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart( 12.0 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4.0, 2.0, 0.5, 6.0 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 37.40, .01 );

}

TEST (WeightBasedItemTest, buyNGetMOffPerfectRoundWithZeroLimit){

    double tax = 0.0;
    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart( 12.0 ) );
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( 4.0, 2.0, 0.5, 0.0 ) );

}

TEST (WeightBasedItemTest, buyNGetMOffWithLessThanMLeft){

    double tax = 0.0;
    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart( 5.0 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4.0, 2.0, 0.5 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 15.3, .01 ); // 13.6 + 1.7

}

TEST (WeightBasedItemTest, buyNGetMOffNoMLeft){

    double tax = 0.0;
    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart( 4.0 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4.0, 2.0, 0.5 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 13.6, .01 ); // 13.6 + 1.7

}

TEST (WeightBasedItemTest, buyNGetMOffWithLimitPreventingMOff){

    double tax = 0.0;
    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart( 6.0 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4.0, 2.0, 0.5, 4.0 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 20.4, .01 );

}

TEST (WeightBasedItemTest, buyNGetMOffWithMarkdown){

    double tax = 0.0;
    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown( 0.4 ));
    ASSERT_EQ( OK, item.addToCart( 6. ) );
    ASSERT_EQ( OK, item.applyDiscount( 4.0, 2.0, 0.5 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 15, .01 ); // 12 + 3

}

TEST (WeightBasedItemTest, buyNGetMOffWithMarkdownWithLimit){

    double tax = 0.0;
    WeightBasedItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown( 0.4 ));
    ASSERT_EQ( OK, item.addToCart( 6.0 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4.0, 2.0, 0.5, 4.0 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 18, .01 ); // 12 + 6

}