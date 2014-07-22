// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Curve.h"
#include "LocationVisitor_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Curve constructor
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
Curve::Curve()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Curve destructor
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
Curve::~Curve()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Curve::PopPoint
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void Curve::PopPoint()
{
    if( ! points_.empty() )
        points_.pop_back();
}

// -----------------------------------------------------------------------------
// Name: Curve::AddPoint
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void Curve::AddPoint( const geometry::Point2f& point )
{
    if( points_.size() < 3 )
        points_.push_back( point );
}

// -----------------------------------------------------------------------------
// Name: Curve::Translate
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void Curve::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
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
// Name: Curve::IsValid
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
bool Curve::IsValid() const
{
    return points_.size() == 3;
}

// -----------------------------------------------------------------------------
// Name: Curve::IsDone
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
bool Curve::IsDone() const
{
    return IsValid();
}

// -----------------------------------------------------------------------------
// Name: Curve::Accept
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void Curve::Accept( LocationVisitor_ABC& visitor ) const
{
    visitor.VisitCurve( points_ );
}

// -----------------------------------------------------------------------------
// Name: Curve::GetName
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
QString Curve::GetName() const
{
    static const QString name = tools::translate( "Localisation", "Curve" );
    return name;
}

// -----------------------------------------------------------------------------
// Name: Curve::Clone
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
Location_ABC& Curve::Clone() const
{
    return *new Curve( *this );
}

// -----------------------------------------------------------------------------
// Name: Curve::GetTypeName
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
std::string Curve::GetTypeName() const
{
    return "curve";
}

// -----------------------------------------------------------------------------
// Name: Curve::IsAt
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
bool Curve::IsAt( const geometry::Point2f& /*point*/, float /*precision*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Curve::AddText
// Created: LGY 2014-07-22
// -----------------------------------------------------------------------------
void Curve::AddText( const QString& /*text*/, const QFont& /*font*/ )
{
    // NOTHING
}
