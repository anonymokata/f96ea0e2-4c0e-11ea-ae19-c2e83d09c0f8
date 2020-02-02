#ifndef PRE_DEFINED_PRICES_FIXTURE_H
#define PRE_DEFINED_PRICES_FIXTURE_H

#include "gtest/gtest.h"
#include "PointOfSale.h"

/// \fn
/// \brief Defines a Fixture that handles configuration of prices for use with tests
///
/// This fixture contains a SetUp function that will create the PointOfSale instance and
/// program a series of prices within the class. This allows re-use of this code and thus
/// test development can focus on the important stuff rather than filling in prices for items.
class PreDefinedPricesFixture : public ::testing::Test { 
    
protected: 

   void SetUp( ) override;

   void TearDown( ) override;

   // This pointer will be allocated as part of the SetUp function and released as part of the TearDown function
   PointOfSale *pSale;
};

#endif