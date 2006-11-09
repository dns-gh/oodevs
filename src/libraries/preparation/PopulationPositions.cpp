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
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: PopulationPositions constructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
PopulationPositions::PopulationPositions( const Population& owner, const kernel::CoordinateConverter_ABC& converter, const geometry::Point2f& position )
    : converter_( converter )
    , owner_( owner )
    , center_( position )
    , livingHumans_( 0 )
    , radius_( 0.f )
{
    UpdatePosition();
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions constructor
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
PopulationPositions::PopulationPositions( xml::xistream& xis, const Population& owner, const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , owner_( owner )
    , center_( ReadPosition( xis, converter ) )
    , livingHumans_( 0 )
    , radius_( 0.f )
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
    xis >> attribute( "position", mgrs );
    return converter.ConvertToXY( mgrs );
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::UpdatePosition
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void PopulationPositions::UpdatePosition()
{
    static const float oneOnpi = 1.f / std::acos( -1.f );

    if( owner_.GetLivingHumans() != livingHumans_ )
    {
        livingHumans_ = owner_.GetLivingHumans();
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
geometry::Point2f PopulationPositions::GetPosition() const
{
    return center_;
}
    
// -----------------------------------------------------------------------------
// Name: PopulationPositions::GetHeight
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
float PopulationPositions::GetHeight() const
{
    return 0.;
}
    
// -----------------------------------------------------------------------------
// Name: PopulationPositions::IsAt
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
bool PopulationPositions::IsAt( const geometry::Point2f& pos, float precision /*= 100.f*/ ) const
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
// Name: PopulationPositions::Draw
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void PopulationPositions::Draw( const geometry::Point2f&, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const
{
    const_cast< PopulationPositions* >( this )->UpdatePosition(); // $$$$ SBO 2006-11-09: 
    const geometry::Vector2f extension( radius_, radius_ );
    geometry::Rectangle2f extended( viewport.BottomLeft() - extension, viewport.TopRight() + extension );
    if( extended.IsInside( center_ ) )
        tools.DrawDisc( center_, radius_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationPositions::SerializeAttributes
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void PopulationPositions::SerializeAttributes( xml::xostream& xos ) const
{
    xos << attribute( "position", converter_.ConvertToMgrs( center_ ) );
}
