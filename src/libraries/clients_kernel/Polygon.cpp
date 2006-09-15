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
    if( ! points_.empty() )
        points_.pop_back();
}

// -----------------------------------------------------------------------------
// Name: Polygon::AddPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Polygon::AddPoint( const geometry::Point2f& point )
{
    points_.push_back( point );
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
// Name: Polygon::Draw
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Polygon::Draw( const GlTools_ABC& tools ) const
{
    if( ! points_.empty() )
    {
        tools.DrawLines( points_ );
        tools.DrawLine( points_.back(), points_.front() );
    }
}
 
// -----------------------------------------------------------------------------
// Name: Polygon::GetName
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
QString Polygon::GetName() const
{
    static const QString name = tools::translate( "Localisation", "Polygone" );
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
