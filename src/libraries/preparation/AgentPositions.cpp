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
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "xeumeuleu/xml.h"

using namespace geometry;
using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentPositions constructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentPositions::AgentPositions( const Agent_ABC& agent, const CoordinateConverter_ABC& converter, const Point2f& position )
    : agent_( agent )
    , converter_( converter )
    , aggregated_( false )
    , position_( position )
    , height_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentPositions constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
AgentPositions::AgentPositions( xml::xistream& xis, const kernel::Agent_ABC& agent, const kernel::CoordinateConverter_ABC& converter )
    : agent_( agent )
    , converter_( converter )
    , aggregated_( false )
    , height_( 0 )
{
    std::string position;
    xis >> attribute( "position", position );
    position_ = converter_.ConvertToXY( position );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions destructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentPositions::~AgentPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetPosition
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
Point2f AgentPositions::GetPosition() const
{
    if( ! aggregated_ )
        return position_;
    return agent_.Get< CommunicationHierarchies >().GetUp().Get< Positions >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetHeight
// Created: AGE 2006-04-18
// -----------------------------------------------------------------------------
float AgentPositions::GetHeight() const
{
    if( ! aggregated_ )
        return height_;
    return agent_.Get< CommunicationHierarchies >().GetUp().Get< Positions >().GetHeight();
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::IsAt
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool AgentPositions::IsAt( const Point2f& pos, float precision /*= 100.f*/ ) const
{
    const float halfSizeX = 500.f * 0.5f * ( aggregated_ ? 2.f : 1.f ); // $$$$ SBO 2006-03-21: use font size?
    const float sizeY     = 400.f * ( aggregated_ ? 2.f : 1.f );
    const Point2f position = GetPosition();
    const Rectangle2f agentBBox( position.X() - halfSizeX - precision, position.Y() - precision,
                                 position.X() + halfSizeX + precision, position.Y() + sizeY + precision);
    return agentBBox.IsInside( pos );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::IsIn
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool AgentPositions::IsIn( const Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f AgentPositions::GetBoundingBox() const
{
    const geometry::Point2f center = GetPosition();
    return geometry::Rectangle2f( center.X() - 250, center.Y(), center.X() + 250, center.Y() + 400 );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void AgentPositions::Draw( const Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsInside( where ) )
        tools.DrawCross( where, GL_CROSSSIZE );    
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Aggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void AgentPositions::Aggregate( const bool& bDummy )
{
    aggregated_ = bDummy;
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::SerializeAttributes
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void AgentPositions::SerializeAttributes( xml::xostream& xos ) const
{
    xos << attribute( "position", converter_.ConvertToMgrs( position_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Set
// Created: SBO 2006-09-29
// -----------------------------------------------------------------------------
void AgentPositions::Set( const geometry::Point2f& point )
{
    position_ = point;
}
