// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __SupplyDotationManualRequestBuilder_h_
#define __SupplyDotationManualRequestBuilder_h_

#include "SupplyRequestBuilder_ABC.h"
#include "protocol/ClientSenders.h"
#include <tools/Resolver.h>
#include <boost/serialization/export.hpp>

class AutomateFactory_ABC;
class MIL_Automate;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

namespace sword
{
    class PushFlowParameters;
}

namespace logistic
{
    class SupplySupplier_ABC;

// =============================================================================
/** @class  SupplyDotationManualRequestBuilder
    @brief  SupplyDotationManualRequestBuilder
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyDotationManualRequestBuilder : public SupplyRequestBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyDotationManualRequestBuilder();
             SupplyDotationManualRequestBuilder( const sword::PushFlowParameters& parameters, SupplySupplier_ABC& supplier, const AutomateFactory_ABC& recipientResolver );
    virtual ~SupplyDotationManualRequestBuilder();
    //@}

    //! @name Operations
    //@{
    virtual void Process( SupplyRequestContainer_ABC& container );
    //@}

    void serialize( MIL_CheckPointInArchive&, const unsigned int );
    void serialize( MIL_CheckPointOutArchive&, const unsigned int );

protected:
    //! @name Operations
    //@{
    void CreateRequest( MIL_Automate& recipient, const sword::SupplyFlowResource& resource, SupplyRequestContainer_ABC& container ) const;
    void SetTransporters( const google::protobuf::RepeatedPtrField< ::sword::SupplyFlowTransporter >& transporters, SupplyRequestContainer_ABC& container ) const;
    //@}

private:
    //! @name Member data
    //@{
    const sword::PushFlowParameters pushFlowParameters_;
    SupplySupplier_ABC* supplier_;
    const AutomateFactory_ABC* recipientResolver_;
    //@}
};

} // end namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::SupplyDotationManualRequestBuilder )

#endif // __SupplyDotationManualRequestBuilder_h_
