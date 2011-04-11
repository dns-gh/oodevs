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
#pragma warning( push, 0 )
#include <vl/aggPub.h>
#include <vl/aggregateSR.h>
#include <vl/reflEntList.h>
#include <vrforces/vrfController.h>
#pragma warning( pop )

using namespace plugins::vrforces;

// -----------------------------------------------------------------------------
// Name: Subordinate constructor
// Created: SBO 2011-03-23
// -----------------------------------------------------------------------------
Subordinate::Subordinate( const DtEntityType& type, DtAggregatePublisher& publisher, DtReal heading, const std::string& identifier, DtVrfRemoteController& controller, const DtSimulationAddress& address, Facade& vrForces, Agent& superior )
    : identifier_( std::string( publisher.asr()->entityId().string() ) + identifier )
    , entityId_  ( DtEntityIdentifier::nullId() )
    , controller_( controller )
    , address_   ( address )
    , vrForces_  ( vrForces )
    , superior_  ( superior )
    , reflected_ ( 0 )
{
    vrForces_.AddListener( *this );
    controller_.createEntity( &Subordinate::OnCreate, this
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
void Subordinate::OnCreate( const DtString& /*name*/, const DtEntityIdentifier& id, void* usr )
{
    if( Subordinate* that = static_cast< Subordinate* >( usr ) )
    {
        that->entityId_ = id;
        if( that->superiorId_ != DtEntityIdentifier::nullId() )
            that->SetSuperior( that->superiorId_ );
        if( !that->reflected_ )
        {
            that->vrForces_.Lookup( id, *that );
            if( that->reflected_ )
                that->vrForces_.RemoveListener( *that );
        }
        DtInfo << "Subordinate created with identifier: " << id.string() << std::endl;
    }
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
            reflected_->addPostUpdateCallback( &Subordinate::OnUpdate, this );
            SetDestination( destination_ );
            return true;
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Subordinate::OnUpdate
// Created: SBO 2011-03-30
// -----------------------------------------------------------------------------
void Subordinate::OnUpdate( DtReflectedEntity* obj, void* userData )
{
    if( Subordinate* that = static_cast< Subordinate* >( userData ) )
        if( obj && that->reflected_ == obj )
        {
            that->state_ = that->reflected_->esr()->damageState();
            that->superior_.NotifyUpdated( *that );
            DtInfo << "Subordinate '" << that->entityId_.string() << "' updated." << std::endl;
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
            DtInfo << "Task 'move to location' assigned to entity '" << reflected_->name() << "'" << std::endl;
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
