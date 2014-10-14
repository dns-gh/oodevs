// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Direction.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Direction constructor
// Created: AGE 2007-12-17
// -----------------------------------------------------------------------------
Direction::Direction( const kernel::Entity_ABC& entity )
    : entity_( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Direction destructor
// Created: AGE 2007-12-17
// -----------------------------------------------------------------------------
Direction::~Direction()
{
    // NOTHING
}

namespace
{
    template< typename Message >
    void GetDirection( const Message& message, geometry::Vector2f& direction )
    {
        if( !message.has_direction() )
            return;
        const float heading = message.direction().heading() * 3.14f / 180.f;
        direction = geometry::Vector2f( std::sin( heading ), std::cos( heading ) ).Normalized();
    }
}

// -----------------------------------------------------------------------------
// Name: Direction::DoUpdate
// Created: AGE 2007-12-17
// -----------------------------------------------------------------------------
void Direction::DoUpdate( const sword::UnitAttributes& message )
{
    GetDirection( message, direction_ );
}

// -----------------------------------------------------------------------------
// Name: Direction::DoUpdate
// Created: AGE 2007-12-17
// -----------------------------------------------------------------------------
void Direction::DoUpdate( const sword::UnitKnowledgeUpdate& message )
{
    GetDirection( message, direction_ );
}

namespace
{
    const float fixedSize = 400;
    const float width = 2;
    const float arrowHeightPercent = 0.8f;
    const float arrowheadHeightPercent = 0.35f;
    const float arrowheadWidthPercent = 0.3f;

    void DrawArrowhead( gui::GLView_ABC& tools,
                        const geometry::Point2f& end,
                        const geometry::Point2f& left,
                        const geometry::Point2f& right )
    {
        glBegin( GL_TRIANGLES );
            glVertex2f( end.X(), end.Y() );
            glVertex2f( left.X(), left.Y() );
            glVertex2f( right.X(), right.Y() );
        glEnd();
        tools.DrawLine( end, left );
        tools.DrawLine( end, right );
        tools.DrawLine( left, right );
    }
}

// -----------------------------------------------------------------------------
// Name: Direction::Draw
// Created: AGE 2007-12-17
// -----------------------------------------------------------------------------
void Direction::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( !viewport.IsHotpointVisible() || !tools.ShouldDisplay( "Direction" ) || entity_.IsAggregated() )
        return;
    const float tailSize = fixedSize * tools.GetAdaptiveZoomFactor( false );
    const float arrowSize = tailSize * arrowHeightPercent;
    const geometry::Point2f tail = where + geometry::Vector2f( 0, -1 ) * tailSize;
    const geometry::Vector2f arrowWidth = direction_.Normal().Normalize() * arrowSize * arrowheadWidthPercent / 2;
    const geometry::Point2f arrowCross = tail + direction_ * arrowSize * ( 1 - arrowheadHeightPercent );
    const geometry::Point2f arrowEnd = tail + direction_ * arrowSize;
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glLineWidth( width );
        static const float color[4] = { COLOR_BLACK };
        glColor4f( color[0], color[1], color[2], tools.GetCurrentAlpha() );
        tools.DrawLine( where, tail, width );
        tools.DrawLine( tail, arrowEnd, width );
        DrawArrowhead( tools, arrowEnd, arrowCross + arrowWidth, arrowCross - arrowWidth );
    glPopAttrib();
}
