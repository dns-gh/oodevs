// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef EXTERNALOWNERSHIPPOLICY_H_
#define EXTERNALOWNERSHIPPOLICY_H_

#include "OwnershipPolicy_ABC.h"
#include "ClassListener_ABC.h"
#include <hla/InteractionNotification_ABC.h>
#include <hla/FederateIdentifier.h>
#include <boost/shared_ptr.hpp>

#include <memory>
#include <set>

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
class TransferSender_ABC;
class UnicodeString;
class RemoteAgentSubject_ABC;

namespace interactions
{
struct TMR_InitiateTransferModellingResponsibility;
struct TMR_OfferTransferModellingResponsibility;
struct TMR_TransferResult;
struct TransactionId;
}

/// =============================================================================
/// @class plugins::hla::ExternalOwnershipPolicy
/// @thread This type is not thread safe
///
/// Created: AHC 2013-07-03
/// =============================================================================
class ExternalOwnershipPolicy : public plugins::hla::OwnershipPolicy_ABC
                              , private ClassListener_ABC
                              , private ::hla::InteractionNotification_ABC< interactions::TMR_InitiateTransferModellingResponsibility >
                              , private ::hla::InteractionNotification_ABC< interactions::TMR_OfferTransferModellingResponsibility >
                              , private ::hla::InteractionNotification_ABC< interactions::TMR_TransferResult >
{
public:
    //! @name Constructors/Destructor
    //@{
    ExternalOwnershipPolicy(const std::string& federateName, const InteractionBuilder& builder,
            OwnershipController_ABC& controller, dispatcher::Logger_ABC& logger, RemoteAgentSubject_ABC& subject,
            const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver, TransferSender_ABC& transferSender);
    virtual ~ExternalOwnershipPolicy();

private:
    //! @name Operations
    //@{
    virtual void Receive( interactions::TMR_InitiateTransferModellingResponsibility& interaction );
    virtual void Receive( interactions::TMR_OfferTransferModellingResponsibility& interaction );
    virtual void Receive( interactions::TMR_TransferResult& interaction );
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Divested( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes );
    virtual void Acquired( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes );
    void TransferCompleted( bool isOk, const interactions::TransactionId& id, const UnicodeString& requestFederate, const UnicodeString& responseFederate );
    void CleanupRequests();
    //@}

private:
    //! @name Types
    //@{
    typedef InteractionSender_ABC< interactions::TMR_OfferTransferModellingResponsibility > T_OfferSender;
    typedef InteractionSender_ABC< interactions::TMR_InitiateTransferModellingResponsibility > T_InitiateSender;
    typedef InteractionSender_ABC< interactions::TMR_TransferResult > T_ResultSender;
    struct PendingRequest;
    typedef boost::shared_ptr< PendingRequest > T_PendingRequestPtr;
    typedef std::map< uint32_t, T_PendingRequestPtr > T_Requests;
    //@}
    friend struct PendingRequest;

private:
    //! @name Member data
    //@{
    const std::string federateName_;
    std::auto_ptr< T_OfferSender > pOfferSender_;
    std::auto_ptr< T_InitiateSender > pInitiateSender_;
    std::auto_ptr< T_ResultSender > pResultSender_;
    OwnershipController_ABC& ownershipController_;
    dispatcher::Logger_ABC& logger_;
    RemoteAgentSubject_ABC& subject_;
    const LocalAgentResolver_ABC& agentResolver_;
    const CallsignResolver_ABC& callsignResolver_;
    TransferSender_ABC& transferSender_;
    std::set< std::string > localObjects_;
    std::set< std::string > remoteObjects_;
    T_Requests requests_;
    //@}

};

} // namespace hla
} // namespace plugins
#endif // EXTERNALOWNERSHIPPOLICY_H_
