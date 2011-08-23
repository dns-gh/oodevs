// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyStockRequestBuilder_h_
#define __SupplyStockRequestBuilder_h_

#include "SupplyRequestBuilder_ABC.h"

class MIL_Automate;
class PHY_DotationStock;

namespace logistic {
    class SupplyRecipient_ABC;

// =============================================================================
/** @class  SupplyStockRequestBuilder
    @brief  SupplyStockRequestBuilder
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyStockRequestBuilder : public SupplyRequestBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyStockRequestBuilder( MIL_Automate& automate, SupplyRecipient_ABC& recipient );
    virtual ~SupplyStockRequestBuilder();
    //@}

    //! @name Operations
    //@{
    virtual void Process( SupplyRequestContainer_ABC& container );
    //@}

private:
    //! @name Operations
    //@{
    void VisitStock( PHY_DotationStock& stock, SupplyRequestContainer_ABC& container ) const;
    //@}

private:
    MIL_Automate& automate_;
    SupplyRecipient_ABC& recipient_;
};

} // end namespace logistic

#endif // __SupplyStockRequestBuilder_h_
