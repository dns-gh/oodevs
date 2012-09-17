// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ToggleLocalizedRadarCommand.h"
#include "RadarClass.h"
#include "wrapper/View.h"
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
    wrapper::View GetTarget( const wrapper::View& parameters, const wrapper::View& model )
    {
        const std::size_t identifier = parameters[ "identifier" ];
        return model[ "entities" ][ identifier ][ "perceptions/localized-radars/" ][ FindRadar( parameters[ "radar-class" ], identifier ) ];
    }
}

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedRadarCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleLocalizedRadarCommand::ToggleLocalizedRadarCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& model, size_t /*identifier*/ )
    : effect_( GetTarget( parameters, model ) )
{
    const std::size_t perceptionId = parameters[ "perception-id" ];
    if( parameters[ "activated" ] )
    {
        effect_[ perceptionId ][ "localization" ] = parameters[ "localization" ];
        effect_[ perceptionId ][ "perception-id" ] = perceptionId;
    }
    else
        effect_[ perceptionId ].MarkForRemove();
}

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedRadarCommand destructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleLocalizedRadarCommand::~ToggleLocalizedRadarCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedRadarCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleLocalizedRadarCommand::Execute( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    effect_.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedRadarCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleLocalizedRadarCommand::ExecutePaused( const wrapper::View& /*parameters*/, const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
