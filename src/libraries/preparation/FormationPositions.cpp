// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationPositions.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

// -----------------------------------------------------------------------------
// Name: FormationPositions constructor
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
FormationPositions::FormationPositions( const kernel::Entity_ABC& formation )
    : formation_( formation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationPositions destructor
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
FormationPositions::~FormationPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::GetPosition
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
geometry::Point2f FormationPositions::GetPosition() const
{
    geometry::Point2f aggregatedPosition;
    unsigned count = 0;
    tools::Iterator< const kernel::Entity_ABC& > children = formation_.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const geometry::Point2f& childPosition = children.NextElement().Get< kernel::Positions >().GetPosition();
        aggregatedPosition.Set( aggregatedPosition.X() + childPosition.X(), aggregatedPosition.Y() + childPosition.Y() );
        ++count;
    }
    return count ? geometry::Point2f( aggregatedPosition.X() / count, aggregatedPosition.Y() / count ) : aggregatedPosition;
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::GetHeight
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
float FormationPositions::GetHeight() const
{
    float height = 0;
    unsigned count = 0;
    tools::Iterator< const kernel::Entity_ABC& > children = formation_.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        height += children.NextElement().Get< kernel::Positions >().GetHeight();
        ++count;
    }
    return count ? height / count : height;
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::IsAt
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
bool FormationPositions::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/, float /*adaptiveFactor = 1.f*/ ) const
{
    // $$$$ SBO 2009-02-02: Aggregated symbol position
    const float halfSizeX = 500.f * 0.5f * 2.f; // $$$$ SBO 2006-03-21: use font size?
    const float sizeY     = 400.f * 2.f;
    const geometry::Point2f position = GetPosition();
    const geometry::Rectangle2f agentBBox( position.X() - halfSizeX - precision, position.Y() - precision,
                                           position.X() + halfSizeX + precision, position.Y() + sizeY + precision);
    return agentBBox.IsInside( pos );
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::IsIn
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
bool FormationPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::GetBoundingBox
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
geometry::Rectangle2f FormationPositions::GetBoundingBox() const
{
    const geometry::Point2f center = GetPosition();
    return geometry::Rectangle2f( center.X() - 500, center.Y(), center.X() + 500, center.Y() + 800 );
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void FormationPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: FormationPositions::Move
// Created: SBO 2009-02-02
// -----------------------------------------------------------------------------
void FormationPositions::Move( const geometry::Point2f& point )
{
    const geometry::Vector2f vect( GetPosition(), point );
    tools::Iterator< const kernel::Entity_ABC& > children = formation_.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        const kernel::Positions* positions = children.NextElement().Retrieve< kernel::Positions >();
        if( const kernel::Moveable_ABC* childPositions = dynamic_cast< const kernel::Moveable_ABC* >( positions ) )
            const_cast< kernel::Moveable_ABC& >( *childPositions ).Move( positions->GetPosition() + vect );
    }
}
