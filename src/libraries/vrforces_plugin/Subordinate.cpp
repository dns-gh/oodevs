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
#pragma warning( push, 0 )
#include <vl/aggPub.h>
#include <vl/aggregateSR.h>
#include <vrforces/vrfController.h>
#pragma warning( pop )

using namespace plugins::vrforces;

// -----------------------------------------------------------------------------
// Name: Subordinate constructor
// Created: SBO 2011-03-23
// -----------------------------------------------------------------------------
Subordinate::Subordinate( const DtEntityType& type, DtAggregatePublisher& publisher, DtReal heading, const std::string& identifier, DtVrfRemoteController& controller, const DtSimulationAddress& address )
    : identifier_( std::string( publisher.asr()->entityId().string() ) + identifier )
    , entityId_  ( DtEntityIdentifier::nullId() )
    , controller_( controller )
    , address_   ( address )
{
    controller_.createEntity( &Subordinate::OnCreate, this
                            , type
                            , publisher.asr()->location()
                            , publisher.asr()->forceId()
                            , heading
                            , identifier_.c_str()
                            , publisher.asr()->markingText()
                            , address_ );
}

// -----------------------------------------------------------------------------
// Name: Subordinate destructor
// Created: SBO 2011-03-23
// -----------------------------------------------------------------------------
Subordinate::~Subordinate()
{
    if( entityId_ != DtEntityIdentifier::nullId() )
        controller_.deleteObject( entityId_, address_ );
}

// -----------------------------------------------------------------------------
// Name: Subordinate::OnCreate
// Created: SBO 2011-03-23
// -----------------------------------------------------------------------------
void Subordinate::OnCreate( const DtString& name, const DtEntityIdentifier& id, void* usr )
{
    if( Subordinate* that = static_cast< Subordinate* >( usr ) )
        that->entityId_ = id;
}
