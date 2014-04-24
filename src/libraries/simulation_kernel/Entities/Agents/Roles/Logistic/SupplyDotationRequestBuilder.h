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
#include <boost/serialization/export.hpp>

class MIL_Automate;

namespace logistic
{
    class SupplyRecipient_ABC;

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
             SupplyDotationRequestBuilder();
             SupplyDotationRequestBuilder( MIL_Automate& automate, SupplyRecipient_ABC& recipient );
    virtual ~SupplyDotationRequestBuilder();
    //@}

    //! @name Operations
    //@{
    virtual void Process( SupplyRequestContainer_ABC& container );
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    MIL_Automate* automate_;
    SupplyRecipient_ABC* recipient_;
};

} // end namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::SupplyDotationRequestBuilder )

#endif // __SupplyDotationRequestBuilder_h_
