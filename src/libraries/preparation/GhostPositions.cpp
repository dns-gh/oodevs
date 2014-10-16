// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "GhostPositions.h"
#include "MoveableProxy.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_gui/DictionaryUpdated.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace geometry;
using namespace kernel;

namespace
{
    geometry::Point2f ReadPosition( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
    {
        const std::string position = xis.attribute< std::string >( "position" );
        return converter.ConvertToXY( position );
    }
}

// -----------------------------------------------------------------------------
// Name: GhostPositions constructor
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
GhostPositions::GhostPositions( const kernel::Ghost_ABC& ghost, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller,
                                const geometry::Point2f& position, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : ghost_     ( ghost )
    , converter_ ( converter )
    , controller_( controller )
    , entity_    ( entity )
    , moveable_  ( new MoveableProxy( *this ) ) // $$$$ _RC_ PHC 2010-06-25: code smell
    , position_  ( position )
    , height_    ( 0 )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions constructor
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
GhostPositions::GhostPositions( xml::xistream& xis, const kernel::Ghost_ABC& ghost, const kernel::CoordinateConverter_ABC& converter,
                                kernel::Controller& controller, gui::PropertiesDictionary& dico, const kernel::Entity_ABC& entity )
    : ghost_     ( ghost )
    , converter_ ( converter )
    , controller_( controller )
    , entity_    ( entity )
    , moveable_  ( new MoveableProxy( *this ) ) // $$$$ _RC_ PHC 2010-06-25: code smell
    , position_  ( ReadPosition( xis, converter_ ) )
    , height_    ( 0 )
{
    CreateDictionary( dico, entity );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions destructor
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
GhostPositions::~GhostPositions()
{
    delete moveable_;
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::GetPosition
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
geometry::Point2f GhostPositions::GetPosition( bool aggregated ) const
{
    if( !aggregated || !entity_.IsAggregated() )
        return position_;
    const kernel::Entity_ABC* superior = ghost_.Get< TacticalHierarchies >().GetSuperior();
    return superior->Get< Positions >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::GetHeight
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
float GhostPositions::GetHeight( bool aggregated ) const
{
    if( !aggregated || !entity_.IsAggregated() )
        return height_;
    const kernel::Entity_ABC* superior = ghost_.Get< TacticalHierarchies >().GetSuperior();
    return superior->Get< Positions >().GetHeight();
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::IsIn
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostPositions::IsIn( const geometry::Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::GetBoundingBox
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
geometry::Rectangle2f GhostPositions::GetBoundingBox() const
{
    const geometry::Point2f center = GetPosition( true );
    return geometry::Rectangle2f( center.X() - 250, center.Y(), center.X() + 250, center.Y() + 400 );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::Accept
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::Draw
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostPositions::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
        tools.DrawCross( where, GL_CROSSSIZE, gui::GLView_ABC::pixels );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::SerializeAttributes
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostPositions::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "position", converter_.ConvertToMgrs( position_ ) );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::CanAggregate
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
bool GhostPositions::CanAggregate() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::Move
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostPositions::Move( const geometry::Point2f& position )
{
    position_ = position;
    controller_.Update( gui::DictionaryUpdated( ghost_, tools::translate( "GhostPositions", "Info" ) ) );
}

// -----------------------------------------------------------------------------
// Name: GhostPositions::CreateDictionary
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void GhostPositions::CreateDictionary( gui::PropertiesDictionary& dico, const kernel::Entity_ABC& entity )
{
    dico.RegisterExtension( entity, this, tools::translate( "GhostPositions", "Info/Position" ), moveable_ );
}
