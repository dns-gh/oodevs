// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Path.h"
#include "LocationVisitor_ABC.h"
#include "Positions.h"
#include "GlTools_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Path constructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
Path::Path( const Positions& position )
    : position_( position )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path destructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
Path::~Path()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Path::PopPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Path::PopPoint()
{
    if( ! points_.empty() )
        points_.pop_back();
}

// -----------------------------------------------------------------------------
// Name: Path::AddPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Path::AddPoint( const geometry::Point2f& point )
{
    points_.push_back( point );
}

// -----------------------------------------------------------------------------
// Name: Path::IsValid
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
bool Path::IsValid() const
{
    return !points_.empty();
}

// -----------------------------------------------------------------------------
// Name: Path::IsDone
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
bool Path::IsDone() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Path::Accept
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Path::Accept( LocationVisitor_ABC& visitor ) const
{
    // le point d'origine n'est pas passé. Bien noter.
    visitor.VisitLines( points_ );
}

// -----------------------------------------------------------------------------
// Name: Path::Draw
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void Path::Draw( const GlTools_ABC& tools ) const
{
    if( IsValid() )
    {
        tools.DrawLine( position_.GetPosition(), points_.front() );
        tools.DrawLines( points_ );
    }
}

// -----------------------------------------------------------------------------
// Name: Path::GetName
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
QString Path::GetName() const
{
    static const QString name = tools::translate( "Localisation", "Path" );
    return name;
}

// -----------------------------------------------------------------------------
// Name: Path::Clone
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
Location_ABC& Path::Clone() const
{
    return *new Path( *this );
}
