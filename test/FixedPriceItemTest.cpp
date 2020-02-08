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


TEST (FixedPriceItemTest, buyNGetMOffPerfectRoundWithLimit){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart( 12 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4, 2, 0.5, 6 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 37.40, .01 );

}

TEST (FixedPriceItemTest, buyNGetMOffWithLessThanMLeft){

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

TEST (FixedPriceItemTest, buyNGetMOffWithLimitPreventingMOff){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.addToCart( 6 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4, 2, 0.5, 4 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 20.4, .01 );

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

TEST (FixedPriceItemTest, buyNGetMOffWithMarkdownWithLimit){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown( 0.4 ));
    ASSERT_EQ( OK, item.addToCart( 6 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4, 2, 0.5, 4 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 18, .01 ); // 12 + 6

}

TEST (FixedPriceItemTest, buyNForXPerfectRound){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown( 0.4 ));
    ASSERT_EQ( OK, item.addToCart( 6 ) );
    ASSERT_EQ( OK, item.applyDiscount( 3, 5.00 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax )); 
    ASSERT_NEAR( tax, 10, .01 );

}

TEST (FixedPriceItemTest, buyNForXPerfectRoundWithLimit){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown( 0.4 ));
    ASSERT_EQ( OK, item.addToCart( 6 ) );
    ASSERT_EQ( OK, item.applyDiscount( 3, 5.00, 3 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 14.0, .01 );

}

TEST (FixedPriceItemTest, buyNForXNotPerfectRound){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown( 0.4 ));
    ASSERT_EQ( OK, item.addToCart( 7 ) );
    ASSERT_EQ( OK, item.applyDiscount( 3, 5.00 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 13, .01 );

}

TEST (FixedPriceItemTest, buyNForXNotPerfectRoundWithLimit){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown( 0.4 ));
    ASSERT_EQ( OK, item.addToCart( 7 ) );
    ASSERT_EQ( OK, item.applyDiscount( 3, 5.00, 3 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 17.0, .01 );

}

TEST (FixedPriceItemTest, buyNForXNegativeArguments){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown( 0.4 ));
    ASSERT_EQ( OK, item.addToCart( 7 ) );
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( -3, 5.00, 3 ) );
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( 3, -5.00, 3 ) );
    ASSERT_EQ( INVALID_DISCOUNT, item.applyDiscount( 3, 5.00, -3 ) );

}

TEST (FixedPriceItemTest, buyNGetYForXInvalidatedByItemRemoval){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown( 0.4 ));
    ASSERT_EQ( OK, item.addToCart( 6 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4, 2, 0.5 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 15.0, .01 );

    ASSERT_EQ( OK, item.removeFromCart( 3 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 9.0, .01 );

}

TEST (FixedPriceItemTest, buyNForXInvalidatedByItemRemoval){

    double tax = 0.0;
    FixedPriceItem item;
    ASSERT_EQ( OK, item.setPrice(3.4));
    ASSERT_EQ( OK, item.applyMarkdown( 0.4 ));
    ASSERT_EQ( OK, item.addToCart( 4 ) );
    ASSERT_EQ( OK, item.applyDiscount( 4, 5.0 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 5.0, .01 );

    ASSERT_EQ( OK, item.removeFromCart( 1 ) );
    ASSERT_EQ( OK, item.computePreTax( &tax ));
    ASSERT_NEAR( tax, 9.0, .01 );

}