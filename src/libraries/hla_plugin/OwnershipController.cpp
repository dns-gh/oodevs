// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "OwnershipController.h"
#include "RemoteAgentSubject_ABC.h"
#include "Agent_ABC.h"
#include "ObjectListener_ABC.h"
#include "Federate_ABC.h"
#include "HlaObject_ABC.h"
#include "HlaClass_ABC.h"
#include "InteractionSender.h"
#include "InteractionBuilder.h"
#include "Interactions.h"
#include "LocalAgentResolver_ABC.h"
#include "ContextFactory_ABC.h"
#include "TransferSender_ABC.h"
#include "dispatcher/Logger_ABC.h"
#include <hla/Interaction.h>
#include <boost/bind.hpp>

using namespace plugins::hla;


// =============================================================================
/** @class  OwnershipController::OwnershipInfo
    @brief  OwnershipInfo
*/
// Created: AHC 2012-02-21
// =============================================================================
struct OwnershipController::OwnershipInfo : private boost::noncopyable
{
    OwnershipInfo( const std::string& agentID, HlaClass_ABC& hlaClass, HlaObject_ABC& object );

    std::string agentID_;
    HlaClass_ABC& hlaClass_;
    HlaObject_ABC& object_;
};

OwnershipController::OwnershipInfo::OwnershipInfo( const std::string& agentID, HlaClass_ABC& hlaClass, HlaObject_ABC& object )
    : agentID_( agentID )
    , hlaClass_( hlaClass )
    , object_( object )
{

}

// -----------------------------------------------------------------------------
// Name: OwnershipController constructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
OwnershipController::OwnershipController(const rpr::EntityIdentifier& federateID, RemoteAgentSubject_ABC& subject,
        dispatcher::Logger_ABC& logger )
    : federateID_( federateID )
    , subject_ ( subject )
    , logger_( logger )
{
    subject_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: OwnershipController destructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
OwnershipController::~OwnershipController()
{
    subject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: OwnershipController::RemoteCreated
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void OwnershipController::RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object )
{
    OwnershipInfo* ptr = new OwnershipInfo( identifier, hlaClass, object );
    states_[identifier] = OwnershipInfoPtr(ptr);
}

// -----------------------------------------------------------------------------
// Name: OwnershipController::RemoteCreated
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void OwnershipController::RemoteDestroyed( const std::string& identifier )
{
    states_.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: OwnershipController::RemoteCreated
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void OwnershipController::LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object )
{
    OwnershipInfo* ptr = new OwnershipInfo( identifier, hlaClass, object );
    states_[identifier] = OwnershipInfoPtr(ptr);
}

// -----------------------------------------------------------------------------
// Name: OwnershipController::RemoteCreated
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void OwnershipController::LocalDestroyed( const std::string& identifier )
{
    states_.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: OwnershipController::Divested
// Created: AHC 2010-03-02
// -----------------------------------------------------------------------------
void OwnershipController::Divested( const std::string& identifier, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    T_OwnershipInfos::iterator it = states_.find( identifier );
    if( states_.end() == it )
        return;

    logger_.LogInfo( "Divestiture complete for object " + identifier );
}

// -----------------------------------------------------------------------------
// Name: OwnershipController::Acquired
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void OwnershipController::Acquired( const std::string& identifier, const ::hla::T_AttributeIdentifiers& /*attributes*/ )
{
    T_OwnershipInfos::iterator it = states_.find( identifier );
    if( states_.end() == it )
        return;

    logger_.LogInfo( "Acquisition complete for object " + identifier );
}

// -----------------------------------------------------------------------------
// Name: OwnershipController::PerformDivestiture
// Created: AHC 2010-03-12
// -----------------------------------------------------------------------------
void OwnershipController::PerformDivestiture( const std::string& identifier, const std::vector< ::hla::AttributeIdentifier>& attributes, const ::hla::VariableLengthData& tag  )
{
    T_OwnershipInfos::iterator it( states_.find( identifier ) );
    if( states_.end() == it )
        return;

    logger_.LogInfo( "Starting divestiture for object " + identifier );
    it->second->hlaClass_.Divest( identifier, attributes, tag );
}

// -----------------------------------------------------------------------------
// Name: OwnershipController::PerformAcquisition
// Created: AHC 2010-03-12
// -----------------------------------------------------------------------------
void OwnershipController::PerformAcquisition( const std::string& identifier, const std::vector< ::hla::AttributeIdentifier>& attributes, const ::hla::VariableLengthData& tag  )
{
    T_OwnershipInfos::iterator it = states_.find( identifier );
    if( states_.end() == it )
        return;

    logger_.LogInfo( "Starting acquisition for object " + identifier );
    it->second->hlaClass_.Acquire( identifier, attributes, tag );
}

