// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "CommandPostAttributes.h"
#include "LogisticLinks.h"
#include "DebugPoints.h"
#include "clients_gui/AggregatedTools.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes constructor
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
CommandPostAttributes::CommandPostAttributes( kernel::Entity_ABC& entity, const sword::UnitCreation& message,
                                              const tools::Resolver_ABC< kernel::AgentType >& resolver )
    : entity_     ( entity )
    , type_       ( resolver.Get( message.type().id() ) )
    , commandPost_( false )
{
    commandPost_ = ( message.pc() != 0 );
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes destructor
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
CommandPostAttributes::~CommandPostAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes::IsCommandPost
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
bool CommandPostAttributes::IsCommandPost() const
{
    return commandPost_;
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes::Draw
// Created: LGY 2011-11-03
// -----------------------------------------------------------------------------
void CommandPostAttributes::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( commandPost_ && !entity_.IsAggregated() )
    {
        if( viewport.IsHotpointVisible() )
            tools.DrawApp6SymbolFixedSize( type_.GetHQSymbol(), where, -1.f, 0 );

        const kernel::Entity_ABC& automata = entity_.Get< kernel::TacticalHierarchies >().GetUp();
        if( const LogisticLinks* links = automata.Retrieve< LogisticLinks >() )
            links->Draw( where, viewport, tools );
        if( const DebugPoints* points = automata.Retrieve< DebugPoints >() )
            points->Draw( where, viewport, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes::DoUpdate
// Created: LDC 2012-05-03
// -----------------------------------------------------------------------------
void CommandPostAttributes::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_headquarters() )
        commandPost_ = message.headquarters();
}
