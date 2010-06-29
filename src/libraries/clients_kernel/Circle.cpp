// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Circle.h"
#include "LocationVisitor_ABC.h"
#include "GlTools_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Circle constructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
Circle::Circle()
    : centerSet_( false )
    , radius_( -1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Circle destructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
Circle::~Circle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Circle::PopPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Circle::PopPoint()
{
    if( radius_ >= 0 )
        radius_ = -1;
    else
        centerSet_ = false;
}

// -----------------------------------------------------------------------------
// Name: Circle::AddPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Circle::AddPoint( const geometry::Point2f& point )
{
    if( ! centerSet_ )
    {
        center_ = point;
        centerSet_ = true;
    }
    else
        radius_ = center_.Distance( point );
}

// -----------------------------------------------------------------------------
// Name: Circle::Translate
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void Circle::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
{
    if( IsValid() && center_.SquareDistance( from ) < precision * precision )
        center_ += translation;
}

// -----------------------------------------------------------------------------
// Name: Circle::IsValid
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
bool Circle::IsValid() const
{
    return centerSet_ && radius_ > 0;
}

// -----------------------------------------------------------------------------
// Name: Circle::IsDone
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
bool Circle::IsDone() const
{
    return IsValid();
}

// -----------------------------------------------------------------------------
// Name: Circle::Accept
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Circle::Accept( LocationVisitor_ABC& visitor ) const
{
    visitor.VisitCircle( center_, radius_ );
}

// -----------------------------------------------------------------------------
// Name: Circle::GetName
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
QString Circle::GetName() const
{
    static const QString name = tools::translate( "Localisation", "Circle" );
    return name;
}

// -----------------------------------------------------------------------------
// Name: Circle::Clone
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
Location_ABC& Circle::Clone() const
{
    return *new Circle( *this );
}

// -----------------------------------------------------------------------------
// Name: Circle::IsAt
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
bool Circle::IsAt( const geometry::Point2f& point, float precision ) const
{
    return IsValid() && center_.SquareDistance( point ) < ( radius_ + precision ) * ( radius_ + precision );
}
