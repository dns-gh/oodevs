// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: TeamHierarchies constructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
TeamHierarchies::TeamHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : TacticalHierarchies( controller, holder, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies destructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
TeamHierarchies::~TeamHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies::SerializeAttributes
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void TeamHierarchies::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "tactical" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "formation" );
        it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        xos << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies::SerializeLogistics
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void TeamHierarchies::SerializeLogistics( xml::xostream& xos ) const
{
    xos << xml::start( "logistics" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Interface().Apply( & Serializable_ABC::SerializeLogistics, xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies::UpdateSymbol
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
void TeamHierarchies::UpdateSymbol( bool up /* = true*/ )
{
    if( ! up )
    {
        tools::Iterator< const kernel::Entity_ABC& > it = CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::TacticalHierarchies* child = it.NextElement().Retrieve< kernel::TacticalHierarchies >();
            if( child )
                const_cast< kernel::TacticalHierarchies* >( child )->UpdateSymbol( false );
        }
    }
}
