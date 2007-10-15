// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IntelligencePositions.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IntelligencePositions constructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
IntelligencePositions::IntelligencePositions( const CoordinateConverter_ABC& converter, const geometry::Point2f& position )
    : converter_( converter )
    , position_ ( position )
    , height_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligencePositions constructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
IntelligencePositions::IntelligencePositions( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : converter_( converter )
    , height_   ( 0 )
{
    std::string mgrs;
    xis >> attribute( "position", mgrs );
    position_ = converter_.ConvertToXY( mgrs );
}

// -----------------------------------------------------------------------------
// Name: IntelligencePositions destructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
IntelligencePositions::~IntelligencePositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligencePositions::GetPosition
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
geometry::Point2f IntelligencePositions::GetPosition() const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: IntelligencePositions::GetHeight
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
float IntelligencePositions::GetHeight() const
{
    return height_;
}

// -----------------------------------------------------------------------------
// Name: IntelligencePositions::IsAt
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
bool IntelligencePositions::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
{
    const float halfSizeX = 500.f * 0.5f; // $$$$ SBO 2006-03-21: use font size?
    const float sizeY     = 400.f;
    const Point2f position = GetPosition();
    const Rectangle2f agentBBox( position.X() - halfSizeX - precision, position.Y() - precision,
                                 position.X() + halfSizeX + precision, position.Y() + sizeY + precision);
    return agentBBox.IsInside( pos );
}

// -----------------------------------------------------------------------------
// Name: IntelligencePositions::IsIn
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
bool IntelligencePositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: IntelligencePositions::GetBoundingBox
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
geometry::Rectangle2f IntelligencePositions::GetBoundingBox() const
{
    const geometry::Point2f center = GetPosition();
    return geometry::Rectangle2f( center.X() - 250, center.Y(), center.X() + 250, center.Y() + 400 );
}

// -----------------------------------------------------------------------------
// Name: IntelligencePositions::Set
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void IntelligencePositions::Set( const geometry::Point2f& point )
{
    position_ = point;
}

// -----------------------------------------------------------------------------
// Name: IntelligencePositions::Draw
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void IntelligencePositions::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
        tools.DrawCross( where, GL_CROSSSIZE );
}

// -----------------------------------------------------------------------------
// Name: IntelligencePositions::SerializeAttributes
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
void IntelligencePositions::SerializeAttributes( xml::xostream& xos ) const
{
    xos << attribute( "position", converter_.ConvertToMgrs( position_ ) );
}
