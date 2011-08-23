// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyDotationRequestBuilder_h_
#define __SupplyDotationRequestBuilder_h_

#include "SupplyRequestBuilder_ABC.h"

class PHY_Dotation;
class MIL_Automate;

namespace logistic {
    class SupplyRecipient_ABC;
    class SupplyRequest;

// =============================================================================
/** @class  SupplyDotationRequestBuilder
    @brief  SupplyDotationRequestBuilder
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyDotationRequestBuilder : public SupplyRequestBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyDotationRequestBuilder( MIL_Automate& automate, SupplyRecipient_ABC& recipient );
    virtual ~SupplyDotationRequestBuilder();
    //@}

    //! @name Operations
    //@{
    virtual void Process( SupplyRequestContainer_ABC& container );
    //@}

private:
    //! @name Operations
    //@{
    void VisitDotation( PHY_Dotation& dotation, SupplyRequestContainer_ABC& container ) const;
    //@}

private:
    MIL_Automate&        automate_;
    SupplyRecipient_ABC& recipient_;
};

} // end namespace logistic

#endif // __SupplyDotationRequestBuilder_h_
