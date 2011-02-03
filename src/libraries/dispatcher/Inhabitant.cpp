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
#include "Model_ABC.h"
#include "Side.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include <boost/foreach.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Inhabitant constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
Inhabitant::Inhabitant( Model_ABC& model, const sword::PopulationCreation& msg )
    : Inhabitant_ABC( msg.id().id(), QString( msg.name().c_str() ) )
    , model_              ( model )
    , nType_              ( msg.type().id() )
    , strName_            ( msg.name() )
    , text_               ( msg.text() )
    , side_               ( model.Sides().Get( msg.party().id() ) )
    , nNbrHealthyHumans_  ( 0 )
    , nNbrDeadHumans_     ( 0 )
    , nNbrWoundedHumans_  ( 0 )
    , healthSatisfaction_ ( 0 )
    , safetySatisfaction_ ( 0 )
    , lodgingSatisfaction_( 0 )
{
    if( msg.has_extension() )
        for( int i = 0; i < msg.extension().entries_size(); ++i )
            extensions_[ msg.extension().entries( i ).name() ] = msg.extension().entries( i ).value();
    for( int i = 0; i < msg.objects_size(); ++i )
        urbanObjectId_.push_back( msg.objects( i ).id() );
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
    if( msg.has_satisfaction() )
    {
        if( msg.satisfaction().has_health() )
            healthSatisfaction_ = msg.satisfaction().health();
        if( msg.satisfaction().has_safety() )
            safetySatisfaction_ = msg.satisfaction().safety();
        if( msg.satisfaction().has_lodging() )
            lodgingSatisfaction_ = msg.satisfaction().lodging();
        for( int i = 0; i < msg.satisfaction().motivations_size(); ++i )
            motivationSatisfactions_[ msg.satisfaction().motivations( i ).motivation() ] = msg.satisfaction().motivations( i ).percentage();
        for( int i = 0; i < msg.satisfaction().resources_size(); ++i )
            resourcesSatisfactions_[ msg.satisfaction().resources( i ).resource().id() ] = msg.satisfaction().resources( i ).value();
    }
    for( int i = 0; i < msg.occupations_size(); ++i )
    {
        const sword::PopulationUpdate::BlockOccupation& occupation = msg.occupations( i );
        T_Block& block = urbanBlocks_[ occupation.object().id() ];
        block.number_ = occupation.number();
        block.alerted_ = occupation.alerted();
    }
    for( int i = 0; i < msg.adhesions_size(); ++i )
    {
        const sword::PartyAdhesion& adhesion = msg.adhesions( i );
        affinities_[ adhesion.party().id() ] = adhesion.value();
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
        msg().add_objects()->set_id( id );
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
    msg().mutable_satisfaction()->set_health( healthSatisfaction_ );
    msg().mutable_satisfaction()->set_safety( safetySatisfaction_ );
    msg().mutable_satisfaction()->set_lodging( lodgingSatisfaction_ );
    for( CIT_MotivationSatisfactions it = motivationSatisfactions_.begin(); it != motivationSatisfactions_.end(); ++it )
    {
        sword::PopulationUpdate_MotivationSatisfaction* motivation = msg().mutable_satisfaction()->add_motivations();
        motivation->set_motivation( it->first );
        motivation->set_percentage( it->second );
    }
    for( CIT_ResourcesSatisfactions it = resourcesSatisfactions_.begin(); it != resourcesSatisfactions_.end(); ++it )
    {
        sword::PopulationUpdate_ResourceSatisfaction* resource = msg().mutable_satisfaction()->add_resources();
        resource->mutable_resource()->set_id( it->first );
        resource->set_value( it->second );
    }
    BOOST_FOREACH( const T_UrbanBlocks::value_type& urbanBlock, urbanBlocks_ )
    {
        sword::PopulationUpdate_BlockOccupation& block = *msg().mutable_occupations()->Add();
        block.mutable_object()->set_id( urbanBlock.first );
        block.set_number( urbanBlock.second.number_ );
        block.set_alerted( urbanBlock.second.alerted_ );
    }
    BOOST_FOREACH( const T_Affinities::value_type& affinity, affinities_ )
    {
        sword::PartyAdhesion& adhesion = *msg().add_adhesions();
        adhesion.mutable_party()->set_id( affinity.first );
        adhesion.set_value( affinity.second );
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
