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
#include <boost/serialization/export.hpp>

class MIL_Automate;
class MIL_AgentPion;
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
             SupplyStockRequestBuilder();
             SupplyStockRequestBuilder( MIL_Automate& automate, SupplyRecipient_ABC& recipient );
    virtual ~SupplyStockRequestBuilder();
    //@}

    //! @name Operations
    //@{
    virtual void Process( SupplyRequestContainer_ABC& container );
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Operations
    //@{
    void VisitStock( const MIL_AgentPion& pion, PHY_DotationStock& stock, SupplyRequestContainer_ABC& container ) const;
    //@}

private:
    MIL_Automate* automate_;
    SupplyRecipient_ABC* recipient_;
};

} // end namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::SupplyStockRequestBuilder )

#endif // __SupplyStockRequestBuilder_h_
