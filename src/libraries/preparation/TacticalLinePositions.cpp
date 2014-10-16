// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TacticalLinePositions.h"
#include "TacticalLine_ABC.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions constructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::TacticalLinePositions( kernel::Controller& controller, const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter,
                                              const kernel::TacticalLine_ABC& owner )
    : TacticalLinePositions_ABC( controller, pointList, converter, owner )
    , isEditing_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions constructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::TacticalLinePositions( kernel::Controller& controller, xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter,
                                              const kernel::TacticalLine_ABC& owner )
    : TacticalLinePositions_ABC( controller, converter, owner )
    , isEditing_( false )
{
    xis >> xml::list( "point", *this, &TacticalLinePositions::ReadPoint );
    ComputeBoundingBox();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions destructor
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
TacticalLinePositions::~TacticalLinePositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::ReadPoint
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void TacticalLinePositions::ReadPoint( xml::xistream& xis )
{
    std::string mgrs;
    xis >> mgrs;
    pointList_.push_back( converter_.ConvertToXY( mgrs ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::SerializeAttributes
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
void TacticalLinePositions::SerializeAttributes( xml::xostream& xos ) const
{
    for( CIT_PointVector itPoint = pointList_.begin() ; itPoint != pointList_.end() ; ++itPoint )
        xos << xml::start( "point" ) << converter_.ConvertToMgrs( *itPoint ) << xml::end;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::Translate
// Created: SBO 2006-12-18
// -----------------------------------------------------------------------------
void TacticalLinePositions::Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision )
{
    const float squarePrecision = precision * precision;
    for( IT_PointVector it = pointList_.begin(); it != pointList_.end(); ++it )
        if( it->SquareDistance( from ) < squarePrecision )
        {
            *it += translation;
            ComputeBoundingBox();
            return;
        }
    for( IT_PointVector it = pointList_.begin(); it != pointList_.end(); ++it )
        *it += translation;
    ComputeBoundingBox();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::Edit
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
void TacticalLinePositions::Edit( gui::ParametersLayer& parameters )
{
    isEditing_ = true;
    parameters.Start( *this, *location_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::Reset
// Created: ABR 2013-02-04
// -----------------------------------------------------------------------------
void TacticalLinePositions::Reset()
{
    isEditing_ = false;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::Handle
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
void TacticalLinePositions::Handle( kernel::Location_ABC& location )
{
    if( location.IsValid() )
    {
        location.Accept( *this );
        ComputeBoundingBox();
    }
    isEditing_ = false;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions::Draw
// Created: ABR 2013-02-04
// -----------------------------------------------------------------------------
void TacticalLinePositions::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( isEditing_ )
        return;
    TacticalLinePositions_ABC::Draw( where, viewport, tools );
}
