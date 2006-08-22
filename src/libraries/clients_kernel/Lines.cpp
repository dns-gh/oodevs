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
// Name: Lines::Draw
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Lines::Draw( const GlTools_ABC& tools ) const
{
    if( IsValid() )
        tools.DrawLines( points_ );
}
    
// -----------------------------------------------------------------------------
// Name: Lines::GetName
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
std::string Lines::GetName() const
{
    return "ligne";
}
