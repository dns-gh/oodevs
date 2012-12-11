// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ToggleRadarCommand.h"
#include "RadarClass.h"
#include <tools/Exception.h>
#include "wrapper/View.h"
#include "wrapper/Effect.h"
#include <boost/lexical_cast.hpp>

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleRadarCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleRadarCommand::ToggleRadarCommand( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleRadarCommand::Destroy
// Created: MCO 2012-10-26
// -----------------------------------------------------------------------------
void ToggleRadarCommand::Destroy( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}

namespace
{
    std::string FindRadar( unsigned int radarClass, size_t entity )
    {
         const RadarClass* found = RadarClass::Find( radarClass );
         if( !found )
             throw MASA_EXCEPTION( "could not find radar class '" + boost::lexical_cast< std::string >( radarClass )
                                     + "' for entity '" + boost::lexical_cast< std::string >( entity ) + "'" );
         return found->GetName();
    }
}

// -----------------------------------------------------------------------------
// Name: ToggleRadarCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRadarCommand::Execute( const wrapper::View& parameters, const wrapper::View& model ) const
{
    const size_t identifier = parameters[ "identifier" ];
    const std::string radarClass = FindRadar( parameters[ "radar-class" ], identifier );
    const bool isActivated = parameters[ "activated" ];
    const wrapper::View& radar = model[ "entities" ][ identifier ][ "perceptions/radars" ][ radarClass ][ "activated" ];
    if( radar == isActivated )
        return;
    wrapper::Effect effect( radar );
    effect = isActivated;
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleRadarCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRadarCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
