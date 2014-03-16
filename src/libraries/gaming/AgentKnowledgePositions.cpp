// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentKnowledgePositions.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions constructor
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
AgentKnowledgePositions::AgentKnowledgePositions( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions destructor
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
AgentKnowledgePositions::~AgentKnowledgePositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions::GetPosition
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
geometry::Point2f AgentKnowledgePositions::GetPosition( bool ) const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions::GetHeight
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
float AgentKnowledgePositions::GetHeight( bool ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions::IsIn
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
bool AgentKnowledgePositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( position_ );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions::DoUpdate
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void AgentKnowledgePositions::DoUpdate( const sword::UnitKnowledgeUpdate& message )
{
    if( message.has_position()  )
        position_ = converter_.ConvertToXY( message.position() );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f AgentKnowledgePositions::GetBoundingBox() const
{
    const geometry::Vector2f diag( 1000, 1000 ); // $$$$ SBO 2006-07-05: hard coded
    return geometry::Rectangle2f( position_ - diag, position_ + diag );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void AgentKnowledgePositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( position_ );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgePositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool AgentKnowledgePositions::CanAggregate() const
{
    return true;
}
