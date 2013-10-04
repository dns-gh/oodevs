// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "VisionConesToggler.h"
#include "Profile.h"
#include "Simulation.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/FourStateOption.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: VisionConesToggler constructor
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
VisionConesToggler::VisionConesToggler( Controllers& controllers, Publisher_ABC& publisher, QObject* parent )
    : QObject( parent )
    , controllers_    ( controllers )
    , publisher_      ( publisher )
    , displayCones_   ( false )
    , displaySurfaces_( false )
    , displayFog_     ( false )
    , connected_      ( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: VisionConesToggler destructor
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
VisionConesToggler::~VisionConesToggler()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: VisionConesToggler::OptionChanged
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void VisionConesToggler::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "VisionCones" )
    {
        displayCones_ = value.To< FourStateOption >().IsSet( true, true, true );
        SendControlEnableVisionCones();
    }
    else if( name == "VisionSurfaces" )
    {
        displaySurfaces_ = value.To< FourStateOption >().IsSet( true, true, true );
        SendControlEnableVisionCones();
    }
    else if( name == "FogOfWar" )
    {
        displayFog_ = value.To< bool >();
        SendControlEnableVisionCones();
    }
}

// -----------------------------------------------------------------------------
// Name: VisionConesToggler::NotifyUpdated
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void VisionConesToggler::NotifyUpdated( const Profile& profile )
{
    connected_ = profile.IsLoggedIn();
    SendControlEnableVisionCones();
}

// -----------------------------------------------------------------------------
// Name: VisionConesToggler::NotifyUpdated
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void VisionConesToggler::NotifyUpdated( const Simulation& simulation )
{
    if( !simulation.IsConnected() )
        connected_ = false;
}

// -----------------------------------------------------------------------------
// Name: VisionConesToggler::SendControlEnableVisionCones
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void VisionConesToggler::SendControlEnableVisionCones()
{
    if( connected_ )
    {
        simulation::ControlEnableVisionCones msg;
        msg().set_vision_cones( displayCones_ || displaySurfaces_ || displayFog_ );
        msg.Send( publisher_ );
    }
}
