// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ToggleRecoCommand.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: ToggleRecoCommand constructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleRecoCommand::ToggleRecoCommand( ModuleFacade& /*module*/, const wrapper::View& parameters, const wrapper::View& /*model*/, size_t /*identifier*/ )
    : identifier_  ( parameters[ "identifier" ] )
    , isActivated_ ( parameters[ "activated" ] )
    , growthSpeed_ ( ( isActivated_ && parameters[ "has-growth-speed" ] ) ? boost::optional< float >( parameters[ "growth-speed" ] ) : boost::none )
    , perceptionId_( parameters[ "perception-id" ] )
    , localization_( isActivated_ ? static_cast< TER_Localisation* >( parameters[ "localization" ].GetUserData() ) : 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleRecoCommand destructor
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
ToggleRecoCommand::~ToggleRecoCommand()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToggleRecoCommand::Execute
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRecoCommand::Execute( const wrapper::View& model ) const
{
    const wrapper::View& radar = model[ "entities" ][ identifier_ ][ "perceptions/reco"];
    wrapper::Effect effect( radar );
    if( isActivated_ )
    {
        effect[ perceptionId_ ][ "has-growth-speed" ] = static_cast< bool >( growthSpeed_ );
        effect[ perceptionId_ ][ "growth-speed" ] = growthSpeed_ ? *growthSpeed_ : 0;
        effect[ perceptionId_ ][ "localization" ].SetUserData( localization_ );
    }
    else
        effect[ perceptionId_ ].MarkForRemove();
    effect.Post();
}

// -----------------------------------------------------------------------------
// Name: ToggleRecoCommand::ExecutePaused
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void ToggleRecoCommand::ExecutePaused( const wrapper::View& /*model*/ ) const
{
    // NOTHING
}
