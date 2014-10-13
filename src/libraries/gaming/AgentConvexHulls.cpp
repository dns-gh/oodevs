// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentConvexHulls.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: AgentConvexHulls constructor
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
AgentConvexHulls::AgentConvexHulls( const kernel::Entity_ABC& holder, const kernel::CoordinateConverter_ABC& converter )
     : ConvexHulls( holder )
     , converter_ ( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentConvexHulls destructor
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
AgentConvexHulls::~AgentConvexHulls()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentConvexHulls::DoUpdate
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void AgentConvexHulls::DoUpdate( const sword::UnitAttributes& attributes )
{
    if( attributes.has_position()  )
        SetPosition( converter_.ConvertToXY( attributes.position() ) );
    if( attributes.has_dead()  )
        SetDead( attributes.dead() != 0 );
}

// -----------------------------------------------------------------------------
// Name: AgentConvexHulls::Draw
// Created: AGE 2007-06-14
// -----------------------------------------------------------------------------
void AgentConvexHulls::Draw( const geometry::Point2f& , const gui::Viewport_ABC& , gui::GLView_ABC& ) const
{
    // NOTHING
}
