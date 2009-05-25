// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "IntelligencePositions.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "game_asn/MessengerSenders.h"
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IntelligencePositions constructor
// Created: SBO 2007-10-15
// -----------------------------------------------------------------------------
IntelligencePositions::IntelligencePositions( const CoordinateConverter_ABC& converter, const Intelligence_ABC& holder, Publisher_ABC& publisher )
    : converter_( converter )
    , holder_   ( holder )
    , publisher_( publisher )
    , position_ ()
    , height_   ( 0 )
{
    // NOTHING
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
// Name: IntelligencePositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void IntelligencePositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition() );
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
// Name: IntelligencePositions::DoUpdate
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void IntelligencePositions::DoUpdate( const ASN1T_MsgIntelligenceCreation& message )
{
    position_ = converter_.ConvertToXY( message.intelligence.location );
}

// -----------------------------------------------------------------------------
// Name: IntelligencePositions::DoUpdate
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void IntelligencePositions::DoUpdate( const ASN1T_MsgIntelligenceUpdate& message )
{
    if( message.m.locationPresent )
        position_ = converter_.ConvertToXY( message.location );
}

// -----------------------------------------------------------------------------
// Name: IntelligencePositions::Set
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void IntelligencePositions::Set( const geometry::Point2f& point )
{
    plugins::messenger::IntelligenceUpdateRequest message;
    message().oid = holder_.GetId();
    message().m.locationPresent = 1;
    converter_.ConvertToGeo( point, message().location );
    message.Send( publisher_ );
}
