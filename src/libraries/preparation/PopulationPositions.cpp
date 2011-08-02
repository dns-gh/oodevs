// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "PopulationPositions.h"
#include "Population.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PopulationPositions constructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
PopulationPositions::PopulationPositions( const Population& owner, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const geometry::Point2f& position )
    : converter_   ( converter )
    , owner_       ( owner )
    , controller_  ( controller )
    , center_      ( position )
    , livingHumans_( 0 )
    , radius_      ( 0.f )
{
    UpdatePosition();
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions constructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
PopulationPositions::PopulationPositions( xml::xistream& xis, const Population& owner, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter )
    : converter_   ( converter )
    , owner_       ( owner )
    , controller_  ( controller )
    , center_      ( ReadPosition( xis, converter ) )
    , livingHumans_( 0 )
    , radius_      ( 0.f )
{
    UpdatePosition();
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions destructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
PopulationPositions::~PopulationPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::ReadPosition
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
geometry::Point2f PopulationPositions::ReadPosition( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
{
    std::string mgrs;
    xis >> xml::attribute( "position", mgrs );
    return converter.ConvertToXY( mgrs );
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::UpdatePosition
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void PopulationPositions::UpdatePosition()
{
    static const float oneOnpi = 1.f / std::acos( -1.f );

    if( owner_.GetTotalLivingHumans() != livingHumans_ )
    {
        livingHumans_ = owner_.GetTotalLivingHumans();
        const float density = owner_.GetType().GetDensity();
        if( density > 0 )
            radius_ = std::sqrt( ( livingHumans_ / density ) * oneOnpi );
        const geometry::Vector2f diag( radius_, radius_ );
        boundingBox_ = geometry::Rectangle2f( center_ - diag, center_ + diag );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::GetPosition
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
geometry::Point2f PopulationPositions::GetPosition( bool ) const
{
    return center_;
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::GetHeight
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
float PopulationPositions::GetHeight( bool ) const
{
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::IsAt
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
bool PopulationPositions::IsAt( const geometry::Point2f& pos, float precision /* = 100.f*/, float /*adaptiveFactor = 1.f*/ ) const
{
    return pos.Distance( center_ ) < radius_ + precision;
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::IsIn
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
bool PopulationPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return ! boundingBox_.Intersect( rectangle ).IsEmpty();
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::GetBoundingBox
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
geometry::Rectangle2f PopulationPositions::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void PopulationPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitCircle( center_, radius_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::Draw
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void PopulationPositions::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    const_cast< PopulationPositions* >( this )->UpdatePosition(); // $$$$ SBO 2006-11-09:
    if( viewport.IsVisible( where ) )
        tools.DrawDisc( center_, radius_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::SerializeAttributes
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void PopulationPositions::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "position", converter_.ConvertToMgrs( center_ ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::Move
// Created: LGY 2011-03-18
// -----------------------------------------------------------------------------
void PopulationPositions::Move( const geometry::Point2f& position )
{
    center_ = position;
    controller_.Update( owner_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool PopulationPositions::CanAggregate() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::IsAggregated
// Created: LGY 2011-03-04
// -----------------------------------------------------------------------------
bool PopulationPositions::IsAggregated() const
{
    return false;
}
