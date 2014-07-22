// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Rectangle.h"
#include "LocationVisitor_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Rectangle constructor
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
Rectangle::Rectangle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Rectangle destructor
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
Rectangle::~Rectangle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Rectangle::PopPoint
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void Rectangle::PopPoint()
{
    if( ! points_.empty() )
        points_.pop_back();
}

// -----------------------------------------------------------------------------
// Name: Rectangle::AddPoint
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void Rectangle::AddPoint( const geometry::Point2f& point )
{
    if( points_.size() < 2 )
        points_.push_back( point );
}

// -----------------------------------------------------------------------------
// Name: Rectangle::Translate
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void Rectangle::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
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
// Name: Rectangle::IsValid
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
bool Rectangle::IsValid() const
{
    return points_.size() == 2;
}

// -----------------------------------------------------------------------------
// Name: Rectangle::IsDone
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
bool Rectangle::IsDone() const
{
    return IsValid();
}

// -----------------------------------------------------------------------------
// Name: Rectangle::Accept
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void Rectangle::Accept( LocationVisitor_ABC& visitor ) const
{
    visitor.VisitRectangle( points_ );
}

// -----------------------------------------------------------------------------
// Name: Rectangle::GetName
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
QString Rectangle::GetName() const
{
    static const QString name = tools::translate( "Localisation", "Rectangle" );
    return name;
}

// -----------------------------------------------------------------------------
// Name: Rectangle::Clone
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
Location_ABC& Rectangle::Clone() const
{
    return *new Rectangle( *this );
}

// -----------------------------------------------------------------------------
// Name: Rectangle::GetTypeName
// Created: JSR 2012-02-17
// -----------------------------------------------------------------------------
std::string Rectangle::GetTypeName() const
{
    return "rectangle";
}

// -----------------------------------------------------------------------------
// Name: Rectangle::AddText
// Created: LGY 2014-07-22
// -----------------------------------------------------------------------------
void Rectangle::AddText( const QString& /*text*/, const QFont& /*font*/ )
{
    // NOTHING
}
