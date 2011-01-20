// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "UrbanMagicAction.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ServerPublisher_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Controller.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::UrbanMagicAction
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
UrbanMagicAction::UrbanMagicAction( const kernel::Entity_ABC& object, kernel::Controller& controller, bool registered /*= true*/ )
    : ActionWithTarget_ABC ( controller, kernel::MagicActionType( "urban" ), object ) // $$$$ _RC_ SLG 2010-12-22: Creation d'un MagicActionType fictif
    , controller_( controller )
    , registered_( registered )
    , state_( false, 0 )
    , threshold_( false, 0. )
    , active_( false, false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::~UrbanMagicAction
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
UrbanMagicAction::~UrbanMagicAction()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::Polish
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::Serialize
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicAction::Serialize( xml::xostream& xos ) const
{
    Action_ABC::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::Publish
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicAction::Publish( Publisher_ABC& publisher, int ) const
{
    simulation::UrbanMagicAction message;
    message().mutable_id()->set_id( GetEntity().GetId() );
    if ( state_.first )
    {
        int state = state_.second;
        message().set_structural_state( state );
    }
    if ( threshold_.first )
    {
        float threshold = threshold_.second;
        message().mutable_infrastructure()->set_threshold( threshold );
    }
    if ( active_.first )
    {
        bool active = active_.second;
        message().mutable_infrastructure()->set_active( active );
    }

    message.Send( publisher );
    message().Clear();
}


// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::SetStructuralState
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicAction::SetStructuralState( int state )
{
    state_ = std::make_pair( true, state );
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::SetThreshold
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicAction::SetThreshold( float threshold )
{
    threshold_ = std::make_pair( true, threshold );
}

// -----------------------------------------------------------------------------
// Name: UrbanMagicAction::SetEnabled
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
void UrbanMagicAction::SetEnabled( bool enabled )
{
    active_ = std::make_pair( true, enabled );
}
