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
#include <algorithm>

using namespace plugins::hla;

namespace
{
    std::set< ::hla::AttributeIdentifier > attributesDifference( const std::set< ::hla::AttributeIdentifier >& lhs, const std::set< ::hla::AttributeIdentifier >& rhs )
    {
        std::vector< ::hla::AttributeIdentifier > result = std::vector< ::hla::AttributeIdentifier >( lhs.size(), ::hla::AttributeIdentifier("") );
        std::vector< ::hla::AttributeIdentifier >::iterator it = std::set_difference(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), result.begin() );
        return std::set< ::hla::AttributeIdentifier >( result.begin(), it );
    }
}
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
    std::set< ::hla::AttributeIdentifier > localAttributes_;
    std::set< ::hla::AttributeIdentifier > divestPending_;
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
    const T_AttributeIdentifiers& allAttributes = hlaClass.GetAttributes();
    ptr->localAttributes_.insert(allAttributes.begin(), allAttributes.end());
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
void OwnershipController::Divested( const std::string& identifier, const T_AttributeIdentifiers& attributes )
{
    T_OwnershipInfos::iterator it = states_.find( identifier );
    if( states_.end() == it )
        return;
    it->second->localAttributes_ = attributesDifference( it->second->localAttributes_, std::set< ::hla::AttributeIdentifier >( attributes.begin(), attributes.end() ) );
    it->second->divestPending_ = attributesDifference( it->second->divestPending_, std::set< ::hla::AttributeIdentifier >( attributes.begin(), attributes.end() ) );
    logger_.LogInfo( "Divestiture complete for object " + identifier );
}

// -----------------------------------------------------------------------------
// Name: OwnershipController::Acquired
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void OwnershipController::Acquired( const std::string& identifier, const T_AttributeIdentifiers& attributes )
{
    T_OwnershipInfos::iterator it = states_.find( identifier );
    if( states_.end() == it )
        return;

    it->second->localAttributes_.insert( attributes.begin(), attributes.end() );
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

    const std::vector< ::hla::AttributeIdentifier>& actualAttributes = attributes.empty() ? it->second->hlaClass_.GetAttributes() : attributes;
    std::vector< ::hla::AttributeIdentifier > attrsToDivest;
    for(auto itAttr = actualAttributes.begin(); actualAttributes.end() != itAttr; ++itAttr)
    {
        if( it->second->localAttributes_.count( *itAttr ) && !it->second->divestPending_.count( *itAttr ) )
            attrsToDivest.push_back(*itAttr);
    }
    logger_.LogInfo( "Starting divestiture for object " + identifier );
    it->second->hlaClass_.Divest( identifier, attrsToDivest, tag );
    it->second->divestPending_.insert(attrsToDivest.begin(), attrsToDivest.end());
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

    const std::vector< ::hla::AttributeIdentifier>& actualAttributes = attributes.empty() ? it->second->hlaClass_.GetAttributes() : attributes;
    const std::set< ::hla::AttributeIdentifier > attrsToAcquire = attributesDifference( std::set< ::hla::AttributeIdentifier >( actualAttributes.begin(), actualAttributes.end() ), it->second->localAttributes_ );
    logger_.LogInfo( "Starting acquisition for object " + identifier );
    it->second->hlaClass_.Acquire( identifier, std::vector< ::hla::AttributeIdentifier >( attrsToAcquire.begin(), attrsToAcquire.end() ), tag );
}

