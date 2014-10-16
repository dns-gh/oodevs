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
#include "DrawingTypes.h"
#include "SimpleLocationDrawer.h"
#include "SvgLocationDrawer.h"
#include "GLView_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include <boost/assign.hpp>
#include <boost/make_shared.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TacticalGraphics constructor
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
TacticalGraphics::TacticalGraphics( const DrawingTypes& drawingTypes )
{
    drawingTypes.Apply(
        [&]( const DrawingCategory& category )
        {
            auto it = category.CreateIterator();
            while( it.HasMoreElements() )
            {
                const DrawingTemplate& drawing = it.NextElement();
                templates_[ drawing.GetCode().toStdString() ] = &drawing;
            }
        } );
}

// -----------------------------------------------------------------------------
// Name: TacticalGraphics destructor
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
TacticalGraphics::~TacticalGraphics()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalGraphics::SetCurrentColor
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void TacticalGraphics::SetCurrentColor( float r, float g, float b, float a )
{
    color_.setRgbF( r, g, b, a );
}

namespace
{
    // The area of effect of discrete objects(point).
    // By default, for a discrete object, the point size is 250m(defined in the simulation).
    class AreaEffect : public kernel::LocationVisitor_ABC
    {
    public:
        AreaEffect( const GLView_ABC& tools, float pointSize )
            : tools_( tools )
            , pointSize_( pointSize > 0.f ? pointSize : 250.f )
        {}
    private:
        virtual void VisitLines    ( const T_PointVector& ) {}
        virtual void VisitRectangle( const T_PointVector& ) {}
        virtual void VisitPolygon  ( const T_PointVector& ) {};
        virtual void VisitCircle   ( const geometry::Point2f&, float ) {}
        virtual void VisitCurve    ( const T_PointVector& ) {}
        virtual void VisitPath     ( const geometry::Point2f&, const T_PointVector& ) {}
        virtual void VisitText     ( const QString&, const QFont&, const geometry::Point2f& ) {}
        virtual void VisitPoint    ( const geometry::Point2f& point )
        {
            tools_.DrawPolygon( boost::assign::list_of( geometry::Point2f( point.X() - pointSize_, point.Y() - pointSize_ ) )
                ( geometry::Point2f( point.X() + pointSize_, point.Y() - pointSize_ ) )
                ( geometry::Point2f( point.X() + pointSize_, point.Y() + pointSize_ ) )
                ( geometry::Point2f( point.X() - pointSize_, point.Y() + pointSize_ ) )
                ( geometry::Point2f( point.X() - pointSize_, point.Y() - pointSize_ ) ) );
        }
    private:
        const GLView_ABC& tools_;
        const float pointSize_;
    };
}

// -----------------------------------------------------------------------------
// Name: TacticalGraphics::Draw
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void TacticalGraphics::Draw( const std::string& symbol, const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport,
                             const GLView_ABC& tools, bool overlined, float pointSize, float zoom )
{
    if( auto renderer = FindRenderer( symbol ) )
        renderer->Draw( location, viewport, tools, color_, overlined, eSolid, zoom );
    else
    {
        SimpleLocationDrawer drawer( tools );
        glPushAttrib( GL_LINE_BIT );
        if( overlined )
        {
            glPushAttrib( GL_CURRENT_BIT );
            glColor4f( 0, 0, 0, 0.5f * tools.GetCurrentAlpha() );
            glLineWidth( 6.f );
            location.Accept( drawer );
            glPopAttrib();
        }
        glLineWidth( tools.LineWidth( 2.f ) );
        location.Accept( drawer );
        glPopAttrib();
    }
    AreaEffect visitor( tools, pointSize );
    location.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: TacticalGraphics::FindRenderer
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
const boost::shared_ptr< SvgLocationDrawer >& TacticalGraphics::FindRenderer( const std::string& symbol )
{
    boost::shared_ptr< SvgLocationDrawer > renderer = renderers_[ symbol ];
    if( !renderer )
    {
        auto it = templates_.find( symbol );
        if( it != templates_.end() )
            renderer = boost::make_shared< gui::SvgLocationDrawer >( *it->second );
    }
    return renderer;
}
