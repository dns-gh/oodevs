// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PcAttributes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/AgentExtensions.h"
#include "LogisticLinks.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PcAttributes constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
PcAttributes::PcAttributes( kernel::Entity_ABC& holder )
    : holder_( holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PcAttributes destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
PcAttributes::~PcAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PcAttributes::Draw
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
void PcAttributes::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    const LogisticLinks_ABC* l = holder_.Get< TacticalHierarchies >().GetUp().Retrieve< LogisticLinks_ABC >();
    const LogisticLinks* links = static_cast< const LogisticLinks* >( l );
    if( links )
        links->Draw( where, viewport, tools );
}
