// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "ModuleFacade.h"
#include "PerceptionCommand.h"
#include "DisableAllPerceptionsCommand.h"
#include "TogglePerceptionCommand.h"
#include "ToggleRadarCommand.h"
#include "ToggleLocalizedRadarCommand.h"
#include "ToggleLocalizedPerceptionCommand.h"
#include "ToggleRecoCommand.h"
#include "ToggleObjectDetectionCommand.h"
#include "ToggleRecognitionPointCommand.h"
#include "ToggleAlatMonitoringCommand.h"
#include "VisionCommand.h"
#include "IdentifyAgentCommand.h"
#include "ExternalPerceptionCommand.h"
#include "SensorType.h"
#include "PerceptionLevel.h"
#include "RadarClass.h"
#include "RadarType.h"
#include "PerceptionFlyingShell.h"
#include "wrapper/Command.h"
#include <boost/lambda/lambda.hpp>
#include <xeumeuleu/xml.hpp>

using namespace sword;
using namespace sword::perception;

namespace
{
    DEFINE_HOOK( GetPerceptionId, 0, int, () )
    {
        static int id = 0;
        return id++;
    }
    DEFINE_HOOK( InitializePerceptionTypes, 1, void, ( const char* xml ) )
    {
        xml::xistringstream xis( xml );
        xis >> xml::start( "sensors" );
        ToggleAlatMonitoringCommand::Initialize( xis ); // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
        SensorType::Initialize( xis ); // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
        RadarType::Initialize( xis ); // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
        PerceptionFlyingShell::Initialize( xis ); // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
    }
    bool IsActivated( const sword::wrapper::View& sensor )
    {
        bool activated = false;
        sensor.VisitIdentifiedChildren( boost::lambda::var( activated ) = true );
        return activated;
    }
    DEFINE_HOOK( IsUsingActiveRadar, 1, bool, ( const SWORD_Model* entity ) )
    {
        const sword::wrapper::View view( entity );
        const bool radar = view[ "perceptions/radars/radar/activated" ]
                        || view[ "perceptions/radars/tapping/activated" ]
                        || view[ "perceptions/radars/tapping-radar/activated" ];
        const bool localizedRadar = IsActivated( view[ "perceptions/localized-radars/radar" ] )
                                 || IsActivated( view[ "perceptions/localized-radars/tapping" ] )
                                 || IsActivated( view[ "perceptions/localized-radars/tapping-radar" ] );
        const bool flyingShell = IsActivated( view[ "perceptions/flying-shell" ] );
        return radar || localizedRadar || flyingShell;
    }
    DEFINE_HOOK( IsUsingSpecializedActiveRadar, 2, bool, ( const SWORD_Model* entity, const char* radarType ) )
    {
        const sword::wrapper::View view( entity );
        const bool radar = view[ "perceptions/radars" ][ radarType ][ "activated" ];
        const bool localizedRadar = IsActivated( view[ "perceptions/localized-radars" ][ radarType ] );
        const bool flyingShell = IsActivated( view[ "perceptions/flying-shell" ] );
        return radar || localizedRadar || flyingShell;
    }
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade constructor
// Created: SLI 2011-12-13
// -----------------------------------------------------------------------------
ModuleFacade::ModuleFacade()
{
    wrapper::RegisterCommand< PerceptionCommand >               ( "perception" );
    wrapper::RegisterCommand< TogglePerceptionCommand >         ( "toggle perception" );
    wrapper::RegisterCommand< ToggleRadarCommand >              ( "toggle radar" );
    wrapper::RegisterCommand< ToggleLocalizedRadarCommand >     ( "toggle localized radar" );
    wrapper::RegisterCommand< ToggleLocalizedPerceptionCommand >( "toggle localized perception" );
    wrapper::RegisterCommand< ToggleRecoCommand >               ( "toggle reco" );
    wrapper::RegisterCommand< ToggleObjectDetectionCommand >    ( "toggle object detection" );
    wrapper::RegisterCommand< ToggleRecognitionPointCommand >   ( "toggle recognition point" );
    wrapper::RegisterCommand< ToggleAlatMonitoringCommand >     ( "toggle alat monitoring" );
    wrapper::RegisterCommand< VisionCommand >                   ( "vision" );
    wrapper::RegisterCommand< IdentifyAgentCommand >            ( "identify all agents in zone" );
    wrapper::RegisterCommand< ExternalPerceptionCommand >       ( "external perception" );
    wrapper::RegisterCommand< DisableAllPerceptionsCommand >    ( "disable all perceptions" );
    PerceptionLevel::Initialize();
    RadarClass::Initialize();
}

// -----------------------------------------------------------------------------
// Name: ModuleFacade destructor
// Created: SLI 2012-04-06
// -----------------------------------------------------------------------------
ModuleFacade::~ModuleFacade()
{
    RadarClass::Terminate();
    PerceptionLevel::Terminate();
}
