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
// Name: ToggleLocalizedRadarCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleLocalizedRadarCommand::ToggleLocalizedRadarCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_  ( parameters[ "identifier" ] )
    , radarClass_  ( FindRadar( parameters[ "radar-class" ], identifier_ ) )
    , isActivated_ ( parameters[ "activated" ] )
    , perceptionId_( parameters[ "perception-id" ] )
    , localization_( isActivated_ ? static_cast< TER_Localisation* >( parameters[ "localization" ].GetUserData() ) : 0 )
{
    // NOTHING
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
void ToggleLocalizedRadarCommand::Execute( const wrapper::View& model ) const
{
    const wrapper::View& radar = model[ "entities" ][ identifier_ ][ "perceptions/localized-radars"][ radarClass_ ];
    wrapper::Effect effect( radar );
    if( isActivated_ )
        effect[ perceptionId_ ].SetUserData( localization_ );
    else
        effect[ perceptionId_ ].MarkForRemove();
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleLocalizedRadarCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleLocalizedRadarCommand::ExecutePaused( const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
