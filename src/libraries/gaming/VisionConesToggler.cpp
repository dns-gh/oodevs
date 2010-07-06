// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "VisionConesToggler.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/FourStateOption.h"
#include "Services.h"
#include "protocol/simulationsenders.h"

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
    , simulation_     ( false )
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
    bool* pDummy =
        name == "VisionCones" ? &displayCones_
      : name == "VisionSurfaces" ? &displaySurfaces_
      : 0;
    if( pDummy )
    {
        *pDummy = value.To< FourStateOption >().IsSet( true, true );
        ToggleVisionCones();
    }

    pDummy =
          name == "FogOfWar" ? &displayFog_
        : 0;
    if( pDummy )
    {
        *pDummy = value.To< bool >();
        ToggleVisionCones();
    }
}

// -----------------------------------------------------------------------------
// Name: VisionConesToggler::NotifyUpdated
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void VisionConesToggler::NotifyUpdated( const Profile_ABC& )
{
    ToggleVisionCones();
}

// -----------------------------------------------------------------------------
// Name: VisionConesToggler::NotifyUpdated
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void VisionConesToggler::NotifyUpdated( const Services& services )
{
    simulation_ = services.HasService< simulation::Service >();
}

// -----------------------------------------------------------------------------
// Name: VisionConesToggler::ToggleVisionCones
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void VisionConesToggler::ToggleVisionCones()
{
    if( simulation_ )
    {
        simulation::ControlToggleVisionCones msg;
        msg().set_vision_cones( displayCones_ || displaySurfaces_ || displayFog_ );
        msg.Send( publisher_ );
    }
}
