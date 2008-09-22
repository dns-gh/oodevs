// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Lines.h"
#include "GlTools_ABC.h"
#include "LocationVisitor_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Lines constructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
Lines::Lines()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lines destructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
Lines::~Lines()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lines::PopPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Lines::PopPoint()
{
    if( ! points_.empty() )
        points_.pop_back();
}
    
// -----------------------------------------------------------------------------
// Name: Lines::AddPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Lines::AddPoint( const geometry::Point2f& point )
{
    points_.push_back( point );
}

// -----------------------------------------------------------------------------
// Name: Lines::Translate
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void Lines::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
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
}

// -----------------------------------------------------------------------------
// Name: Lines::IsValid
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
bool Lines::IsValid() const
{
    return points_.size() >= 2;
}

// -----------------------------------------------------------------------------
// Name: Lines::IsDone
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
bool Lines::IsDone() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Lines::Accept
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Lines::Accept( LocationVisitor_ABC& visitor ) const
{
    visitor.VisitLines( points_ );
}
    
// -----------------------------------------------------------------------------
// Name: Lines::GetName
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
QString Lines::GetName() const
{
    static const QString name = tools::translate( "Localisation", "Line" );
    return name;
}

// -----------------------------------------------------------------------------
// Name: Lines::Clone
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
Location_ABC& Lines::Clone() const
{
    return *new Lines( *this );
}
