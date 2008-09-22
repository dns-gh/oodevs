// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Polygon.h"
#include "LocationVisitor_ABC.h"
#include "GlTools_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Polygon constructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
Polygon::Polygon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon destructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
Polygon::~Polygon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Polygon::PopPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Polygon::PopPoint()
{
    if( points_.size() > 2 )
    {
        std::swap( *( points_.end() - 2 ), points_.back() );
        points_.pop_back();
    }
    else if( points_.size() == 2 )
        points_.clear();
}

// -----------------------------------------------------------------------------
// Name: Polygon::AddPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Polygon::AddPoint( const geometry::Point2f& point )
{
    if( points_.empty() )
        points_.push_back( point );
    points_.push_back( point );
    if( points_.size() > 2 )
        std::swap( *( points_.end() - 2 ), points_.back() );
}

// -----------------------------------------------------------------------------
// Name: Polygon::Translate
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void Polygon::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
{
    const float squarePrecision = precision * precision;
    for( IT_PointVector it = points_.begin(); it != points_.end(); ++it )
        if( it->SquareDistance( from ) < squarePrecision )
        {
            *it += translation;
            return;
        }
    for( IT_PointVector it = points_.begin(); it != points_.end(); ++it )
        *it += translation;
    if( ! points_.empty() )
        points_.back() = points_.front();
}

// -----------------------------------------------------------------------------
// Name: Polygon::IsValid
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
bool Polygon::IsValid() const
{
    return points_.size() >= 3;
}

// -----------------------------------------------------------------------------
// Name: Polygon::IsDone
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
bool Polygon::IsDone() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Polygon::Accept
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Polygon::Accept( LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPolygon( points_ );
}

// -----------------------------------------------------------------------------
// Name: Polygon::GetName
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
QString Polygon::GetName() const
{
    static const QString name = tools::translate( "Localisation", "Polygon" );
    return name;
}

// -----------------------------------------------------------------------------
// Name: Polygon::Clone
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
Location_ABC& Polygon::Clone() const
{
    return *new Polygon( *this );
}
