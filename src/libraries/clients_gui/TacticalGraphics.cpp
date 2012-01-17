// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TacticalGraphics.h"
#include "DrawingCategory.h"
#include "DrawingTemplate.h"
#include "SvgLocationDrawer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/SimpleLocationDrawer.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TacticalGraphics constructor
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
TacticalGraphics::TacticalGraphics( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalGraphics destructor
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
TacticalGraphics::~TacticalGraphics()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalGraphics::NotifyCreated
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void TacticalGraphics::NotifyCreated( const DrawingCategory& category )
{
    tools::Iterator< const DrawingTemplate& > it( category.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const DrawingTemplate& drawing = it.NextElement();
        templates_[ drawing.GetCode().ascii() ] = &drawing;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalGraphics::NotifyDeleted
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void TacticalGraphics::NotifyDeleted( const DrawingCategory& category )
{
    tools::Iterator< const DrawingTemplate& > it( category.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const DrawingTemplate& drawing = it.NextElement();
        {
            T_Renderers::iterator itD = renderers_.find( drawing.GetCode().ascii() );
            if( itD != renderers_.end() )
                renderers_.erase( itD );
        }
        {
            T_Templates::iterator itD = templates_.find( drawing.GetCode().ascii() );
            if( itD != templates_.end() )
                templates_.erase( itD );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalGraphics::SetCurrentColor
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void TacticalGraphics::SetCurrentColor( float r, float g, float b, float a )
{
    color_.setRgb( static_cast< int >( r * 255.f ), static_cast< int >( g * 255.f ), int( b * 255.f ), static_cast< int >( a * 255.f ) );
}

// -----------------------------------------------------------------------------
// Name: TacticalGraphics::Draw
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void TacticalGraphics::Draw( const std::string& symbol, const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools, bool overlined )
{
    if( boost::shared_ptr< SvgLocationDrawer > renderer = FindRenderer( symbol ) )
        renderer->Draw( location, viewport, tools, color_, overlined );
    else
    {
        kernel::SimpleLocationDrawer drawer( tools );
        glPushAttrib( GL_LINE_BIT );
            if( overlined )
            {
                glPushAttrib( GL_CURRENT_BIT );
                    glColor4f( 0, 0, 0, 0.5f );
                    glLineWidth( 6.f );
                    location.Accept( drawer );
                glPopAttrib();
            }
            glLineWidth( 2.f );
            location.Accept( drawer );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalGraphics::FindRenderer
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
boost::shared_ptr< SvgLocationDrawer > TacticalGraphics::FindRenderer( const std::string symbol )
{
    boost::shared_ptr< SvgLocationDrawer > renderer = renderers_[ symbol ];
    if( ! renderer.get() )
    {
        T_Templates::const_iterator it = templates_.find( symbol );
        if( it != templates_.end() )
            renderer.reset( new gui::SvgLocationDrawer( *it->second ) );
    }
    return renderer;
}
