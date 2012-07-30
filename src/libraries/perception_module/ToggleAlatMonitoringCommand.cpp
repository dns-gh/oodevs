// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ToggleAlatMonitoringCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"
#include "wrapper/Hook.h"
#include "tools/Codec.h"
#include <xeumeuleu/xml.hpp>

using namespace sword;
using namespace sword::perception;

class TER_Localisation;

DECLARE_HOOK( GetVisionObjectsInSurface, void, ( const TER_Localisation* localisation, unsigned int& emptySurface, unsigned int& forestSurface, unsigned int& urbanSurface ) )
DECLARE_HOOK( ConvertSecondsToSim, double, ( double seconds ) )

namespace
{
    double rForestSurveillanceTime_ = std::numeric_limits< double >::max();
    double rUrbanSurveillanceTime_  = std::numeric_limits< double >::max();
    double rEmptySurveillanceTime_  = std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: ToggleAlatMonitoringCommand constructor
// Created: SLI 2012-07-26
// -----------------------------------------------------------------------------
ToggleAlatMonitoringCommand::ToggleAlatMonitoringCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_  ( parameters[ "identifier" ] )
    , isActivated_ ( parameters[ "activated" ] )
    , perceptionId_( parameters[ "perception-id" ] )
    , localization_( isActivated_ ? parameters[ "localization" ].GetUserData() : 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleAlatMonitoringCommand destructor
// Created: SLI 2012-07-26
// -----------------------------------------------------------------------------
ToggleAlatMonitoringCommand::~ToggleAlatMonitoringCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleAlatMonitoringCommand::Execute
// Created: SLI 2012-07-26
// -----------------------------------------------------------------------------
void ToggleAlatMonitoringCommand::Execute( const wrapper::View& model ) const
{
    const wrapper::View& perception = model[ "entities" ][ identifier_ ][ "perceptions/alat/monitoring"];
    wrapper::Effect effect( perception );
    if( isActivated_ )
    {
        effect[ perceptionId_ ][ "localization" ].SetUserData( localization_ );
        const unsigned int currentTimeStep = model[ "step" ];
        unsigned int nForestSurface = 0;
        unsigned int nEmptySurface  = 0;
        unsigned int nUrbanSurface  = 0;
        GET_HOOK( GetVisionObjectsInSurface )( reinterpret_cast< const TER_Localisation* >( localization_ ), nEmptySurface, nForestSurface, nUrbanSurface );
        effect[ perceptionId_ ][ "forest-detection-time-step" ] = currentTimeStep + static_cast< unsigned int >( nForestSurface * rForestSurveillanceTime_ );
        effect[ perceptionId_ ][ "empty-detection-time-step" ] = currentTimeStep + static_cast< unsigned int >( nEmptySurface * rEmptySurveillanceTime_ );
        effect[ perceptionId_ ][ "urban-detection-time-step" ] = currentTimeStep + static_cast< unsigned int >( nUrbanSurface * rUrbanSurveillanceTime_ );
    }
    else
        effect[ perceptionId_ ].MarkForRemove();
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleAlatMonitoringCommand::ExecutePaused
// Created: SLI 2012-07-26
// -----------------------------------------------------------------------------
void ToggleAlatMonitoringCommand::ExecutePaused( const wrapper::View& /*model*/ ) const
{
    // NOTHING
}

struct ToggleAlatMonitoringCommand::LoadingWrapper
{
    void ReadAlatTime( xml::xistream& xis )
    {
        ToggleAlatMonitoringCommand::ReadAlatTime( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: ToggleAlatMonitoringCommand::Initialize
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
void ToggleAlatMonitoringCommand::Initialize( xml::xistream& xis )
{
    LoadingWrapper loader;
    xis >> xml::start( "alat-monitoring-times" )
            >> xml::list( "alat-monitoring-time", loader, &LoadingWrapper::ReadAlatTime )
        >> xml::end;
}

namespace
{
    void UpdateTime( xml::xistream& xis, double& time )
    {
        tools::ReadTimeAttribute( xis, "time", time );
        time = GET_HOOK( ConvertSecondsToSim )( time ); // second.hectare-1 => dT.hectare-1
        time /= 10000.;                                 // dT.hectare-1     => dT.m-2
        time = 1. / ( 1. / time );                      // dT.m-2           => dT.px-2
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoSurveillance::ReadAlatTime
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void ToggleAlatMonitoringCommand::ReadAlatTime( xml::xistream& xis )
{
    const std::string terrain = xis.attribute< std::string >( "terrain" );
    if( terrain == "Vide" )
        UpdateTime( xis, rEmptySurveillanceTime_ );
    else if( terrain == "Foret" )
        UpdateTime( xis, rForestSurveillanceTime_ );
    else if( terrain == "Urbain" )
        UpdateTime( xis, rUrbanSurveillanceTime_ );
}
