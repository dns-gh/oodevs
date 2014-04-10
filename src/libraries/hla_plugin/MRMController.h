// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MRMController_h
#define plugins_hla_MRMController_h

#include "ClassListener_ABC.h"
#include "UniqueId.h"
#include "InteractionsNetn.h"
#include <hla/InteractionNotification_ABC.h>
#include <memory>
#include <set>
#include <map>

namespace dispatcher
{
    class Logger_ABC;
}

namespace plugins
{
namespace hla
{

template< typename Interaction > class InteractionSender_ABC;
class InteractionBuilder;
class RemoteAgentSubject_ABC;
class LocalAgentResolver_ABC;
class CallsignResolver_ABC;

class MRMController
        : private ClassListener_ABC
        , private ::hla::InteractionNotification_ABC< interactions::MRM_DisaggregationRequest >
        , private ::hla::InteractionNotification_ABC< interactions::MRM_DisaggregationResponse >
        , private ::hla::InteractionNotification_ABC< interactions::MRM_ActionComplete >
        , private ::hla::InteractionNotification_ABC< interactions::MRM_AggregationRequest >
        , private ::hla::InteractionNotification_ABC< interactions::MRM_AggregationResponse >
{
public:
    //! @name Constructors/Destructor
    //@{
    MRMController( const std::string& federateName, const InteractionBuilder& builder, RemoteAgentSubject_ABC& subject, dispatcher::Logger_ABC& logger,
            const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver );
    virtual ~MRMController();
    //@}

private:
    //! @name Types
    //@{
    typedef InteractionSender_ABC< interactions::MRM_DisaggregationRequest > T_DisaggregationRequestSender;
    typedef InteractionSender_ABC< interactions::MRM_DisaggregationResponse > T_DisaggregationResponseSender;
    typedef InteractionSender_ABC< interactions::MRM_ActionComplete > T_ActionCompleteSender;
    typedef InteractionSender_ABC< interactions::MRM_AggregationRequest > T_AggregationRequestSender;
    typedef InteractionSender_ABC< interactions::MRM_AggregationResponse > T_AggregationResponseSender;
    struct Info
    {
        Info();
        Info( const std::string& entity, bool aggregate );
        std::string entity;
        bool aggregate;
    };
    //@}

private:
    //! @name Operations
    //@{
    virtual void Receive( interactions::MRM_DisaggregationRequest& interaction );
    virtual void Receive( interactions::MRM_DisaggregationResponse& interaction );
    virtual void Receive( interactions::MRM_ActionComplete& interaction );
    virtual void Receive( interactions::MRM_AggregationRequest& interaction );
    virtual void Receive( interactions::MRM_AggregationResponse& interaction );
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Divested( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes );
    virtual void Acquired( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes );
    //@}

private:
    //! @name Attributes
    const std::string federateName_;
    RemoteAgentSubject_ABC& subject_;
    dispatcher::Logger_ABC& logger_;
    const LocalAgentResolver_ABC& agentResolver_;
    const CallsignResolver_ABC& callsignResolver_;
    std::unique_ptr< T_DisaggregationRequestSender > disaggregationRequestSender_;
    std::unique_ptr< T_DisaggregationResponseSender > disaggregationResponseSender_;
    std::unique_ptr< T_ActionCompleteSender > actionCompleteSender_;
    std::unique_ptr< T_AggregationRequestSender > aggregationRequestSender_;
    std::unique_ptr< T_AggregationResponseSender > aggregationResponseSender_;
    std::set< std::string > localObjects_;
    std::set< std::string > remoteObjects_;
    std::map< interactions::TransactionId, Info > pendingRequests_;
    std::set< std::string > disaggregatedObjects_;
    //@}
};

} // namespace hla
} // namespace plugins
#endif // plugins_hla_MRMController_h