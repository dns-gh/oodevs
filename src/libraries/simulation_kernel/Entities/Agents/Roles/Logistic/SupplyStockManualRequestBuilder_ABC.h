// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyStockManualRequestBuilder_h_
#define __SupplyStockManualRequestBuilder_h_

#include "SupplyRequestBuilder_ABC.h"
#include "protocol/ClientSenders.h"
#include <tools/Resolver.h>
#include <boost/serialization/export.hpp>

class MIL_Automate;
class PHY_DotationStock;

namespace logistic {
    class SupplySupplier_ABC;

// =============================================================================
/** @class  SupplyStockManualRequestBuilder_ABC
    @brief  SupplyStockManualRequestBuilder_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyStockManualRequestBuilder_ABC : public SupplyRequestBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyStockManualRequestBuilder_ABC();
    virtual ~SupplyStockManualRequestBuilder_ABC();
    //@}

    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< SupplyRequestBuilder_ABC >( *this );
    }

protected:
    //! @name Operations
    //@{
    void CreateRequest  ( MIL_Automate& recipient, const sword::SupplyFlowResource& resource, SupplyRequestContainer_ABC& container ) const;
    void SetTransporters( const google::protobuf::RepeatedPtrField< ::sword::SupplyFlowTransporter >& transporters, SupplyRequestContainer_ABC& container ) const;
    //@}
};

} // end namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::SupplyStockManualRequestBuilder_ABC )

#endif // __SupplyStockManualRequestBuilder_h_
