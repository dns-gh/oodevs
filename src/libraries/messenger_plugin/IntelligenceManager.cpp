// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "IntelligenceManager.h"

#include "IdManager.h"
#include "Intelligence.h"
#include "ASN_Messages.h"

using namespace messenger;

// -----------------------------------------------------------------------------
// Name: IntelligenceManager constructor
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
IntelligenceManager::IntelligenceManager( IdManager& idmanager, const kernel::CoordinateConverter_ABC& converter)
    : idManager_( idmanager )
    , converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligenceManager destructor
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
IntelligenceManager::~IntelligenceManager()
{
    for( CIT_IntelligenceMap it = intelligences_.begin(); it != intelligences_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: IntelligenceManager::ReadIntelligence
// Created: RDS 2008-04-08
// -----------------------------------------------------------------------------
void IntelligenceManager::ReadIntelligence(xml::xistream& xis, const ASN1T_Formation& formation)
{
    Intelligence* intelligence = new Intelligence( idManager_.nextId(), xis, formation, converter_ );
    intelligences_[intelligence->GetID()] = intelligence ;
}

// -----------------------------------------------------------------------------
// Name: IntelligenceManager::Write
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void IntelligenceManager::Write(xml::xostream& xos)
{
    for( CIT_IntelligenceMap it = intelligences_.begin(); it != intelligences_.end(); ++it )
        it->second->Write( xos, converter_ );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceManager::OnReceive (creation)
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void IntelligenceManager::OnReceive( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgIntelligenceCreationRequest&     asn)
{
    ASN_MsgIntelligenceCreationRequestAck ack ;
    ack() = EnumIntelligenceErrorCode::no_error;
    try
    {
        Intelligence* intelligence = new Intelligence( idManager_.nextId(), asn );
        intelligences_[ intelligence->GetID() ] = intelligence;
        intelligence->SendCreation( clients );
    }
    catch(...)
    {
        // TODO : ERROR !!
    }
    ack.Send(publisher);

}
// -----------------------------------------------------------------------------
// Name: IntelligenceManager::OnReceive ( update )
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void IntelligenceManager::OnReceive( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgIntelligenceUpdateRequest&       asn)
{
    ASN_MsgIntelligenceUpdateRequestAck ack ;
    ack() = EnumIntelligenceErrorCode::no_error;

    CIT_IntelligenceMap it = intelligences_.find(asn.oid);
    if( it!=intelligences_.end())
    {
        Intelligence* intelligence = it->second;
        intelligence->Update(asn);
        intelligence->SendUpdate(clients);
    }
    ack.Send(publisher);
}

// -----------------------------------------------------------------------------
// Name: IntelligenceManager::OnReceive ( destruction )
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void IntelligenceManager::OnReceive( dispatcher::ClientPublisher_ABC& publisher, dispatcher::ClientPublisher_ABC& clients, const ASN1T_MsgIntelligenceDestructionRequest&  asn)
{
    ASN_MsgIntelligenceDestructionRequestAck ack ;
    ack() = EnumIntelligenceErrorCode::no_error;

    IT_IntelligenceMap it = intelligences_.find(asn.oid);
    if( it!=intelligences_.end())
    {
        Intelligence* intelligence = it->second;
        intelligence->SendDestruction(clients);
        delete intelligence;
        intelligences_.erase( it );
    }
    ack.Send(publisher);
}

// -----------------------------------------------------------------------------
// Name: IntelligenceManager::SendStateToNewClient
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void IntelligenceManager::SendStateToNewClient( dispatcher::ClientPublisher_ABC& client) const
{
    for( CIT_IntelligenceMap it = intelligences_.begin(); it != intelligences_.end(); ++it )
        it->second->SendFullState(client);
}

// -----------------------------------------------------------------------------
// Name: IntelligenceManager::CollectIntelligences
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void IntelligenceManager::CollectFormations(T_FormationMap& collection)
{
    for( CIT_IntelligenceMap it = intelligences_.begin(); it != intelligences_.end(); ++it )
        collection[it->second->GetFormation()].insert(it->second);
}
