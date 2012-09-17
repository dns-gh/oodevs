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
#include "wrapper/View.h"
#include "wrapper/Effect.h"
#include <boost/lexical_cast.hpp>

using namespace sword;
using namespace sword::perception;

namespace
{
    std::string FindRadar( unsigned int radarClass, size_t entity )
    {
         const RadarClass* found = RadarClass::Find( radarClass );
         if( !found )
             throw std::runtime_error( "could not find radar class '" + boost::lexical_cast< std::string >( radarClass )
                                     + "' for entity '" + boost::lexical_cast< std::string >( entity ) + "'" );
         return found->GetName();
    }
}

// -----------------------------------------------------------------------------
// Name: ToggleRadarCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleRadarCommand::ToggleRadarCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_ ( parameters[ "identifier" ] )
    , radarClass_ ( FindRadar( parameters[ "radar-class" ], identifier_ ) )
    , isActivated_( parameters[ "activated" ] )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleRadarCommand destructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleRadarCommand::~ToggleRadarCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleRadarCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRadarCommand::Execute( const wrapper::View& /*parameters*/, const wrapper::View& model ) const
{
    const wrapper::View& radar = model[ "entities" ][ identifier_ ][ "perceptions/radars" ][ radarClass_ ][ "activated" ];
    if( radar == isActivated_ )
        return;
    wrapper::Effect effect( radar );
    effect = isActivated_;
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
