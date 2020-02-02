#ifndef PRE_DEFINED_PRICES_FIXTURE_H
#define PRE_DEFINED_PRICES_FIXTURE_H

#include "gtest/gtest.h"
#include "PointOfSale.h"

class PreDefinedPricesFixture : public ::testing::Test { 
    
protected: 

   void SetUp( ) override;

   void TearDown( ) override;

   // put in any custom data members that you need 
   PointOfSale *pSale;
};

#endif