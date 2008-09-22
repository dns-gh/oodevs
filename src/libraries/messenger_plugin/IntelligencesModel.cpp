// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "IntelligencesModel.h"
#include "IdManager.h"
#include "Intelligence.h"
#include "dispatcher/ClientPublisher_ABC.h"
#include <boost/bind.hpp>

using namespace messenger;

// -----------------------------------------------------------------------------
// Name: IntelligencesModel constructor
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
IntelligencesModel::IntelligencesModel( dispatcher::ClientPublisher_ABC& clients, IdManager& idmanager, const kernel::CoordinateConverter_ABC& converter)
    : clients_  ( clients )
    , idManager_( idmanager )
    , converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel destructor
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
IntelligencesModel::~IntelligencesModel()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::ReadIntelligence
// Created: RDS 2008-04-08
// -----------------------------------------------------------------------------
void IntelligencesModel::ReadIntelligence( xml::xistream& xis, const ASN1T_Formation& formation )
{
    std::auto_ptr< Intelligence > intelligence( new Intelligence( idManager_.nextId(), xis, formation, converter_ ) );
    Register( intelligence->GetID(), *intelligence );
    intelligence.release();
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::Write
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void IntelligencesModel::Write( xml::xostream& xos )
{
    std::for_each( elements_.begin(), elements_.end(), boost::bind( &Intelligence::Write
                 , boost::bind( &T_Elements::value_type::second, _1 ), boost::ref( xos ), boost::ref( converter_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::HandleRequest
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void IntelligencesModel::HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgIntelligenceCreationRequest& asn )
{
    messenger::IntelligenceCreationRequestAck ack ;
    ack() = EnumIntelligenceErrorCode::no_error;
    try
    {
        std::auto_ptr< Intelligence > intelligence( new Intelligence( idManager_.nextId(), asn ) );
        Register( intelligence->GetID(), *intelligence );
        intelligence->SendCreation( clients_ );
        intelligence.release();
    }
    catch( ... )
    {
        // $$$$ SBO 2008-06-09: error
    }
    ack.Send( publisher );

}
// -----------------------------------------------------------------------------
// Name: IntelligencesModel::HandleRequest
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void IntelligencesModel::HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgIntelligenceUpdateRequest& asn )
{
    messenger::IntelligenceUpdateRequestAck ack ;
    ack() = EnumIntelligenceErrorCode::no_error;

    Intelligence* intelligence = Find( asn.oid );
    if( intelligence )
    {
        intelligence->Update( asn );
        intelligence->SendUpdate( clients_ );
    }
    else
        ack() = EnumIntelligenceErrorCode::error_invalid_oid;
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::HandleRequest
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void IntelligencesModel::HandleRequest( dispatcher::ClientPublisher_ABC& publisher, const ASN1T_MsgIntelligenceDestructionRequest& asn )
{
    messenger::IntelligenceDestructionRequestAck ack ;
    ack() = EnumIntelligenceErrorCode::no_error;

    Intelligence* intelligence = Find( asn.oid );
    if( intelligence )
    {
        intelligence->SendDestruction( clients_ );
        delete intelligence;
        Remove( asn.oid );
    }
    else
        ack() = EnumIntelligenceErrorCode::error_invalid_oid;
    ack.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::SendStateToNewClient
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void IntelligencesModel::SendStateToNewClient( dispatcher::ClientPublisher_ABC& client ) const
{
    std::for_each( elements_.begin(), elements_.end(), boost::bind( &Intelligence::SendFullState
                 , boost::bind( &T_Elements::value_type::second, _1 ), boost::ref( client ) ) );
}

// -----------------------------------------------------------------------------
// Name: IntelligencesModel::CollectIntelligences
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void IntelligencesModel::CollectFormations( T_FormationMap& collection )
{
    for( T_Elements::const_iterator it = elements_.begin(); it != elements_.end(); ++it )
        collection[ it->second->GetFormation() ].insert( it->second );
}
