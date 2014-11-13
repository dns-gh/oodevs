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
#include "SimulationController.h"
#include "clients_gui/GLMainProxy.h"
#include "clients_gui/GLOptions.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/FourStateOption.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: VisionConesToggler constructor
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
VisionConesToggler::VisionConesToggler( Controllers& controllers,
                                        const SimulationController& simulationController,
                                        const gui::GLMainProxy& mainProxy,
                                        QObject* parent )
    : QObject( parent )
    , controllers_( controllers )
    , simulationController_( simulationController )
    , mainProxy_( mainProxy )
    , connected_( false )
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
void VisionConesToggler::OptionChanged( const std::string& name, const OptionVariant& )
{
    if( name == "VisionCones" || name == "VisionSurfaces" || name == "FogOfWar" )
        SendControlEnableVisionCones();
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
    bool display = false;
    mainProxy_.ApplyToOptions( [&display]( const gui::GLOptions& options ) {
        display |= options.Get( "VisionCones" ).To< FourStateOption >().IsSet( true, true, true ) ||
                   options.Get( "VisionSurfaces" ).To< FourStateOption >().IsSet( true, true, true ) ||
                   options.Get( "FogOfWar" ).To< bool >();
    } );
    if( connected_ )
        simulationController_.SendEnableVisionCones( display );
}
