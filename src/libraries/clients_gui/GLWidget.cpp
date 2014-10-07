// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GlWidget.h"
#include "GlRenderPass_ABC.h"
#include "IconLayout.h"
#include "PickingSelector.h"
#include "clients_kernel/OptionVariant.h"
#include <graphics/Scale.h>
#include <graphics/extensions.h>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>
#include <iterator>
#include <ctime>
#include <GL/glu.h>

using namespace geometry;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: GlWidget::passLess::operator()
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
bool GlWidget::passLess::operator()( GlRenderPass_ABC* lhs, GlRenderPass_ABC* rhs ) const
{
    return names_.find( lhs->GetName() ) < names_.find( rhs->GetName() );
}

namespace
{
    void CALLBACK vertexCallback( GLvoid* vertex )
    {
        glVertex3dv( (GLdouble*) vertex );
    }
    void CALLBACK beginCallback( GLenum which )
    {
        glBegin( which );
    }
    void CALLBACK endCallback()
    {
        glEnd();
    }
    float defaultSymbolSize = 3.f;
}

// -----------------------------------------------------------------------------
// Name: GlWidget::GlWidget
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
GlWidget::GlWidget( QWidget* pParent, Controllers& controllers, float width, float height, IconLayout& iconLayout )
    : SetGlOptions()
    , MapWidget( context_, pParent, width, height, 0 )
    , GlToolsBase( controllers )
    , baseWidth_( 600.f )
    , windowHeight_( 0 )
    , windowWidth_ ( 0 )
    , circle_      ( 0 )
    , halfCircle_  ( 0 )
    , viewport_    ( 0, 0, width, height )
    , frame_       ( 0 )
    , iconLayout_  ( iconLayout )
    , passes_      ( passLess( "" ) )
    , currentPass_ ()
    , bMulti_      ( false )
    , SymbolSize_  ( defaultSymbolSize )
    , tesselator_  ( 0 )
    , pPickingSelector_( new PickingSelector() )
    , selectionBuffer_( 1024 )
{
    setAcceptDrops( true );
    if( context() != context_ || ! context_->isValid() )
        throw MASA_EXCEPTION( "Unable to create context" );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::~GlWidget
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
GlWidget::~GlWidget()
{
    if( tesselator_ )
        gluDeleteTess( tesselator_ );
    glDeleteLists( halfCircle_, 1 );
    glDeleteLists( circle_, 1 );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::initializeGL
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::initializeGL()
{
    glEnable( GL_TEXTURE_2D );
    MapWidget::initializeGL();
    //glDisable( GL_DEPTH_TEST );
    //glShadeModel( GL_FLAT );
    circle_ = GenerateCircle();
    halfCircle_ = GenerateHalfCircle();
    glEnableClientState( GL_VERTEX_ARRAY );
    gl::Initialize();
    glShadeModel( GL_SMOOTH );
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    bMulti_ = gl::HasMultiTexturing();
    tesselator_ = gluNewTess();
    gluTessCallback( tesselator_, GLU_TESS_VERTEX, (void (__stdcall *)(void))vertexCallback );
    gluTessCallback( tesselator_, GLU_TESS_BEGIN,  (void (__stdcall *)(void))beginCallback );
    gluTessCallback( tesselator_, GLU_TESS_END,    (void (__stdcall *)(void))endCallback );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::resizeGL
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::resizeGL( int w, int h )
{
    windowHeight_ = h;
    windowWidth_ = w;
    MapWidget::resizeGL( w, h );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::updateGL
// Created: AGE 2006-12-13
// -----------------------------------------------------------------------------
void GlWidget::updateGL()
{
    if( isVisible() )
        MapWidget::updateGL();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::paintGL
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
void GlWidget::paintGL()
{
    if( bMulti_ ) // $$$$ LGY 2012-03-05: disable painGL : crash in remote desktop
    {
        setAutoUpdate( false );
        for( T_RenderPasses::iterator it = passes_.begin(); it != passes_.end(); ++it )
            RenderPass( **it );
        Scale().Draw( 20, 20, *this );
        setAutoUpdate( true );
    }
}

// -----------------------------------------------------------------------------
// Name: GLWidget::PickGL
// Created: LGY 2013-02-18
// -----------------------------------------------------------------------------
void GlWidget::PickGL()
{
    if( bMulti_ ) // $$$$ LGY 2012-03-05: disable painGL : crash in remote desktop
    {
        setAutoUpdate( false );
        for( T_RenderPasses::iterator it = passes_.begin(); it != passes_.end(); ++it )
            if( (*it)->GetName() == "main" )
                PickingPass( **it );
        setAutoUpdate( true );
    }
}

// -----------------------------------------------------------------------------
// Name: GlWidget::PaintLayers
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void GlWidget::PaintLayers()
{
    if( bMulti_ )  // $$$$ LGY 2012-03-05: disable PaintLayers : crash in remote desktop
        MapWidget::PaintLayers();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::Width
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
unsigned int GlWidget::Width() const
{
    return width();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::Height
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
unsigned int GlWidget::Height() const
{
    return height();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::Viewport
// Created: AGE 2008-05-14
// -----------------------------------------------------------------------------
geometry::Rectangle2f GlWidget::Viewport() const
{
    return GetViewport();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::SetPassOrder
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void GlWidget::SetPassOrder( const std::string& names )
{
    passLess comparator( names );
    T_RenderPasses passes( comparator );
    std::copy( passes_.begin(), passes_.end(), std::inserter( passes, passes.begin() ) );
    std::swap( passes_, passes );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::AddPass
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
void GlWidget::AddPass( GlRenderPass_ABC& pass )
{
    passes_.insert( &pass );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::RemovePass
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void GlWidget::RemovePass( GlRenderPass_ABC& pass )
{
    passes_.erase( &pass );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::RenderPass
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
void GlWidget::RenderPass( GlRenderPass_ABC& pass )
{
    ++frame_;
    currentPass_ = pass.GetName();
    const geometry::Rectangle2f viewport = GetViewport();
    const int windowHeight = windowHeight_;
    const int windowWidth  = windowWidth_;
    viewport_ = pass.Viewport();
    SetViewport( viewport_ );
    windowWidth_  = pass.Width();
    windowHeight_ = pass.Height();

    MapWidget::resizeGL( windowWidth_, windowHeight_ );
    pass.Render( *this );

    SetViewport( viewport );
    viewport_     = viewport;
    windowHeight_ = windowHeight;
    windowWidth_  = windowWidth;
    MapWidget::resizeGL( windowWidth_, windowHeight_ );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::PickingPass
// Created: LGY 2013-02-18
// -----------------------------------------------------------------------------
void GlWidget::PickingPass( GlRenderPass_ABC& pass )
{
    currentPass_ = pass.GetName();
    pass.Render( *this );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::Picking
// Created: LGY 2013-03-06
// -----------------------------------------------------------------------------
void GlWidget::Picking()
{
    pPickingSelector_->Picking( point_, windowHeight_ );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::GetCurrentPass
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
std::string GlWidget::GetCurrentPass() const
{
    return currentPass_;
}

namespace
{
    unsigned int GenerateCirclePart( float maxAngle )
    {
        const float step = std::acos( -1.f ) / 20.f + 1e-7f;
        unsigned int id = glGenLists( 1 );
        glNewList( id, GL_COMPILE );
            for( float angle = 0; angle < maxAngle; angle += step )
                glVertex2f( std::cos( angle ), std::sin( angle ) );
            glVertex2f( std::cos( maxAngle ), std::sin( maxAngle ) );
        glEndList();
        return id;
    }
}

// -----------------------------------------------------------------------------
// Name: GlWidget::GenerateCircle
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
unsigned int GlWidget::GenerateCircle()
{
    return GenerateCirclePart( 2.f * std::acos( -1.f ) );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::GenerateHalfCircle
// Created: JSR 2013-09-25
// -----------------------------------------------------------------------------
unsigned int GlWidget::GenerateHalfCircle()
{
    return GenerateCirclePart( std::acos( -1.f ) );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::UpdateStipple
// Created: RPD 2010-01-06
// -----------------------------------------------------------------------------
void GlWidget::UpdateStipple() const
{
    glEnable( GL_LINE_STIPPLE );
    long time = clock();
    unsigned short shift = ( unsigned short ) ( ( long ) ( time / 8 ) % 128 ) / 8;
    glLineStipple( 1, 0x0FFF << shift | 0x0FFF >> ( 16-shift ) );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawTextLabel
// Created: RPD 2010-01-01
// -----------------------------------------------------------------------------
void GlWidget::DrawTextLabel( const std::string& content, const geometry::Point2f& where, int /*baseSize = 12*/ )
{
    HDC screen = GetDC( NULL );
    const int hSize = GetDeviceCaps( screen, HORZSIZE );
    const int hRes = GetDeviceCaps( screen, HORZRES );
    ReleaseDC( NULL, screen );
    const float scale = Pixels() * 1000.f * hRes / hSize;

    if( scale < minVisuScale_ || scale >= maxVisuScale_ )
        return;

    QFontMetrics fm( currentFont_ );
    QRect rc = fm.boundingRect( content.c_str() );

    QPoint point = CoordinatesToClient( where );
    geometry::Point2f leftBottom = RetrieveCoordinates( point.x() - 4, int( point.y() + rc.height() * 0.25f ) );
    geometry::Point2f rightTop = RetrieveCoordinates( point.x() + rc.width() + 4, int( point.y() - rc.height() * 0.75f ) );

    float color[ 4 ];
    color[ 0 ] = 1.f;
    color[ 1 ] = 1.f;
    color[ 2 ] = 0.67f;
    color[ 3 ] = __min( 0.1f, Zoom() * 300 );
    glColor4fv( color );
    glBegin( GL_POLYGON );
        glVertex3f( leftBottom.X(), leftBottom.Y(), 0 );
        glVertex3f( leftBottom.X(), rightTop.Y(), 0 );
        glVertex3f( rightTop.X(), rightTop.Y(), 0 );
        glVertex3f( rightTop.X(), leftBottom.Y(), 0 );
    glEnd();

    // $$$$ JSR 2010-06-14: Changing font attributes does not seem to work with this version of Qt:
    // OpenGL display lists cannot be generated.
    //currentFont_.setPointSize( ( int ) adaptiveSize );
    //currentFont_.setPointSize( 14 );
    //currentFont_.setItalic( true );
    //currentFont_.setStyleStrategy( QFont::PreferAntialias );
    color[ 0 ] = 0.f;
    color[ 1 ] = 0.f;
    color[ 2 ] = 0.f;
    color[ 3 ] = __min( 1, Zoom() * 1000 );
    glColor4fv( color );

    renderText( where.X(), where.Y(), 2, content.c_str(), currentFont_ );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::GetAdaptiveZoomFactor
// Created: RPD 2009-12-04
// -----------------------------------------------------------------------------
float GlWidget::GetAdaptiveZoomFactor( bool bVariableSize /*= true*/ ) const
{
    if( !bVariableSize )
        return SymbolSize_ * Pixels() / 60;

    float zoom = Zoom();
    float pixels = Pixels();
    if( zoom <= .00024f )
        return 1;
    else if( zoom <= .002f )
    {
        return pixels / 15;
    }
    else
        return 0.12f;
}

// -----------------------------------------------------------------------------
// Name: GLWidget::GetActualZoomFactor
// Created: LDC 2013-04-11
// -----------------------------------------------------------------------------
float GlWidget::GetActualZoomFactor() const
{
    return 0.6f; // 1 pixel <=> 1 metre
}

// -----------------------------------------------------------------------------
// Name: GlWidget::Pixels
// Created: AGE 2007-04-05
// -----------------------------------------------------------------------------
float GlWidget::Pixels( const geometry::Point2f& ) const
{
    if( windowWidth_ > 0 )
        return viewport_.Width() / windowWidth_;
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: GLWidget::LineWidth
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
float GlWidget::LineWidth( float base ) const
{
    if( pPickingSelector_->IsPickingMode() )
        return 5.f * Pixels();
    return base;
}

// -----------------------------------------------------------------------------
// Name: GlWidget::Zoom
// Created: RPD 2009-12-14
// -----------------------------------------------------------------------------
float GlWidget::Zoom() const
{
    return rZoom_;
}

// -----------------------------------------------------------------------------
// Name: GlWidget::StipplePattern
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
unsigned short GlWidget::StipplePattern( int factor /* = 1*/ ) const
{
    static unsigned short pattern[] = {
        0x00FF, 0x01FE, 0x03FC, 0x07F8,
        0x0FF0, 0x1FE0, 0x3FC0, 0x7F80,
        0xFF00, 0xFE01, 0xFC03, 0xF807,
        0xF00F, 0xE01F, 0xC03F, 0x807F
    };
    return pattern[ ( factor * frame_ ) % 16 ];
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawCross
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawCross( const Point2f& at, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    if( unit == pixels )
    {
        if( size < 0 )
            size = 100.f;
        size *= GetAdaptiveZoomFactor( false );
    }
    else
    {        
        if( size < 0 )
            size = 10.f * Pixels();
        else
            size *= GetAdaptiveZoomFactor();
    }
    glPushAttrib( GL_LINE_BIT );
    glEnable( GL_LINE_SMOOTH );
    glBegin( GL_LINES );
        glVertex2f( at.X() - size, at.Y() - size );
        glVertex2f( at.X() + size, at.Y() + size );
        glVertex2f( at.X() + size, at.Y() - size );
        glVertex2f( at.X() - size, at.Y() + size );
    glEnd();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawLine
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawLine( const Point2f& from, const Point2f& to, float width ) const
{
    glPushAttrib( GL_LINE_BIT );
    glEnable( GL_LINE_SMOOTH );
    if( width >= 0.f )
        glLineWidth( width );
    glBegin( GL_LINES );
        glVertex2f( from.X(), from.Y() );
        glVertex2f( to.X()  , to.Y() );
    glEnd();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawStippledLine
// Created: JSR 2012-05-14
// -----------------------------------------------------------------------------
void GlWidget::DrawStippledLine( const geometry::Point2f& from, const geometry::Point2f& to ) const
{
    float color[ 4 ] = { 1.f, 1.f, 1.f, 0.75f };
    glColor4fv( color );
    glBegin( GL_LINES );
        glVertex2f( from.X(), from.Y() );
        glVertex2f( to.X()  , to.Y() );
    glEnd();
    color[ 0 ] = 0.f;
    color[ 1 ] = 0.f;
    color[ 2 ] = 0.f;
    glColor4fv( color );
    UpdateStipple();
    glBegin( GL_LINES );
        glVertex2f( from.X(), from.Y() );
        glVertex2f( to.X()  , to.Y() );
    glEnd();
    glDisable( GL_LINE_STIPPLE );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawLines
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void GlWidget::DrawLines( const T_PointVector& points ) const
{
    if( !points.empty() )
    {
        glPushAttrib( GL_LINE_BIT );
        glEnable( GL_LINE_SMOOTH );
        glVertexPointer( 2, GL_FLOAT, 0, (const void*)(&points.front()) );
        glDrawArrays( GL_LINE_STRIP, 0, static_cast< GLsizei >( points.size() ) );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawRectangle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void GlWidget::DrawRectangle( const T_PointVector& points ) const
{
    if( points.size() > 1 )
    {
        glPushAttrib( GL_LINE_BIT );
        glEnable( GL_LINE_SMOOTH );
        glBegin( GL_LINE_LOOP );
            glVertex2f( points.front().X(), points.front().Y() );
            glVertex2f( points.front().X(), points.back().Y() );
            glVertex2f( points.back().X(), points.back().Y() );
            glVertex2f( points.back().X(), points.front().Y() );
        glEnd();
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawPolygon
// Created: AGE 2007-05-23
// -----------------------------------------------------------------------------
void GlWidget::DrawPolygon( const T_PointVector& points ) const
{
    if( points.empty() )
        return;
    glEnable( GL_STENCIL_TEST );
    glVertexPointer( 2, GL_FLOAT, 0, (const void*)(&points.front()) );
    glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
    glStencilFunc( GL_ALWAYS, 0x1, 0x1 );
    glStencilOp( GL_KEEP, GL_INVERT, GL_INVERT );
    glDrawArrays( GL_TRIANGLE_FAN, 0, static_cast< GLsizei >( points.size() ) );
    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glStencilFunc( GL_EQUAL, 0x1, 0x1 );
    glStencilOp( GL_KEEP, GL_INVERT, GL_INVERT );
    glPushAttrib( GL_CURRENT_BIT );
        float color[4];
        glGetFloatv( GL_CURRENT_COLOR, color );
        if( !pPickingSelector_->IsPickingMode() )
            color[3] *= 0.5;
        glColor4fv( color );
        glDrawArrays( GL_TRIANGLE_FAN, 0, static_cast< GLsizei >( points.size() ) );
    glPopAttrib();
    glPushAttrib( GL_LINE_BIT );
    glEnable( GL_LINE_SMOOTH );
    glDrawArrays( GL_LINE_LOOP, 0, static_cast< GLsizei >( points.size() ) );
    glPopAttrib();
    glDisable( GL_STENCIL_TEST );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawSelectedPolygon
// Created: JSR 2012-05-14
// -----------------------------------------------------------------------------
void GlWidget::DrawSelectedPolygon( const T_PointVector& points ) const
{
    if( points.empty() )
        return;
    if( points.size() > 3 )
        UpdateStipple();
    float color[ 4 ];
    glGetFloatv( GL_CURRENT_COLOR, color );
    color[ 0 ] = 1.f - color[ 0 ];
    color[ 1 ] = 1.f - color[ 1 ];
    color[ 2 ] = 1.f - color[ 2 ];
    color[ 3 ] = 0.5f;
    glColor4fv( color );
    for( auto it = points.begin(); it + 1 != points.end(); ++it )
        DrawLine( *it, *(it+1), 1.5 );
    DrawLine( points.back(), points.front(), 1.5 );
    glDisable( GL_LINE_STIPPLE );
}

namespace
{
    void Fill( std::vector< std::vector< GLdouble > >& geometry, const std::vector< geometry::Point2f >& vertices )
    {
        for( std::size_t i = 0; i< vertices.size(); i++ )
        {
            const geometry::Point2f& point = vertices[ i ];
            geometry[ i ] = boost::assign::list_of( point.X() )( point.Y() )( 0. );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawDecoratedPolygon
// Created: RPD 2009-12-15
// -----------------------------------------------------------------------------
void GlWidget::DrawDecoratedPolygon( const geometry::Polygon2f& polygon, const std::string& name, unsigned int fontHeight )
{
    if( !tesselator_ )
        return;

    // TODO renommer en DrawUrbanBlock?
    const T_PointVector& vertices = polygon.Vertices();
    const std::size_t size = vertices.size();

    if( size == 0u )
        return;

    if( size > urbanGeometryBuffer_.size() )
        urbanGeometryBuffer_.resize( size );

    Fill( urbanGeometryBuffer_, vertices );

    gluTessBeginPolygon( tesselator_, NULL );
    gluTessBeginContour( tesselator_ );

    for( size_t i = 0; i< vertices.size(); i++ )
        gluTessVertex( tesselator_, (GLdouble*)&urbanGeometryBuffer_[ i ][ 0 ], (GLdouble*)&urbanGeometryBuffer_[ i ][ 0 ] );

    gluTessEndContour( tesselator_ );
    gluTessEndPolygon( tesselator_ );

    for( auto it = vertices.begin(); it + 1 != vertices.end(); ++it )
        DrawLine( *it, *(it+1), 1. );
    DrawLine( vertices.back(), vertices.front(), 1. );

    if( !name.empty() )
        DrawTextLabel( name, polygon.BoundingBoxCenter(), fontHeight );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawArrow
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawArrow( const Point2f& from, const Point2f& to, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    Point2f end = to;
    float tipFactor = 1.f;
    if( unit == gui::GlTools_ABC::pixels )
    {
        tipFactor = 0.4f;
        size = 900.f * GetAdaptiveZoomFactor( false );
    }
    else if( size < 0.f )
        size = 15.f * Pixels();

    const Vector2f u = Vector2f( from, to ).Normalize() * size;
    const Vector2f v = 0.5f * u.Normal();
    if( unit == pixels )
        end = from + u;
    const Point2f left  = end + ( - u + v ) * tipFactor;
    const Point2f right = end + ( - u - v ) * tipFactor;

    glPushAttrib( GL_LINE_BIT );
    glEnable( GL_LINE_SMOOTH );
    glBegin( GL_LINES );
        glVertex2f( end.X(), end.Y() );
        glVertex2f( left.X(), left.Y() );
        glVertex2f( end.X(), end.Y() );
        glVertex2f( right.X(), right.Y() );
        glVertex2f( end.X(), end.Y() );
        glVertex2f( from.X(), from.Y() );
    glEnd();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawArc
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void GlWidget::DrawArc( const geometry::Point2f& center, const geometry::Point2f& from, const geometry::Point2f& to, float width ) const
{
    const float radius = center.Distance( from );
    if( radius == 0 )
        return;

    Vector2f v1( center, from ); v1.Normalize();
    float minAngle = std::acos( v1.X() ) * ( v1.Y() > 0 ? 1.f : -1.f );
    Vector2f v2( center, to ); v2.Normalize();
    float maxAngle = std::acos( v2.X() ) * ( v2.Y() > 0 ? 1.f : -1.f );
    if( minAngle > maxAngle )
    {
        static const float twoPi = 2.f * std::acos( -1.f );
        maxAngle = maxAngle + twoPi;
    }

    const float deltaAngle = ( maxAngle - minAngle ) / 24.f + 1e-6f;
    glMatrixMode(GL_MODELVIEW);
    glPushAttrib( GL_LINE_BIT );
    glEnable( GL_LINE_SMOOTH );
    glLineWidth( width );
    glPushMatrix();
        glTranslatef( center.X(), center.Y(), 0.f );
        glScalef    ( radius, radius, 1.f );
        glBegin( GL_LINE_STRIP );
            for( float angle = minAngle; angle < maxAngle; angle += deltaAngle )
                glVertex2f( std::cos( angle ), std::sin( angle ) );
            glVertex2f( std::cos( maxAngle ), std::sin( maxAngle ) );
        glEnd();
    glPopMatrix();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawCurvedArrow
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawCurvedArrow( const Point2f& from, const Point2f& to, float curveRatio /* = 0.2f*/, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    if( curveRatio == 0 )
    {
        DrawArrow( from, to, size, unit );
        return;
    }
    const Vector2f u( from, to );
    const Vector2f v( u.Normal() );
    const Point2f middle = from + 0.5f * u;
    const Point2f center = middle + v * ( 1.f / curveRatio - 1.f );

    DrawArc( center, from, to, 3.f );
    Vector2f endSegment = Vector2f( center, to ).Normal();
    endSegment.Normalize();
    DrawArrow( to - endSegment * 10.f * Pixels(), to, 15.f * Pixels() );
}

float GlWidget::Radius( float radius, E_Unit unit ) const
{
    if( radius < 0 )
        return 10.f * Pixels();
    if( unit == pixels )
        return radius * Pixels();
    return radius;
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawCircle
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawCircle( const Point2f& center, float radius /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    radius = Radius( radius, unit );
    glPushAttrib( GL_LINE_BIT );
    glEnable( GL_LINE_SMOOTH );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
        glTranslatef( center.X(), center.Y(), 0.f );
        glScalef    ( radius, radius, 1.f );
        glBegin( GL_LINE_STRIP );
            glCallList( circle_ );
        glEnd();
    glPopMatrix();
    glPopAttrib();
}

void GlWidget::DrawDiscPart( const geometry::Point2f& center, int glList, float angleDegrees, float radius, GlTools_ABC::E_Unit unit ) const
{
    radius = Radius( radius, unit );
    glPushAttrib( GL_LINE_BIT );
    glEnable( GL_LINE_SMOOTH );
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glTranslatef( center.X(), center.Y(), 0.f );
        glScalef( radius, radius, 1.f );
        if( angleDegrees != 0 )
            glRotatef( -angleDegrees, 0, 0, 1 );
        glBegin( GL_TRIANGLE_FAN );
            glVertex2f( 0.f, 0.f );
            glCallList( glList );
        glEnd();
    glPopMatrix();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawDisc
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawDisc( const Point2f& center, float radius /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    DrawDiscPart( center, circle_, 0, radius, unit );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawHalfDisc
// Created: JSR 2013-09-25
// -----------------------------------------------------------------------------
void GlWidget::DrawHalfDisc( const geometry::Point2f& center, float angleDegrees, float radius /*= -1.f*/, E_Unit unit /*= meters*/ ) const
{
    DrawDiscPart( center, halfCircle_, angleDegrees, radius, unit );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawLife
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void GlWidget::DrawLife( const Point2f& where, float h, float factor /* = 1.f*/, bool fixedSize /*= true*/ ) const
{
    if( !GlToolsBase::ShouldDisplay( "UnitDetails" ) )
        return;
    // $$$$ AGE 2006-04-10: hard coded voodoo numbers
    factor *= GetAdaptiveZoomFactor( !fixedSize );
    const float halfWidth   = factor * 600.f * 0.5f * 0.92f;
    const float deltaHeight = factor * 600.f * 0.062f;

    const float y = where.Y() - deltaHeight;
    const float x = where.X();
    const float ydelta = factor * 20.f; // $$$$ SBO 2007-05-04: hard coded again
    const float xdelta = h * halfWidth * 2;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glEnable( GL_LINE_SMOOTH );
        glLineWidth( 1. );
        glColor3f( 0.8f, 0.8f, 0.8f );  // light gray
        glBegin( GL_QUADS );
            glVertex2f( x - halfWidth + xdelta, y - ydelta );
            glVertex2f( x + halfWidth, y - ydelta );
            glVertex2f( x + halfWidth, y + ydelta );
            glVertex2f( x - halfWidth + xdelta, y + ydelta );
        glEnd();
        glColor3f( 1 - h, h*h*h, 0.f ); // gradiation from green (full) to red (dead)
        glBegin( GL_QUADS );
            glVertex2f( x - halfWidth, y - ydelta );
            glVertex2f( x - halfWidth + xdelta, y - ydelta );
            glVertex2f( x - halfWidth + xdelta, y + ydelta );
            glVertex2f( x - halfWidth, y + ydelta );
        glEnd();
        glColor3f( 0.1f, 0.1f, 0.1f );   // almost black
        glBegin( GL_LINE_LOOP );
            glVertex2f( x - halfWidth, y - ydelta );
            glVertex2f( x + halfWidth, y - ydelta );
            glVertex2f( x + halfWidth, y + ydelta );
            glVertex2f( x - halfWidth, y + ydelta );
        glEnd();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::Print
// Created: AGE 2006-03-20
// -----------------------------------------------------------------------------
void GlWidget::Print( const std::string& message, const Point2f& where ) const
{
   Print( message, where, QFont() );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::Print
// Created: AGE 2007-06-20
// -----------------------------------------------------------------------------
void GlWidget::Print( const std::string& message, const geometry::Point2f& where, const QFont& /*font*/ ) const
{
   const_cast< GlWidget*>( this )->renderText( where.X(), where.Y(), 2, message.c_str(), QFont() );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::ComputeZoomFactor
// Created: MMC 2012-02-04
// -----------------------------------------------------------------------------
float GlWidget::ComputeZoomFactor( float& factor, bool bVariableSize /*= true*/ ) const
{
    float adaptiveZoomFactor = factor < 0.f ? GetAdaptiveZoomFactor( bVariableSize ) : 1.f;
    factor = fabs( factor ) * adaptiveZoomFactor;
    return adaptiveZoomFactor;
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawApp6Symbolc
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void GlWidget::DrawApp6Symbol( const std::string& symbol, const Point2f& where, float factor /* = 1.f*/, float thickness /* = 1.f*/, unsigned int direction /*= 0*/ ) const
{
    DrawApp6Symbol( symbol, DefaultStyle(), where, factor, thickness, direction );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::DrawInfrastructureSymbol
// Created: LGY 2013-06-12
// -----------------------------------------------------------------------------
void GlWidget::DrawInfrastructureSymbol( const std::string& symbol, const geometry::Point2f& where, float factor, float thickness ) const
{
    thickness *= ComputeZoomFactor( factor );
    DrawApp6Symbol( symbol, DefaultStyle(), where, baseWidth_ * factor, viewport_
        , static_cast< unsigned int >( windowWidth_ * thickness ), static_cast< unsigned int >( windowHeight_ * thickness ), 0u, 1., 1., -20, -80, false );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawApp6Symbol
// Created: AGE 2008-05-07
// -----------------------------------------------------------------------------
void GlWidget::DrawApp6Symbol( const std::string& symbol, const std::string& style, const geometry::Point2f& where, float factor /* = 1.f*/, float thickness /* = 1.f*/, unsigned int direction /*= 0*/ ) const
{
    thickness *= ComputeZoomFactor( factor );
    DrawApp6Symbol( symbol, style, where, baseWidth_ * factor, viewport_
                  , static_cast< unsigned int >( windowWidth_ * thickness ), static_cast< unsigned int >( windowHeight_ * thickness ), direction, 1., 1., -20, -80 );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawApp6SymbolFixedSize
// Created: MMC 2012-02-04
// -----------------------------------------------------------------------------
void GlWidget::DrawApp6SymbolFixedSize( const std::string& symbol, const geometry::Point2f& where, float factor, unsigned int direction ) const
{
    ComputeZoomFactor( factor, false );
    DrawApp6Symbol( symbol, DefaultStyle(), where, baseWidth_ * factor, Rectangle2f( Point2f( 0.f, 0.f ), Point2f( 256, 256 ) ), 4, 4, direction, 1., 1. );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::DrawApp6SymbolScaledSize
// Created: LDC 2013-04-11
// -----------------------------------------------------------------------------
void GlWidget::DrawApp6SymbolScaledSize( const std::string& symbol, const geometry::Point2f& where, float factor, unsigned int direction, float width, float depth ) const
{
    factor = fabs( factor ) * GetActualZoomFactor() * SymbolSize_/defaultSymbolSize;
    const float svgDeltaX = -20; // Offset of 20 in our svg files...
    const float svgDeltaY = -80 + 120; // Offset of 80 in our svg files + half of 240 which is the defautl height...
    Rectangle2f rectangle( Point2f( 0.f, 0.f ), Point2f( 256, 256 ) );
    DrawApp6Symbol( symbol, DefaultStyle(), where, baseWidth_ * factor, rectangle, 4, 4, direction, width, depth, svgDeltaX, svgDeltaY );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::DrawUnitSymbol
// Created: LDC 2013-04-09
// -----------------------------------------------------------------------------
void GlWidget::DrawUnitSymbol( const std::string& symbol, const std::string& moveSymbol, const std::string& staticSymbol, const std::string& level, bool isMoving, const geometry::Point2f& where, float factor, unsigned int direction, float width, float depth ) const
{
    width = width ? width / 360 : 1;
    float symbolDepth = 240;
    float baseDepth = depth;
    depth = depth ? depth / symbolDepth : 1;
    bool mirror = direction > 180;
    float xFactor = mirror ? -1.f : 1.f;
    if( isMoving )
    {
        if( !moveSymbol.empty() )
        {
            geometry::Vector2f directionVector( 0., 1. );
            float radians = direction * 3.14f/180;
            directionVector.Rotate( - radians );
            geometry::Point2f arrowTail = where + directionVector * (-baseDepth);
            geometry::Point2f arrowHead = where;
            geometry::Point2f symbolTail = arrowHead + directionVector * (-symbolDepth * SymbolSize_/defaultSymbolSize);
            geometry::Vector2f symbolVector( symbolTail, arrowHead );
            geometry::Point2f symbolPosition = symbolTail + symbolVector * 0.5f; 
            DrawApp6SymbolScaledSize( moveSymbol, symbolPosition, factor, direction, xFactor, 1 );
            if( baseDepth && baseDepth > symbolDepth * SymbolSize_/defaultSymbolSize )
            {
                T_PointVector points;
                points.push_back( arrowTail );
                points.push_back( symbolTail );
                DrawTail( points, 3.f );
            }
            DrawApp6SymbolScaledSize( level, symbolPosition, factor, direction, xFactor, 1 );
        }
        else
            DrawApp6SymbolFixedSize( symbol, where, factor, 0 );
    }
    else
    {
        if( !staticSymbol.empty() )
            DrawApp6SymbolScaledSize( staticSymbol, where, factor, direction, width * xFactor, depth );
        else
            DrawApp6SymbolFixedSize( symbol, where, factor, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawUnitSymbolAndTail
// Created: LDC 2013-04-26
// -----------------------------------------------------------------------------
void GlWidget::DrawUnitSymbolAndTail( const std::string& symbol, const std::string& level, const T_PointVector& points ) const
{
    geometry::Point2f penultimatePoint = points.at( points.size() -2 );
    geometry::Point2f lastPoint = points.back();
    float symbolDepth = 240;
    geometry::Vector2f directionVector( penultimatePoint, lastPoint );
    directionVector.Normalize();
    geometry::Vector2f vertical( 0.f, 1.f );
    float radians = geometry::Angle( vertical, directionVector );
    int direction = static_cast< int >( radians * 180 / 3.14f );
    if( direction < 0 )
        direction = 360 + direction;
    bool mirror = direction > 180;
    float xFactor = mirror ? -1.f : 1.f;
    unsigned int udirection = 360 - static_cast< unsigned int >( direction );
    geometry::Point2f symbolTail = lastPoint + directionVector * (-symbolDepth/2);
    DrawApp6SymbolScaledSize( symbol, lastPoint, -1.f, udirection, xFactor, 1 );
    DrawApp6SymbolScaledSize( level, lastPoint, -1.f, udirection, xFactor, 1 );
    T_PointVector arrowPoints( points );
    arrowPoints.pop_back();
    arrowPoints.push_back( symbolTail );
    DrawTail( arrowPoints, 3.f );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawTail
// Created: LDC 2013-04-25
// -----------------------------------------------------------------------------
void GlWidget::DrawTail( const T_PointVector& points, float width ) const
{
    glPushAttrib( GL_LINE_BIT );
    glLineWidth( std::abs( width ) );
    DrawLines( points );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawApp6Symbol
// Created: MMC 2012-02-04
// -----------------------------------------------------------------------------
void GlWidget::DrawApp6Symbol( const std::string& symbol, const std::string& style, const geometry::Point2f& where
                              , float expectedWidth, const geometry::Rectangle2f& viewport, unsigned int printWidth
                              , unsigned int printHeight, unsigned int direction, float xFactor, float yFactor
                              , const float svgDeltaX, const float svgDeltaY, bool checkAlpha ) const
{
    const float svgWidth = 360;
    const float expectedHeight = expectedWidth * 0.660f;
    const Point2f center = Point2f( where.X() /*- expectedWidth * 0.5f*/, where.Y() /*+ expectedHeight*/ );
    const float scaleRatio = ( expectedWidth / svgWidth );

    gl::Initialize();
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glEnable( GL_LINE_SMOOTH );
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
            glTranslatef( center.X(), center.Y(), 0.0f );
            glRotatef( - (GLfloat)direction, 0, 0, 1 );
            glScalef( xFactor, yFactor, 1 );
            glTranslatef( - expectedWidth * 0.5f, expectedHeight, 0.0f );
            glScalef( scaleRatio, -scaleRatio, 1 );
            glTranslatef( svgDeltaX, svgDeltaY, 0.0f );
            Base().PrintApp6( symbol, style, viewport, printWidth, printHeight, checkAlpha );
        glPopMatrix();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawTacticalGraphics
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void GlWidget::DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location,
                                     bool overlined, float pointSize, bool fixedSize /*= true*/ ) const
{
    Base().DrawTacticalGraphics( symbol, location, viewport_, overlined, pointSize, GetAdaptiveZoomFactor( !fixedSize ) );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawIcon
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void GlWidget::DrawIcon( const char** xpm, const Point2f& where, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    if( !GlToolsBase::ShouldDisplay( "UnitDetails" ) )
        return;
    if( size < 0 )
        size = 32 * Pixels();

    float factor = GetAdaptiveZoomFactor( unit != GlTools_ABC::pixels );
    size *= 0.7f * factor;
    glPushMatrix();
    glPushAttrib( GL_TEXTURE_BIT );
        glEnable( GL_TEXTURE_2D );
        glDisable( GL_TEXTURE_GEN_S );
        glDisable( GL_TEXTURE_GEN_T );
        Base().BindIcon( xpm );
        const Point2f iconTranslation = iconLayout_.IconLocation( xpm );
        glTranslatef( where.X()+ iconTranslation.X()*factor, where.Y() + iconTranslation.Y()*factor, 0.f );
        glScalef( size, size, size );
        Base().DrawBillboardRect();
        glPopMatrix();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawImage
// Created: AGE 2006-11-17
// -----------------------------------------------------------------------------
void GlWidget::DrawImage( const QImage& image, const geometry::Point2f& where ) const
{
    if( image.bits() )
    {
        glRasterPos3f( where.X(), where.Y(), 300 );
        glDrawPixels( image.width(), image.height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
    }
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawCell
// Created: AGE 2006-12-01
// -----------------------------------------------------------------------------
void GlWidget::DrawCell( const geometry::Point2f& center ) const
{
    glVertex2fv( (const float*)&center );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawSvg
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void GlWidget::DrawSvg( const std::string& svg, const geometry::Point2f& center, float ratio /* = 1.f*/, bool fixedSize /*= true*/ ) const
{
    glPushMatrix();
    glTranslatef( center.X(), center.Y(), 0 );
    if( ratio != 1 )
        glScalef( ratio, ratio, ratio );

    Base().DrawSvg( svg, fixedSize? Rectangle2f( Point2f( 0.f, 0.f ), Point2f( 5000, 5000 ) ) : viewport_, windowWidth_, windowHeight_ );
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::CenterOn
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void GlWidget::CenterOn( const Point2f& point )
{
    Center( point );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::GetCenter
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
Point2f GlWidget::GetCenter() const
{
    return center_;
}

// -----------------------------------------------------------------------------
// Name: GlWidget::Zoom
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
void GlWidget::Zoom( float w )
{
    if( width() )
        MapWidget::Zoom( w * height() / width() );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::SetCurrentCursor
// Created: SBO 2008-02-12
// -----------------------------------------------------------------------------
void GlWidget::SetCurrentCursor( const QCursor& cursor )
{
    setCursor( cursor );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::MapToterrainCoordinates
// Created: NPT 2013-06-12
// -----------------------------------------------------------------------------
geometry::Point2f GlWidget::MapToterrainCoordinates( int x, int y )
{
    return RetrieveCoordinates( mapFromGlobal( QPoint( x, y ) ).x(), mapFromGlobal( QPoint( x, y ) ).y() );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::HasFocus
// Created: NPT 2013-06-13
// -----------------------------------------------------------------------------
bool GlWidget::HasFocus()
{
    return hasFocus() && QApplication::widgetAt( QCursor::pos() ) == this;
}

// -----------------------------------------------------------------------------
// Name: GlWidget::OptionChanged
// Created: JSR 2010-06-14
// -----------------------------------------------------------------------------
void GlWidget::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "VisuScaleMin13" ) // see VisualisationScalesPanel.cpp
        minVisuScale_ = value.To< int >();
    else if( name == "VisuScaleMax13" ) // see VisualisationScalesPanel.cpp
        maxVisuScale_ = value.To< int >();
    else if( name == "SymbolSize" )
        SymbolSize_ = value.To< float >();
    else
        GlToolsBase::OptionChanged( name, value );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::IsInSelectionViewport
// Created: LGY 2013-03-25
// -----------------------------------------------------------------------------
bool GlWidget::IsInSelectionViewport( const geometry::Point2f& point ) const
{
    // Is in the scene && Is in the terrain
    return rect().contains( mapFromGlobal( QCursor::pos() ) ) && extent_.IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::FillSelection
// Created: LGY 2013-03-11
// -----------------------------------------------------------------------------
void GlWidget::FillSelection( const geometry::Point2f& point, T_ObjectsPicking& selection,
        const boost::optional< E_LayerTypes >& type )
{
    if( !IsInSelectionViewport( point ) )
        return;
    point_ = CoordinatesToClient( point );
    pPickingSelector_->FillSelection( selection, type, boost::bind( &GlWidget::PickGL, this ) );
    // Force a regular rendering to hide the picking one. For some reason, it
    // does not work with updateGL.
    paintGL();
}

// -----------------------------------------------------------------------------
// Name: GLWidget::ShouldDisplay
// Created: LGY 2013-03-11
// -----------------------------------------------------------------------------
bool GlWidget::ShouldDisplay( E_LayerTypes type ) const
{
    return pPickingSelector_->ShouldDisplay( type );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::IsPickingMode
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
bool GlWidget::IsPickingMode() const
{
    return pPickingSelector_->IsPickingMode();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::RenderPicking
// Created: LGY 2013-02-19
// -----------------------------------------------------------------------------
void GlWidget::RenderPicking( const T_ObjectPicking& object )
{
    pPickingSelector_->RenderPicking( object, boost::bind( &GlToolsBase::SetCurrentColor, this, _1, _2, _3, _4 ) );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::GetPickingColor
// Created: LGY 2013-03-15
// -----------------------------------------------------------------------------
QColor GlWidget::GetPickingColor() const
{
    return pPickingSelector_->GetColor();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::wheelEvent
// Created: ABR 2013-02-21
// -----------------------------------------------------------------------------
void GlWidget::wheelEvent( QWheelEvent* event )
{
    MapWidget::wheelEvent( event );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawShapeText
// Created: LGY 2014-06-12
// -----------------------------------------------------------------------------
void GlWidget::DrawShapeText( const QImage& image, const geometry::Point2f& where ) const
{
    if( image.bits() )
    {
        glPushMatrix();
        glPushAttrib( GL_TEXTURE_BIT | GL_CURRENT_BIT );
        glEnable( GL_TEXTURE_2D );
        glDisable( GL_TEXTURE_GEN_S );
        glDisable( GL_TEXTURE_GEN_T );
        glEnable( GL_ALPHA_TEST );
        glAlphaFunc( GL_GREATER, 0.1f );
        glColor3f( 1.f, 1.f, 1.f );

        unsigned int texture;
        glGenTextures( 1, & texture );
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexImage2D( GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        glTranslatef( where.X(), where.Y(), 300.f );
        glScalef( (float)image.width() *  Pixels(), (float)image.height() * Pixels(), 1.f );
        glBegin( GL_QUADS );
        glTexCoord2f( 0.f, 1.f );
        glVertex2f( 0.f, -1.f );
        glTexCoord2f( 0.f, 0.f );
        glVertex2f( 0.f, 0 );
        glTexCoord2f( 1.f, 0.f );
        glVertex2f( 1, 0 );
        glTexCoord2f( 1.f, 1.f );
        glVertex2f( 1, -1.f );
        glEnd();

        glDisable( GL_ALPHA_TEST );
        glPopMatrix();
        glPopAttrib();

        glDeleteTextures( 1, &texture );
    }
}
