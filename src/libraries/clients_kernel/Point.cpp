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
#include "Tools.h"

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
// Name: Point::Translate
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void Point::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
{
    if( IsValid() && point_.SquareDistance( from ) < precision * precision )
        point_ += translation;
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
// Name: Point::GetName
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
QString Point::GetName() const
{
    static const QString name = tools::translate( "Localisation", "Point" );
    return name;
}

// -----------------------------------------------------------------------------
// Name: Point::Clone
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
Location_ABC& Point::Clone() const
{
    return *new Point( *this );
}

// -----------------------------------------------------------------------------
// Name: Point::GetTypeName
// Created: JSR 2012-02-17
// -----------------------------------------------------------------------------
std::string Point::GetTypeName() const
{
    return "point";
}

// -----------------------------------------------------------------------------
// Name: Point::AddText
// Created: LGY 2014-07-22
// -----------------------------------------------------------------------------
void Point::AddText( const QString& /*text*/, const QFont& /*font*/ )
{
    // NOTHING
}
