// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "GhostHierarchies.h"
#include "clients_kernel/Ghost_ABC.h"

// -----------------------------------------------------------------------------
// Name: GhostHierarchies constructor
// Created: ABR 2012-06-28
// -----------------------------------------------------------------------------
GhostHierarchies::GhostHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, const std::string& level, const std::string& symbol, kernel::Entity_ABC* superior )
    : AgentHierarchies( controller, holder, level, symbol, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostHierarchies destructor
// Created: ABR 2012-06-28
// -----------------------------------------------------------------------------
GhostHierarchies::~GhostHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostHierarchies::SerializeAttributes
// Created: ABR 2012-06-28
// -----------------------------------------------------------------------------
void GhostHierarchies::SerializeAttributes( xml::xostream& xos ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const kernel::Entity_ABC* child = it->second;
        if( dynamic_cast< const kernel::Ghost_ABC* >( child ) )
        {
            xos << xml::start( "phantom" );
            it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
            xos << xml::end;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: GhostHierarchies::DoUpdate
// Created: ABR 2012-07-02
// -----------------------------------------------------------------------------
void GhostHierarchies::DoUpdate( const kernel::InstanciationComplete& instanciation )
{
    if( GetSuperior() && GetSuperior()->GetTypeName() == kernel::Ghost_ABC::typeName_ )
        return;
    AgentHierarchies::DoUpdate( instanciation );
}
