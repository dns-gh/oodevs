// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationHierarchies.h"
#include "AutomatDecisions.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: FormationHierarchies constructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationHierarchies::FormationHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::MergingTacticalHierarchies( controller, holder, 0 )
    , superior_( superior )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: FormationHierarchies destructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationHierarchies::~FormationHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::ChangeSuperior
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void FormationHierarchies::ChangeSuperior( kernel::Entity_ABC& superior )
{
    kernel::MergingTacticalHierarchies::ChangeSuperior( &superior );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::SerializeAttributes
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void FormationHierarchies::SerializeAttributes( xml::xostream& xos ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( it->second->Retrieve< AutomatDecisions >() ? "automat" : "formation" ); // $$$$ SBO 2006-09-22: bof bof
        it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        xos << end();
    }
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::SerializeLogistics
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void FormationHierarchies::SerializeLogistics( xml::xostream& xos ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->Retrieve< AutomatDecisions >() )
        {
            const kernel::Automat_ABC* automat = static_cast< const kernel::Automat_ABC* >( it->second );
            if( automat && automat->GetType().HasLogistics() )
            {
                xos << start( "automat" );
                it->second->Interface().Apply( & Serializable_ABC::SerializeLogistics, xos );
                xos << end();
            }
        }
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::DoUpdate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void FormationHierarchies::DoUpdate( const kernel::InstanciationComplete& ic )
{
    SetSuperior( superior_ );
    kernel::MergingTacticalHierarchies::DoUpdate( ic );
}
