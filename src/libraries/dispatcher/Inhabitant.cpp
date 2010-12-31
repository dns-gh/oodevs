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
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( Model_ABC& model, const sword::PopulationCreation& msg )
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
void Inhabitant::DoUpdate( const sword::PopulationUpdate& msg )
{
    if( msg.has_healthy() )
        nNbrHealthyHumans_ = msg.healthy();
    if( msg.has_dead() )
        nNbrDeadHumans_ = msg.dead();
    if( msg.has_wounded() )
        nNbrWoundedHumans_ = msg.wounded();
    for( int i = 0; i < msg.occupations_size(); ++i )
    {
        const sword::PopulationUpdate_BlockOccupation& occupation = msg.occupations( i );
        urbanBlocks_[ occupation.block().id() ] = occupation.number();
    }
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::SendCreation
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void Inhabitant::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::PopulationCreation msg;
    msg().mutable_id()->set_id( GetId() );
    msg().mutable_party()->set_id( side_.GetId() );
    msg().mutable_type()->set_id( nType_ );
    msg().set_text( text_ );
    msg().set_name( strName_ );
    BOOST_FOREACH( const T_Extensions::value_type& extension, extensions_ )
    {
        sword::Extension_Entry* entry = msg().mutable_extension()->add_entries();
        entry->set_name( extension.first );
        entry->set_value( extension.second );
    }
    BOOST_FOREACH( int id, urbanObjectId_ )
    {
        sword::UrbanObjectId* blockId = msg().add_blocks();
        blockId->set_id( id );
    }
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::SendFullUpdate
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void Inhabitant::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::PopulationUpdate msg;
    msg().mutable_id()->set_id( GetId() );
    msg().set_healthy( nNbrHealthyHumans_ );
    msg().set_wounded( nNbrWoundedHumans_ );
    msg().set_dead( nNbrDeadHumans_ );
    BOOST_FOREACH( const T_UrbanBlocks::value_type& urbanBlock, urbanBlocks_ )
    {
        sword::PopulationUpdate_BlockOccupation& block = *msg().mutable_occupations()->Add();
        block.mutable_block()->set_id( urbanBlock.first );
        block.set_number( urbanBlock.second );
    }
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Inhabitant::SendDestruction
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void Inhabitant::SendDestruction( ClientPublisher_ABC& /*publisher*/ ) const
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
void Inhabitant::Draw( const kernel::GlTools_ABC& /*tools*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}
