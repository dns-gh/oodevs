// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GridLayer.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: GridLayer constructor
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
GridLayer::GridLayer( Controllers& controllers, GlTools_ABC& tools )
    : controllers_( controllers )
    , tools_   ( tools )
    , gridSize_( 10000 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GridLayer destructor
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
GridLayer::~GridLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GridLayer::Initialize
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void GridLayer::Initialize( const geometry::Rectangle2f& extent )
{
    extent_ = extent;
}

// -----------------------------------------------------------------------------
// Name: GridLayer::Paint
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void GridLayer::Paint( const geometry::Rectangle2f& v )
{
    if( !ShouldDrawPass() )
        return;
    if( gridSize_ > 0 )
    {
        geometry::Rectangle2f viewport = v.Intersect( extent_ );
        glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
            float gridSize = gridSize_;
            if( gridSize < 2 * tools_.Pixels() )
                gridSize = 2 * tools_.Pixels();
            glColor4f( 1.0f, 1.0f, 1.0f, 0.3f );
            glLineWidth( 1.0 );
                for( float x = Displace( viewport.Left() ); x < viewport.Right(); x += gridSize )
                    tools_.DrawLine( geometry::Point2f( x, viewport.Top() ),
                                        geometry::Point2f( x, viewport.Bottom() ) );
                for( float y = Displace( viewport.Bottom() ); y < viewport.Top(); y += gridSize )
                    tools_.DrawLine( geometry::Point2f( viewport.Left(),  y  ),
                                        geometry::Point2f( viewport.Right(), y ) );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: GridLayer::Paint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void GridLayer::Paint( kernel::Viewport_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GridLayer::Displace
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
float GridLayer::Displace( float value )
{
    if( gridSize_ <= 0 )
        return value;
    int clipped = int( std::floor( value / gridSize_ ) );
    return clipped * gridSize_;
}

// -----------------------------------------------------------------------------
// Name: GridLayer::OptionChanged
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void GridLayer::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "GridSize" )
        gridSize_ = value.To< float >() * 1000.f;
}
