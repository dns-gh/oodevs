// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "DisableAllPerceptionsCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"
#include "wrapper/Remove.h"
#include "wrapper/Event.h"
#include <boost/bind.hpp>

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: DisableAllPerceptionsCommand constructor
// Created: SLI 2012-10-03
// -----------------------------------------------------------------------------
DisableAllPerceptionsCommand::DisableAllPerceptionsCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisableAllPerceptionsCommand::Destroy
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void DisableAllPerceptionsCommand::Destroy( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}

namespace
{
    void Deactivate( const wrapper::View& perception )
    {
        wrapper::Effect effect( perception );
        effect = false;
        effect.Post();
    }
    void RemoveElement( const wrapper::View& child )
    {
        wrapper::Remove( child ).Post();
    }
    void RemoveElements( const wrapper::View& perception )
    {
        perception.VisitIdentifiedChildren( boost::bind( &RemoveElement, _2 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DisableAllPerceptionsCommand::Execute
// Created: SLI 2012-10-03
// -----------------------------------------------------------------------------
void DisableAllPerceptionsCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const std::size_t identifier = parameters[ "identifier" ];
    const wrapper::View perceptions = model[ "entities" ][ identifier ][ "perceptions" ];
    Deactivate( perceptions[ "peripherical-vision/activated" ] );
    Deactivate( perceptions[ "sensor/activated" ] );
    Deactivate( perceptions[ "scan/activated" ] );
    Deactivate( perceptions[ "radars/radar/activated" ] );
    Deactivate( perceptions[ "radars/tapping/activated" ] );
    Deactivate( perceptions[ "radars/tapping-radar/activated" ] );
    RemoveElements( perceptions[ "alat/reco" ] );
    RemoveElements( perceptions[ "alat/monitoring" ] );
    RemoveElements( perceptions[ "object-detection" ] );
    RemoveElements( perceptions[ "urban" ] );
    RemoveElements( perceptions[ "reco" ] );
    RemoveElements( perceptions[ "flying-shell/zones" ] );
    RemoveElements( perceptions[ "flying-shell/previous" ] );
    RemoveElements( perceptions[ "recognition-point" ] );
    RemoveElements( perceptions[ "radars/acquisitions" ] );
    RemoveElements( perceptions[ "localized-radars/radar" ] );
    RemoveElements( perceptions[ "localized-radars/tapping" ] );
    RemoveElements( perceptions[ "localized-radars/tapping-radar" ] );
    wrapper::Event event( "alat monitoring disabled" );
    event[ "entity/data" ] = model[ "entities" ][ identifier ][ "data" ];
    event.Post();
}

// -----------------------------------------------------------------------------
// Name: DisableAllPerceptionsCommand::ExecutePaused
// Created: SLI 2012-10-03
// -----------------------------------------------------------------------------
void DisableAllPerceptionsCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
