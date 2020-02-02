#ifndef PRE_DEFINED_PRICES_FIXTURE_H
#define PRE_DEFINED_PRICES_FIXTURE_H

#include "gtest/gtest.h"
#include "PointOfSale.h"

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

#endif