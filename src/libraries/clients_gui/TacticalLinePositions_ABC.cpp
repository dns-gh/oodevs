// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TacticalLinePositions_ABC.h"

#include "GlTools_ABC.h"
#include "Viewport_ABC.h"

#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Controller.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC constructor
// Created: MMC 2012-05-14
// -----------------------------------------------------------------------------
TacticalLinePositions_ABC::TacticalLinePositions_ABC( Controller& controller, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner )
    : controller_( controller )
    , converter_( converter )
    , owner_    ( owner )
    , location_( new kernel::Lines() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC constructor
// Created: MMC 2012-05-14
// -----------------------------------------------------------------------------
TacticalLinePositions_ABC::TacticalLinePositions_ABC( Controller& controller, const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner )
    : controller_( controller )
    , converter_( converter )
    , owner_    ( owner )
    , pointList_( pointList )
    , location_( new kernel::Lines() )
{
    ComputeBoundingBox();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC destructor
// Created: MMC 2012-05-14
// -----------------------------------------------------------------------------
TacticalLinePositions_ABC::~TacticalLinePositions_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC::GetPosition
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
geometry::Point2f TacticalLinePositions_ABC::GetPosition( bool ) const
{
    return pointList_.front();
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC::GetHeight
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
float TacticalLinePositions_ABC::GetHeight( bool ) const
{
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC::IsIn
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
bool TacticalLinePositions_ABC::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC::GetBoundingBox
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
geometry::Rectangle2f TacticalLinePositions_ABC::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC::Draw
// Created: SBO 2006-11-06
// -----------------------------------------------------------------------------
void TacticalLinePositions_ABC::Draw( const geometry::Point2f&, const Viewport_ABC& viewport, GlTools_ABC& tools ) const
{
    if( pointList_.empty() || !viewport.IsVisible( boundingBox_ ) )
        return;

    std::string symbol = owner_.Get< kernel::TacticalHierarchies >().GetSymbol();
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    if( tools.ShouldDisplay() )
        for( auto it = pointList_.begin(); it != pointList_.end(); ++it )
            tools.DrawDisc( *it, 5.f, GlTools_ABC::pixels );
    glPopAttrib();

    tools.DrawTacticalGraphics( symbol, *location_, false, false );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void TacticalLinePositions_ABC::Pick( const geometry::Point2f& where, const Viewport_ABC& viewport, GlTools_ABC& tools ) const
{
    Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC::ComputeBoundingBox
// Created: SBO 2007-03-08
// -----------------------------------------------------------------------------
void TacticalLinePositions_ABC::ComputeBoundingBox()
{
    VisitLines( pointList_ );
    boundingBox_ = geometry::Rectangle2f();
    for( auto it = pointList_.begin(); it != pointList_.end(); ++it )
        boundingBox_.Incorporate( *it );
    controller_.Update( owner_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool TacticalLinePositions_ABC::CanAggregate() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void TacticalLinePositions_ABC::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitLines( pointList_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC::VisitLines
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
void TacticalLinePositions_ABC::VisitLines( const T_PointVector& points )
{
    pointList_ = points;

    location_.reset( new kernel::Lines() );
    for( auto it = pointList_.begin(); it != pointList_.end(); ++it )
    {
        const geometry::Point2f& point = *it;
        location_->AddPoint( point );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC::VisitPoint
// Created: ABR 2013-02-01
// -----------------------------------------------------------------------------
void TacticalLinePositions_ABC::VisitPoint( const geometry::Point2f& point )
{
    pointList_.clear();
    pointList_.push_back( point );

    location_.reset( new kernel::Lines() );
    for( auto it = pointList_.begin(); it != pointList_.end(); ++it )
    {
        const geometry::Point2f& point = *it;
        location_->AddPoint( point );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalLinePositions_ABC::VisitCurve
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void TacticalLinePositions_ABC::VisitCurve( const T_PointVector& /*points*/ )
{
    // NOTHING
}
