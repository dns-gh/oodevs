// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Netn2TransferSender_h
#define plugins_hla_Netn2TransferSender_h

#include "TransferSender_ABC.h"
#include "rpr/EntityIdentifier.h"
#include "dispatcher/Logger_ABC.h"
#include <hla/InteractionNotification_ABC.h>
#include <hla/FederateIdentifier.h>
#include <memory>
#include <map>

namespace dispatcher
{
    class Logger_ABC;
}

namespace plugins
{
namespace hla
{

class ContextFactory_ABC;
class InteractionBuilder;
template< typename Interaction > class InteractionSender_ABC;
class OwnershipStrategy_ABC;
class OwnershipController_ABC;
class LocalAgentResolver_ABC;
class CallsignResolver_ABC;

namespace interactions
{
    struct TMR_OfferTransferModellingResponsibility;
    struct TMR_RequestTransferModellingResponsibility;
}

// =============================================================================
/** @class  Netn2TransferSender
    @brief  Netn2TransferSender
*/
// Created: AHC 2012-02-23
// =============================================================================
class Netn2TransferSender :  public TransferSender_ABC
                            , private ::hla::InteractionNotification_ABC< interactions::TMR_OfferTransferModellingResponsibility >
                            , private ::hla::InteractionNotification_ABC< interactions::TMR_RequestTransferModellingResponsibility >
{
public:
    //! @name Constructors/Destructor
    //@{
    Netn2TransferSender( const std::string& federateName, const ::hla::FederateIdentifier& federateHandle, const ContextFactory_ABC& ctxtFactory, const InteractionBuilder& builder,
            OwnershipStrategy_ABC& ownershipStrategy, OwnershipController_ABC& controller, dispatcher::Logger_ABC& logger,
            const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver );
    ~Netn2TransferSender();
    //@}

    //! @name Operations
    //@{
    virtual void RequestTransfer( const std::string& agentID, const TransferRequestCallback& callback, TransferType type, const std::vector< ::hla::AttributeIdentifier >& attributes, ::hla::VariableLengthData& tag, uint32_t capability = 0 );
    virtual void RequestTransfer( const std::vector< std::string >& agentIDs, const TransferRequestCallback& callback, TransferType type, const std::vector< ::hla::AttributeIdentifier >& attributes, ::hla::VariableLengthData& tag, uint32_t capability = 0 );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Receive( interactions::TMR_OfferTransferModellingResponsibility& interaction );
    virtual void Receive( interactions::TMR_RequestTransferModellingResponsibility& interaction );
    //@}

private:
    //! @name Types
    //@{
    typedef InteractionSender_ABC< interactions::TMR_OfferTransferModellingResponsibility > T_OfferSender;
    typedef InteractionSender_ABC< interactions::TMR_RequestTransferModellingResponsibility > T_RequestSender;
    typedef std::map< unsigned int, TransferRequestCallback > T_Callbacks;
    //@}

private:
    //! @name Member data
    //@{
    const ContextFactory_ABC& ctxtFactory_;
    const std::string federateName_;
    const ::hla::FederateIdentifier federateHandle_;
    std::auto_ptr< T_OfferSender > pOfferSender_;
    std::auto_ptr< T_RequestSender > pRequestSender_;
    OwnershipStrategy_ABC& ownershipStrategy_;
    OwnershipController_ABC& ownershipController_;
    dispatcher::Logger_ABC& logger_;
    const LocalAgentResolver_ABC& agentResolver_;
    const CallsignResolver_ABC& callsignResolver_;
    T_Callbacks callbacks_;
    //@}
};
}
}
#endif // plugins_hla_Netn2TransferSender_h
