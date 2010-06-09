// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "tic_plugin_pch.h"
#include "PlatformAdapter.h"
#include "dispatcher/Agent.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

using namespace plugins::tic;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PlatformAdapter constructor
// Created: AGE 2008-04-24
// -----------------------------------------------------------------------------
PlatformAdapter::PlatformAdapter( const dispatcher::Agent& agent, const kernel::CoordinateConverter_ABC& converter )
    : agent_    ( agent )
    , converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PlatformAdapter destructor
// Created: AGE 2008-04-24
// -----------------------------------------------------------------------------
PlatformAdapter::~PlatformAdapter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PlatformAdapter::GetType
// Created: AGE 2008-04-24
// -----------------------------------------------------------------------------
const ComponentType& PlatformAdapter::GetType() const
{
    tools::Iterator< const AgentComposition& > it = agent_.GetType().CreateIterator();
    if( it.HasMoreElements() )
        return it.NextElement().GetType();
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: PlatformAdapter::GetPosition
// Created: AGE 2008-04-24
// -----------------------------------------------------------------------------
geometry::Point2f PlatformAdapter::GetPosition() const
{
    // $$$$ _RC_ SBO 2010-05-27: reverse position_ lat/long
    const geometry::Point2d pos( agent_.GetPosition().Y(), agent_.GetPosition().X() );
    return converter_.ConvertFromGeo( pos );
}

// -----------------------------------------------------------------------------
// Name: PlatformAdapter::GetLastPosition
// Created: AGE 2008-05-05
// -----------------------------------------------------------------------------
geometry::Point2f PlatformAdapter::GetLastPosition() const
{
    return GetPosition();
}

// -----------------------------------------------------------------------------
// Name: PlatformAdapter::GetAltitude
// Created: AGE 2008-04-24
// -----------------------------------------------------------------------------
float PlatformAdapter::GetAltitude() const
{
    return float( agent_.GetAltitude() );
}

// -----------------------------------------------------------------------------
// Name: PlatformAdapter::GetSpeed
// Created: AGE 2008-04-24
// -----------------------------------------------------------------------------
float PlatformAdapter::GetSpeed() const
{
    return float( agent_.GetSpeed() );
}

// -----------------------------------------------------------------------------
// Name: PlatformAdapter::GetHeading
// Created: AGE 2008-04-24
// -----------------------------------------------------------------------------
float PlatformAdapter::GetHeading() const
{
    return float( agent_.GetDirection() );
}

// -----------------------------------------------------------------------------
// Name: PlatformAdapter::GetState
// Created: AGE 2008-04-24
// -----------------------------------------------------------------------------
Platform_ABC::E_State PlatformAdapter::GetState() const
{
    if( agent_.GetOperationalState() == Common::detruit_totalement )
        return destroyed;
    if( agent_.GetOperationalStateValue() < 100 )
        return broken;
    return okay;
}
