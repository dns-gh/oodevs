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
#include "wrapper/Hook.h"
#include "tools/Codec.h"
#include <xeumeuleu/xml.hpp>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( GetVisionObjectsInSurface, void, ( const SWORD_Model* localisation, unsigned int& emptySurface, unsigned int& forestSurface, unsigned int& urbanSurface ) )
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
ToggleAlatMonitoringCommand::ToggleAlatMonitoringCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& model, size_t /*identifier*/ )
    : effect_( model[ "entities" ][ static_cast< std::size_t >( parameters[ "identifier" ] ) ][ "perceptions/alat/monitoring" ] )
{
    const std::size_t perceptionId = parameters[ "perception-id" ];
    if( parameters[ "activated" ] )
    {
        const unsigned int currentTimeStep = model[ "tick" ];
        unsigned int nForestSurface = 0;
        unsigned int nEmptySurface  = 0;
        unsigned int nUrbanSurface  = 0;
        GET_HOOK( GetVisionObjectsInSurface )( parameters[ "localization" ], nEmptySurface, nForestSurface, nUrbanSurface );
        effect_[ perceptionId ][ "forest-detection-time-step" ] = currentTimeStep + static_cast< unsigned int >( nForestSurface * rForestSurveillanceTime_ );
        effect_[ perceptionId ][ "empty-detection-time-step" ] = currentTimeStep + static_cast< unsigned int >( nEmptySurface * rEmptySurveillanceTime_ );
        effect_[ perceptionId ][ "urban-detection-time-step" ] = currentTimeStep + static_cast< unsigned int >( nUrbanSurface * rUrbanSurveillanceTime_ );
        effect_[ perceptionId ][ "localization" ] = parameters[ "localization" ];
    }
    else
        effect_[ perceptionId ].MarkForRemove();
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
void ToggleAlatMonitoringCommand::Execute( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    effect_.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleAlatMonitoringCommand::ExecutePaused
// Created: SLI 2012-07-26
// -----------------------------------------------------------------------------
void ToggleAlatMonitoringCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
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
