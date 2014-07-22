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
            if( it == points_.begin() )
                points_.back() = points_.front();
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
    return !IsSegment();
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
// Name: Polygon::IsSegment
// Created: JSR 2010-05-28
// -----------------------------------------------------------------------------
bool Polygon::IsSegment() const
{
    // returns false it at least 3 points are different, true otherwise
    if( points_.size() > 2 )
        for( auto it = points_.begin() + 1; it != points_.end(); ++it )
            if( *it != points_.front() )
                for( CIT_PointVector it2 = it; it2 != points_.end(); ++it2 )
                    if( *it2 != *it && *it2 != points_.front() )
                        return false;

    return true;
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

// -----------------------------------------------------------------------------
// Name: Polygon::GetTypeName
// Created: JSR 2012-02-17
// -----------------------------------------------------------------------------
std::string Polygon::GetTypeName() const
{
    return "polygon";
}

// -----------------------------------------------------------------------------
// Name: Polygon::GetPoints
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
const T_PointVector& Polygon::GetPoints() const
{
    return points_;
}

// -----------------------------------------------------------------------------
// Name: Polygon::AddText
// Created: LGY 2014-07-22
// -----------------------------------------------------------------------------
void Polygon::AddText( const QString& /*text*/, const QFont& /*font*/ )
{
    // NOTHING
}
