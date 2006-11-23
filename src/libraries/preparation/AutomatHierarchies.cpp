// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AutomatHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies constructor
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
AutomatHierarchies::AutomatHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::MergingTacticalHierarchies( controller, holder, 0 )
    , superior_( superior )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AutomatHierarchies destructor
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
AutomatHierarchies::~AutomatHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::ChangeSuperior
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void AutomatHierarchies::ChangeSuperior( kernel::Entity_ABC& superior )
{
    MergingTacticalHierarchies::ChangeSuperior( &superior );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::SerializeAttributes
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void AutomatHierarchies::SerializeAttributes( xml::xostream& xos ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "unit" );
        it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        xos << end();
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::DoUpdate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void AutomatHierarchies::DoUpdate( const kernel::InstanciationComplete& ic )
{
    SetSuperior( superior_ );
    MergingTacticalHierarchies::DoUpdate( ic );
}
