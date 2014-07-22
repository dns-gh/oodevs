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
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Path constructor
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
Path::Path( const Positions* position )
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
// Name: Path::SetEntityPosition
// Created: ABR 2014-01-07
// -----------------------------------------------------------------------------
void Path::SetEntityPosition( const Positions* position )
{
    position_ = position;
}

// -----------------------------------------------------------------------------
// Name: Path::FixOrigin
// Created: ABR 2014-01-07
// -----------------------------------------------------------------------------
void Path::FixOrigin( bool fixOrigin )
{
    if( fixOrigin && position_ )
        origin_ = geometry::Point2f( position_->GetPosition().X(), position_->GetPosition().Y() );
    else
        origin_ = boost::none;
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
// Name: Path::Translate
// Created: SBO 2008-05-30
// -----------------------------------------------------------------------------
void Path::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
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
    if( points_.size() == 0 )
        return;
    const geometry::Point2f start = origin_
                                    ? *origin_
                                    : position_
                                      ? position_->GetPosition()
                                      : points_[ 0 ];
    visitor.VisitPath( start, points_ );
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

// -----------------------------------------------------------------------------
// Name: Path::GetTypeName
// Created: JSR 2012-02-17
// -----------------------------------------------------------------------------
std::string Path::GetTypeName() const
{
    return "path";
}

// -----------------------------------------------------------------------------
// Name: Path::AddText
// Created: LGY 2014-07-22
// -----------------------------------------------------------------------------
void Path::AddText( const QString& /*text*/, const QFont& /*font*/ )
{
    // NOTHING
}
