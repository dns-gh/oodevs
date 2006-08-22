// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Point.h"
#include "LocationVisitor_ABC.h"
#include "GlTools_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
Point::Point()
    : pointSet_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point destructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
Point::~Point()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point::PopPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Point::PopPoint()
{
    pointSet_ = false;
}

// -----------------------------------------------------------------------------
// Name: Point::AddPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Point::AddPoint( const geometry::Point2f& point )
{
    pointSet_ = true;
    point_ = point;
}

// -----------------------------------------------------------------------------
// Name: Point::IsValid
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
bool Point::IsValid() const
{
    return pointSet_;
}

// -----------------------------------------------------------------------------
// Name: Point::IsDone
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
bool Point::IsDone() const
{
    return IsValid();
}

// -----------------------------------------------------------------------------
// Name: Point::Accept
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Point::Accept( LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( point_ );   
}

// -----------------------------------------------------------------------------
// Name: Point::Draw
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Point::Draw( const GlTools_ABC& tools ) const
{
    if( IsValid() )
        tools.DrawCross( point_ );
}

// -----------------------------------------------------------------------------
// Name: Point::GetName
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
std::string Point::GetName() const
{
    return "point";
}
