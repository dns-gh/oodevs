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
#include "clients_kernel/TacticalHierarchies.h"

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes constructor
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
CommandPostAttributes::CommandPostAttributes( xml::xistream& xis, kernel::Controller& controller, const kernel::Entity_ABC& entity, const kernel::AgentType& type,
                                              kernel::PropertiesDictionary& dictionary )
    : kernel::CommandPostAttributes( entity, type, xis.attribute< bool >( "command-post", false ) )
    , controller_ ( controller ) 
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes constructor
// Created: LGY 2011-11-03
// -----------------------------------------------------------------------------
CommandPostAttributes::CommandPostAttributes( const kernel::Entity_ABC& entity, kernel::Controller& controller, const kernel::AgentType& type,
                                              kernel::PropertiesDictionary& dictionary, bool commandPost )
    : kernel::CommandPostAttributes( entity, type, commandPost )
    , controller_ ( controller ) 
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
void CommandPostAttributes::CreateDictionary( kernel::PropertiesDictionary& dictionary )
{
    dictionary.Register( entity_, tools::translate( "Agent", "Info/Command post" ), commandPost_, *this, &CommandPostAttributes::Update );
}

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes::Draw
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void CommandPostAttributes::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( commandPost_ )
    {
        const kernel::Entity_ABC& superior = entity_.Get< kernel::TacticalHierarchies >().GetUp();
        if( const LogisticBaseStates* bl = static_cast< const LogisticBaseStates* >( superior.Retrieve< LogisticHierarchiesBase >() ) )
            bl->Draw( where, viewport, tools );
    }
    kernel::CommandPostAttributes::Draw( where, viewport, tools );
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
// Name: CommandPostAttributes::Update
// Created: LGY 2012-02-06
// -----------------------------------------------------------------------------
void CommandPostAttributes::Update( const bool& commandPost )
{
    commandPost_ = commandPost;
    if( const kernel::Symbol_ABC* symbol = entity_.Retrieve< kernel::TacticalHierarchies >() )
        controller_.Update( *symbol );
}
//