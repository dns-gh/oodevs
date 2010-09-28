// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ArchitectureAttribute.h"
#include "protocol/Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute constructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
ArchitectureAttribute::ArchitectureAttribute( const MsgsSimToClient::UrbanAttributes& message )
    : height_ ( 0. )
    , floorNumber_     ( 0 )
    , basementLevelNumber_ ( 0 )
    , roofShape_ ( "" )
    , material_ ( "" )
    , occupation_ ( 0. )
    , trafficability_ ( 0. )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute destructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
ArchitectureAttribute::~ArchitectureAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute::Update
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void ArchitectureAttribute::Update( const MsgsSimToClient::UrbanAttributes& message )
{
    if( message.has_architecture() )
    {
        if( message.architecture().has_height() )
            height_ = message.architecture().height();
        if( message.architecture().has_floor_number() )
            floorNumber_ = message.architecture().floor_number();
        if( message.architecture().has_roof_shape() )
            roofShape_ = message.architecture().roof_shape();
        if( message.architecture().has_material() )
            material_ = message.architecture().material();
        if( message.architecture().has_occupation() )
            occupation_ = message.architecture().occupation();
        if( message.architecture().has_trafficability() )
            trafficability_ = message.architecture().trafficability();
    }
}

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute::Send
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void ArchitectureAttribute::Send( MsgsSimToClient::UrbanAttributes& message ) const
{
    message.mutable_architecture()->set_height( height_ );
    message.mutable_architecture()->set_floor_number( floorNumber_ );
    message.mutable_architecture()->set_roof_shape( roofShape_.c_str() );
    message.mutable_architecture()->set_material( material_.c_str() );
    message.mutable_architecture()->set_occupation( occupation_ );
    message.mutable_architecture()->set_trafficability( trafficability_ );
}
