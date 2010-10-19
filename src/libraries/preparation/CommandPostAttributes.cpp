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
#include "Tc2States.h"
#include "LogisticBaseStates.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

// -----------------------------------------------------------------------------
// Name: CommandPostAttributes constructor
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
CommandPostAttributes::CommandPostAttributes( const kernel::Entity_ABC& entity )
    : entity_( entity )
{
    // NOTHING
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
// Name: CommandPostAttributes::Draw
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void CommandPostAttributes::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    const kernel::Entity_ABC& superior = entity_.Get< kernel::TacticalHierarchies >().GetUp();
    if( const Tc2States* tc2 = static_cast< const Tc2States* >( superior.Retrieve< kernel::TC2Hierarchies >() ) )
        tc2->Draw( where, viewport, tools );
    if( const LogisticBaseStates* bl = static_cast< const LogisticBaseStates* >( superior.Retrieve< kernel::LogisticBaseHierarchies >() ) )
        bl->Draw( where, viewport, tools );
}

