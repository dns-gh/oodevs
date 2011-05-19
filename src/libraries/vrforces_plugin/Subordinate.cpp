// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "vrforces_plugin_pch.h"
#include "Subordinate.h"
#include "Agent.h"
#include "AggregatedState_ABC.h"
#include "AggregatedPosition_ABC.h"
#include "Facade.h"
#include "dispatcher/Logger_ABC.h"
#pragma warning( push, 0 )
#include <boost/lexical_cast.hpp>
#include <vl/aggPub.h>
#include <vl/aggregateSR.h>
#include <vl/reflEntList.h>
#include <vrforces/vrfcontrol/vrfController.h>
#pragma warning( pop )

using namespace plugins::vrforces;

namespace
{
    void OnCreate( const DtString& name, const DtEntityIdentifier& id, void* usr )
    {
        if( Subordinate* that = static_cast< Subordinate* >( usr ) )
            that->OnCreate( name, id );

    }
    void OnUpdate( DtReflectedEntity* reflected, void* usr )
    {
        if( Subordinate* that = static_cast< Subordinate* >( usr ) )
            that->OnUpdate( reflected );
    }
}

// -----------------------------------------------------------------------------
// Name: Subordinate constructor
// Created: SBO 2011-03-23
// -----------------------------------------------------------------------------
Subordinate::Subordinate( const DtEntityType& type, DtAggregatePublisher& publisher, DtReal heading, const std::string& identifier, DtVrfRemoteController& controller, const DtSimulationAddress& address, Facade& vrForces, Agent& superior, dispatcher::Logger_ABC& logger )
    : identifier_( std::string( publisher.asr()->entityId().string() ) + identifier )
    , entityId_  ( DtEntityIdentifier::nullId() )
    , controller_( controller )
    , address_   ( address )
    , vrForces_  ( vrForces )
    , superior_  ( superior )
    , logger_    ( logger )
    , reflected_ ( 0 )
{
    vrForces_.AddListener( *this );
    controller_.createEntity( &::OnCreate, this
                            , type
                            , publisher.asr()->location()
                            , publisher.asr()->forceId()
                            , heading
                            , DtString::nullString()
                            , publisher.asr()->markingText()
                            , address_ );
}

// -----------------------------------------------------------------------------
// Name: Subordinate destructor
// Created: SBO 2011-03-23
// -----------------------------------------------------------------------------
Subordinate::~Subordinate()
{
    vrForces_.RemoveListener( *this );
    if( entityId_ != DtEntityIdentifier::nullId() )
        controller_.deleteObject( entityId_, address_ );
}

// -----------------------------------------------------------------------------
// Name: Subordinate::OnCreate
// Created: SBO 2011-03-23
// -----------------------------------------------------------------------------
void Subordinate::OnCreate( const DtString& /*name*/, const DtEntityIdentifier& id )
{
    entityId_ = id;
    if( superiorId_ != DtEntityIdentifier::nullId() )
        SetSuperior( superiorId_ );
    if( !reflected_ )
    {
        vrForces_.Lookup( id, *this );
        if( reflected_ )
            vrForces_.RemoveListener( *this );
    }
    logger_.LogInfo( std::string( "Subordinate created with identifier: " ) + id.string() );
}

// -----------------------------------------------------------------------------
// Name: Subordinate::NotifyCreated
// Created: SBO 2011-03-30
// -----------------------------------------------------------------------------
bool Subordinate::NotifyCreated( DtReflectedEntity& entity )
{
    if( reflected_ )
        return true;
    if( entityId_ != DtEntityIdentifier::nullId() )
        if( entity.esr()->entityId() == entityId_ )
        {
            reflected_ = &entity;
            entityId_ = reflected_->esr()->entityId();
            reflected_->addPostUpdateCallback( &::OnUpdate, this );
            SetDestination( destination_ );
            return true;
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Subordinate::OnUpdate
// Created: SBO 2011-03-30
// -----------------------------------------------------------------------------
void Subordinate::OnUpdate( DtReflectedEntity* reflected )
{
    if( reflected && reflected_ == reflected )
    {
        state_ = reflected_->esr()->damageState();
        superior_.NotifyUpdated( *this );
        logger_.LogInfo( std::string( "Subordinate '" ) + entityId_.string() + std::string( "' updated." ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Subordinate::SetSuperior
// Created: SBO 2011-04-01
// -----------------------------------------------------------------------------
void Subordinate::SetSuperior( const DtEntityIdentifier& identifier )
{
    superiorId_ = identifier;
    if( entityId_ != DtEntityIdentifier::nullId() )
        controller_.addToOrganization( entityId_, superiorId_, address_ );
}

// -----------------------------------------------------------------------------
// Name: Subordinate::SetDestination
// Created: SBO 2011-04-07
// -----------------------------------------------------------------------------
void Subordinate::SetDestination( const DtVector& location )
{
    if( location != DtVector::zero() )
        if( reflected_ )
        {
            controller_.moveToLocation( reflected_->esr()->markingText(), location, true, address_ );
            logger_.LogInfo( std::string( "Task 'move to location' assigned to entity '" ) + boost::lexical_cast< std::string >( reflected_->name() ) + "'" );
            destination_ = DtVector::zero();
        }
        else
            destination_ = location;
}

// -----------------------------------------------------------------------------
// Name: Subordinate::Update
// Created: SBO 2011-04-04
// -----------------------------------------------------------------------------
void Subordinate::Update( AggregatedPosition_ABC& position ) const
{
    if( reflected_ )
        position.Update( entityId_, reflected_->esr()->location() );
}

// -----------------------------------------------------------------------------
// Name: Subordinate::Update
// Created: SBO 2011-04-11
// -----------------------------------------------------------------------------
void Subordinate::Update( AggregatedState_ABC& state ) const
{
    if( reflected_ )
        state.Update( entityId_, reflected_->esr()->damageState() );
}

// -----------------------------------------------------------------------------
// Name: Subordinate::IsUndamaged
// Created: SBO 2011-04-11
// -----------------------------------------------------------------------------
bool Subordinate::IsUndamaged() const
{
    return state_ != DtDamageDestroyed;
}
