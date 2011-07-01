// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MissionParameters.h"
#include "actions/Action_ABC.h"
#include "actions/ActionFactory_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/Protocol.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: MissionParameters constructor
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
MissionParameters::MissionParameters( kernel::Controller& controller, const actions::ActionFactory_ABC& factory )
    : controller_( controller )
    , factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameters destructor
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
MissionParameters::~MissionParameters()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::UpdateMessage
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
template< typename T >
void MissionParameters::UpdateMessage( const T& message )
{
    if( message.type().id() == 0 )
        return;

    try
    {
        Action_ABC* action = factory_.CreateAction( message );
        Register( action->GetId(), *action );
        controller_.Update( *this );
    }
    catch( ... )
    {
        // $$$$ SBO 2010-05-14: Warning/invalid mission
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const sword::UnitOrder& message )
{
    UpdateMessage( message );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const sword::AutomatOrder& message )
{
    UpdateMessage( message );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const sword::CrowdOrder& message )
{
    UpdateMessage( message );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: MGD 2010-12-27
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const sword::FragOrder& message )
{
    UpdateMessage( message );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::Draw
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void MissionParameters::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !elements_.empty() && tools.ShouldDisplay( "MissionParameters" ) )
    {
        Action_ABC* action = ( --elements_.end() )->second;
        if( action )
            action->Draw( where, viewport, tools );
    }
}
