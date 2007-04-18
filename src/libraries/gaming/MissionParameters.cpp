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
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Controller.h"
#include "ActionFactory_ABC.h"
#include "Action_ABC.h"

// -----------------------------------------------------------------------------
// Name: MissionParameters constructor
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
MissionParameters::MissionParameters( kernel::Controller& controller, const ActionFactory_ABC& factory )
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
// Name: MissionParameters::DoUpdate
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const ASN1T_MsgPionOrder& message )
{
    Action_ABC* action = factory_.CreateAction( message );
    Register( action->GetId(), *action );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const ASN1T_MsgAutomateOrder& message )
{
    Action_ABC* action = factory_.CreateAction( message );
    Register( action->GetId(), *action );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const ASN1T_MsgPionOrderManagement& message )
{
//    if( message.etat == EnumOrderState::stopped )
//        mission_.reset();
//    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const ASN1T_MsgAutomateOrderManagement& message )
{
//    if( message.etat == EnumOrderState::stopped )
//        mission_.reset();
//    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::Draw
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void MissionParameters::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( tools.ShouldDisplay( "MissionParameters" ) )
        for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
            it->second->Draw( where, viewport, tools );
}
