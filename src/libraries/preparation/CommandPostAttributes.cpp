// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "CommandPostAttributes.h"
#include "LogisticBaseStates.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/AgentType.h"

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes constructor
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
CommandPostAttributes::CommandPostAttributes( xml::xistream& xis, const kernel::Entity_ABC& entity, const kernel::AgentType& type,
                                              gui::PropertiesDictionary& dictionary )
    : entity_     ( entity )
    , type_       ( type )
    , commandPost_( xis.attribute< bool >( "command-post", false ) )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes constructor
// Created: LGY 2011-11-03
// -----------------------------------------------------------------------------
CommandPostAttributes::CommandPostAttributes( const kernel::Entity_ABC& entity, const kernel::AgentType& type,
                                              gui::PropertiesDictionary& dictionary, bool commandPost )
    : entity_     ( entity )
    , type_       ( type )
    , commandPost_( commandPost )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes destructor
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
CommandPostAttributes::~CommandPostAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes::CreateDictionary
// Created: LGY 2011-11-03
// -----------------------------------------------------------------------------
void CommandPostAttributes::CreateDictionary( gui::PropertiesDictionary& dictionary )
{
    dictionary.Register( entity_, tools::translate( "Agent", "Info/Command post" ), commandPost_ );
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes::Draw
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void CommandPostAttributes::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( commandPost_ && !entity_.IsAnAggregatedSubordinate() )
    {
        if( viewport.IsHotpointVisible() )
            tools.DrawApp6SymbolFixedSize( type_.GetHQSymbol(), where, -1.f, 0 );

        const kernel::Entity_ABC& superior = entity_.Get< kernel::TacticalHierarchies >().GetUp();
        if( const LogisticBaseStates* bl = static_cast< const LogisticBaseStates* >( superior.Retrieve< gui::LogisticHierarchiesBase >() ) )
            bl->Draw( where, viewport, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes::SerializeAttributes
// Created: LGY 2011-11-03
// -----------------------------------------------------------------------------
void CommandPostAttributes::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "command-post", commandPost_ );
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes::IsCommandPost
// Created: LGY 2011-11-03
// -----------------------------------------------------------------------------
bool CommandPostAttributes::IsCommandPost() const
{
    return commandPost_;
}
