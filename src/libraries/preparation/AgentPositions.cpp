// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentPositions.h"
#include "MoveableProxy.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
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

// -----------------------------------------------------------------------------
// Name: AgentPositions constructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentPositions::AgentPositions( Agent_ABC& agent, const CoordinateConverter_ABC& converter, Controller& controller, const Point2f& position, gui::PropertiesDictionary& dico )
    : agent_     ( agent )
    , converter_ ( converter )
    , controller_( controller )
    , moveable_  ( new MoveableProxy( *this ) ) // $$$$ _RC_ PHC 2010-06-25: code smell
    , position_  ( position )
    , height_    ( 0 )
{
    CreateDictionary( dico );
}

namespace
{
    geometry::Point2f ReadPosition( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
    {
        const std::string position = xis.attribute< std::string >( "position" );
        return converter.ConvertToXY( position );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentPositions constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
AgentPositions::AgentPositions( xml::xistream& xis, Agent_ABC& agent, const CoordinateConverter_ABC& converter, Controller& controller, gui::PropertiesDictionary& dico )
    : agent_( agent )
    , converter_( converter )
    , controller_( controller )
    , moveable_( new MoveableProxy( *this ) ) // $$$$ _RC_ PHC 2010-06-25: code smell
    , position_( ReadPosition( xis, converter_ ) )
    , height_( 0 )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions destructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentPositions::~AgentPositions()
{
    delete moveable_;
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetPosition
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
Point2f AgentPositions::GetPosition( bool aggregated ) const
{
    if( !aggregated || !agent_.IsAnAggregatedSubordinate() )
        return position_;
    const kernel::Entity_ABC* superior = agent_.Get< TacticalHierarchies >().GetSuperior();
    return superior->Get< Positions >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetHeight
// Created: AGE 2006-04-18
// -----------------------------------------------------------------------------
float AgentPositions::GetHeight( bool aggregated ) const
{
    if( !aggregated || !agent_.IsAnAggregatedSubordinate() )
        return height_;
    const kernel::Entity_ABC* superior = agent_.Get< TacticalHierarchies >().GetSuperior();
    return superior->Get< Positions >().GetHeight();
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::IsIn
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool AgentPositions::IsIn( const Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f AgentPositions::GetBoundingBox() const
{
    const geometry::Point2f center = GetPosition( true );
    return geometry::Rectangle2f( center.X() - 250, center.Y(), center.X() + 250, center.Y() + 400 );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void AgentPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void AgentPositions::Draw( const Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( viewport.IsVisible( where ) )
        tools.DrawCross( where, GL_CROSSSIZE, gui::GLView_ABC::pixels );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::SerializeAttributes
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void AgentPositions::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "position", converter_.ConvertToMgrs( position_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Move
// Created: SBO 2006-09-29
// -----------------------------------------------------------------------------
void AgentPositions::Move( const geometry::Point2f& position )
{
    position_ = position;
    controller_.Update( gui::DictionaryUpdated( agent_, tools::translate( "AgentPositions", "Info/Position" ) ) );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::CreateDictionary
// Created: SBO 2008-03-25
// -----------------------------------------------------------------------------
void AgentPositions::CreateDictionary( gui::PropertiesDictionary& dico )
{
    dico.Register( agent_, tools::translate( "AgentPositions", "Info/Position" ), moveable_ );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool AgentPositions::CanAggregate() const
{
    return true;
}
