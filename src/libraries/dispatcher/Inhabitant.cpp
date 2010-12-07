// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Inhabitant.h"
#include "Model.h"
#include "Side.h"
#include "EntityPublisher.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( Model_ABC& model, const MsgsSimToClient::MsgPopulationCreation& msg )
    : dispatcher::Inhabitant_ABC( msg.id().id(), QString( msg.name().c_str() ) )
    , model_            ( model )
    , nType_            ( msg.type().id() )
    , strName_          ( msg.name() )
    , text_             ( msg.text() )
    , side_             ( model.Sides().Get( msg.party().id() ) )
    , nNbrHealthyHumans_( 0 )
    , nNbrDeadHumans_   ( 0 )
    , nNbrWoundedHumans_( 0 )
{
    if( msg.has_extension() )
        for( int i = 0; i < msg.extension().entries_size(); ++i )
            extensions_[ msg.extension().entries( i ).name() ] = msg.extension().entries( i ).value();

    for( int i = 0; i < msg.blocks_size(); ++i )
        urbanObjectId_.push_back( msg.blocks( i ).id() );

    side_.Register( *this );
    RegisterSelf( *this );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant destructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
Inhabitant::~Inhabitant()
{
    side_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::DoUpdate
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void Inhabitant::DoUpdate( const MsgsSimToClient::MsgPopulationUpdate& msg )
{
    if( msg.has_healthy()  )
        nNbrHealthyHumans_ = msg.healthy();
    if( msg.has_dead()  )
        nNbrDeadHumans_ = msg.dead();
    if( msg.has_wounded()  )
        nNbrWoundedHumans_ = msg.wounded();
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::SendCreation
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void Inhabitant::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::PopulationCreation asn;
    asn().mutable_id()->set_id( GetId() );
    asn().mutable_party()->set_id( side_.GetId() );
    asn().mutable_type()->set_id( nType_ );
    asn().set_text( text_ );
    asn().set_name( strName_ );
    for( std::map< std::string, std::string >::const_iterator it = extensions_.begin(); it !=  extensions_.end(); ++it )
    {
        MsgsSimToClient::Extension_Entry* entry = asn().mutable_extension()->add_entries();
        entry->set_name( it->first );
        entry->set_value( it->second );
    }

    for( std::vector< int >::const_iterator it = urbanObjectId_.begin(); it != urbanObjectId_.end(); ++it )
    {
        Common::UrbanObjectId* blockId = asn().add_blocks();
        blockId->set_id( *it );
    }

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::SendFullUpdate
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void Inhabitant::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::PopulationUpdate asn;
    asn().mutable_id()->set_id( GetId() );
    asn().set_healthy( nNbrHealthyHumans_ );
    asn().set_wounded( nNbrDeadHumans_ );
    asn().set_dead( nNbrWoundedHumans_ );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::SendDestruction
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void Inhabitant::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Accept
// Created: SLG 2010-12-01
// -----------------------------------------------------------------------------
void Inhabitant::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::Draw
// Created: SLG 2010-12-01
// -----------------------------------------------------------------------------
void Inhabitant::Draw( const kernel::GlTools_ABC& tools ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}
