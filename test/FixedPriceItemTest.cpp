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

TEST (FixedPriceItemTest, addToCartNegative){

    FixedPriceItem item;
    
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( ERROR, item.addToCart( -1 ) );

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

TEST (FixedPriceItemTest, removeItemFromCartNegativeAmount){

    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice( 1.0 ) );
    ASSERT_EQ( OK, item.addToCart( 3 ) );
    ASSERT_EQ( ERROR, item.removeFromCart( -1 ) );

}

///////////////////////////////////////////////////////////////////////////////
//                           Compute Pre-Tax Verification
///////////////////////////////////////////////////////////////////////////////

TEST (FixedPriceItemTest, computeTaxWithoutPrice){

    double tax = 0;
    FixedPriceItem item;
    ASSERT_EQ( NO_PRICE_DEFINED, item.computePreTax( &tax ) );

}

TEST (FixedPriceItemTest, computeTaxNoItemsInCarts){

    double tax = 0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.computePreTax( &tax ) );
    ASSERT_NEAR( tax, 0.0, .01 );

}

TEST (FixedPriceItemTest, computeTaxNoMarkdown){

    double tax = 0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart(3));
    ASSERT_EQ( OK, item.computePreTax( &tax ) );
    ASSERT_NEAR( tax, 10.2, .01 );

}

TEST (FixedPriceItemTest, computeTaxWithMarkdown){

    double tax = 0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown(0.4));
    ASSERT_EQ( OK, item.addToCart(3));
    ASSERT_EQ( OK, item.computePreTax( &tax ) );
    ASSERT_NEAR( tax, 9.0, .01 );

}

///////////////////////////////////////////////////////////////////////////////
//                           Discount Verification
///////////////////////////////////////////////////////////////////////////////

TEST (FixedPriceItemTest, buyNGetMOffNegativeValues){

    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( -1, 2, .5 ) );
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( 4, -4, .5 ) );
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( 2, 2, -.5 ) );

}

TEST (FixedPriceItemTest, buyNGetMOffInvalidPercentage){

    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));

    // should not be able to provide a percentage greater than 100 percent
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( 1, 2, 1.1 ) );
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( 4, 2, 1.01 ) );

}

TEST (FixedPriceItemTest, buyNGetMOffPerfectRound){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart( 6 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4, 2, 0.5 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 17, .01 ); // 13.6 + 3.4

}

TEST (FixedPriceItemTest, buyNGetMOffLessThanMLeft){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart( 5 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4, 2, 0.5 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 15.3, .01 ); // 13.6 + 1.7

}

TEST (FixedPriceItemTest, buyNGetMOffNoMLeft){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart( 4 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4, 2, 0.5 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 13.6, .01 ); // 13.6 + 1.7

}

TEST (FixedPriceItemTest, buyNGetMOffWithMarkdown){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown( 0.4 ));
    ASSERT_EQ( OK, item.addToCart( 6 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4, 2, 0.5 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 15, .01 ); // 12 + 3

}