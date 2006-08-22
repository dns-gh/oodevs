// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Circle.h"
#include "LocationVisitor_ABC.h"
#include "GlTools_ABC.h"

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
// Name: Circle::Draw
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Circle::Draw( const GlTools_ABC& tools ) const
{
    if( IsValid() )
        tools.DrawCircle( center_, radius_ );
}

// -----------------------------------------------------------------------------
// Name: Circle::GetName
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
std::string Circle::GetName() const
{
    // $$$$ AGE 2006-08-22: $$tr$$
    return "cercle";
}
