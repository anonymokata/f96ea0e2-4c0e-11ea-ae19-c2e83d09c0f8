#ifndef ITEM_H
#define ITEM_H

#include "Types.h"

class Item
{
    public:

        Item();
        ~Item();

        ReturnCode_t setPrice( double price );
        ReturnCode_t applyMarkdown( double amount );
        // ReturnCode_t applyDiscount();
        virtual ReturnCode_t computePreTax( double *pTaxAmount ) = 0;

    protected:
        double price;
        bool is_price_set;

        double markdown;
        bool is_markdown_set;

};

#endif