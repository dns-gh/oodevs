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
#include "graphics/MapLayer_ABC.h"
#include "graphics/Scale.h"
#include "IconLayout.h"
#include "GlRenderPass_ABC.h"
#include "graphics/extensions.h"
#include <xeumeuleu/xml.h>
#include <qbitmap.h>

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

// -----------------------------------------------------------------------------
// Name: GlWidget::GlWidget
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
GlWidget::GlWidget( QWidget* pParent, Controllers& controllers, const tools::ExerciseConfig& config, IconLayout& iconLayout )
    : WorldParameters( config )
    , SetGlOptions()
    , MapWidget( context_, pParent, width_, height_ )
    , GlToolsBase( controllers )
    , windowHeight_( 0 )
    , windowWidth_ ( 0 )
    , circle_( 0 )
    , viewport_( 0, 0, width_, height_ )
    , frame_( 0 )
    , iconLayout_( iconLayout )
    , passes_( passLess( "" ) )
    , currentPass_()
    , baseFont_( 0 )
{
    setAcceptDrops( true );
    if( context() != context_ || ! context_->isValid() )
        throw std::runtime_error( "Unable to create context" );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::~GlWidget
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
GlWidget::~GlWidget()
{
    glDeleteLists( circle_, 1 );
    for( CIT_Fonts it = fonts_.begin(); it != fonts_.end(); ++it )
        glDeleteLists( it->second, 256 );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::initializeGL
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::initializeGL()
{
    glEnable( GL_TEXTURE_2D );
    MapWidget::initializeGL();
    glDisable( GL_DEPTH_TEST );
    glShadeModel( GL_FLAT );
    circle_ = GenerateCircle();
    glEnableClientState( GL_VERTEX_ARRAY );
    gl::Initialize();
    glShadeModel( GL_SMOOTH );
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
    setAutoUpdate( false );
    for( T_RenderPasses::iterator it = passes_.begin(); it != passes_.end(); ++it )
        RenderPass( **it );
    Scale().Draw( 20, 20, *this );
    setAutoUpdate( true );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::PaintLayers
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void GlWidget::PaintLayers()
{
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
// Name: GlWidget::GetCurrentPass
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
std::string GlWidget::GetCurrentPass() const
{
    return currentPass_;
}

// -----------------------------------------------------------------------------
// Name: GlWidget::GenerateCircle
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
unsigned int GlWidget::GenerateCircle()
{
    const float twoPi = 2.f * std::acos( -1.f );
    unsigned int id = glGenLists(1);
    glNewList( id, GL_COMPILE);
        for( float angle = 0; angle < twoPi; angle += twoPi / 40.f + 1e-7f )
            glVertex2f( std::cos( angle ), std::sin( angle ) );
        glVertex2f( 1.f, 0.f );
    glEndList();
    return id;
}

// -----------------------------------------------------------------------------
// Name: GlWidget::GetZoomFactorAttenuation
// Created: RPD 2009-12-04
// -----------------------------------------------------------------------------
float GlWidget::GetZoomFactorAttenuation() const
{
    float zoom = rZoom_;
    float pixels = Pixels();
    if ( zoom <= .00024f )
        return 1;
    else if ( zoom <= .002f )
    {
        return pixels / 15;
    }
    else
        return 0.12f;
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
// Name: GlWidget::StipplePattern
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
unsigned short GlWidget::StipplePattern( int factor /*= 1*/ ) const
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
void GlWidget::DrawCross( const Point2f& at, float size /*= -1.f*/, E_Unit unit /*= meters*/ ) const
{
    size *= GetZoomFactorAttenuation();
    if( size < 0 )
        size = 10.f * Pixels();
    else if( unit == pixels )
        size *= Pixels();

    glEnable( GL_LINE_SMOOTH );
    glBegin( GL_LINES );
        glVertex2f( at.X() - size, at.Y() - size );
        glVertex2f( at.X() + size, at.Y() + size );
        glVertex2f( at.X() + size, at.Y() - size );
        glVertex2f( at.X() - size, at.Y() + size );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawLine
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawLine( const Point2f& from, const Point2f& to ) const
{
    glEnable( GL_LINE_SMOOTH );
    glBegin( GL_LINES );
        glVertex2f(  from.X(), from.Y() );
        glVertex2f(  to.X()  , to.Y() );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawLines
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void GlWidget::DrawLines( const T_PointVector& points ) const
{
    if( !points.empty() )
    {
        glEnable( GL_LINE_SMOOTH );
        glVertexPointer( 2, GL_FLOAT, 0, (const void*)(&points.front()) );
        glDrawArrays( GL_LINE_STRIP, 0, points.size() );
    }
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawConvexPolygon
// Created: AGE 2007-05-23
// -----------------------------------------------------------------------------
void GlWidget::DrawConvexPolygon( const T_PointVector& points ) const
{
    glEnable( GL_LINE_SMOOTH );
    glVertexPointer( 2, GL_FLOAT, 0, (const void*)(&points.front()) );
    glPushAttrib( GL_CURRENT_BIT );
        float color[4];
        glGetFloatv( GL_CURRENT_COLOR, color );
        color[3]*=0.5f;
        glColor4fv( color );
        glDrawArrays( GL_POLYGON, 0, points.size() );
    glPopAttrib();
    glDrawArrays( GL_LINE_LOOP, 0, points.size() );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawArrow
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawArrow( const Point2f& from, const Point2f& to, float size /*= -1.f*/, E_Unit unit /*= meters*/ ) const
{
    if( size < 0 )
        size = 15.f * Pixels();
    else if( unit == pixels )
        size *= Pixels();

    const Vector2f u = Vector2f( from, to ).Normalize() * size;
    const Vector2f v = 0.5f * u.Normal();
    const Point2f left  = to - u + v;
    const Point2f right = to - u - v;

    glEnable( GL_LINE_SMOOTH );
    glBegin( GL_LINES );
        glVertex2f( to.X(), to.Y() );
        glVertex2f( left.X(), left.Y() );
        glVertex2f( to.X(), to.Y() );
        glVertex2f( right.X(), right.Y() );
        glVertex2f( to.X(), to.Y() );
        glVertex2f( from.X(), from.Y() );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawArc
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void GlWidget::DrawArc( const geometry::Point2f& center, const geometry::Point2f& from, const geometry::Point2f& to ) const
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
    glEnable( GL_LINE_SMOOTH );
    glPushMatrix();
        glTranslatef( center.X(), center.Y(), 0.f );
        glScalef    ( radius, radius, 1.f );
        glBegin( GL_LINE_STRIP );
            for( float angle = minAngle; angle < maxAngle; angle += deltaAngle )
                glVertex2f( std::cos( angle ), std::sin( angle ) );
            glVertex2f( std::cos( maxAngle ), std::sin( maxAngle ) );
        glEnd();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawCurvedArrow
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawCurvedArrow( const Point2f& from, const Point2f& to, float curveRatio /*= 0.2f*/, float size /*= -1.f*/, E_Unit unit /*= meters*/ ) const
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

    DrawArc( center, from, to );
    Vector2f endSegment = Vector2f( center, to ).Normal();
    endSegment.Normalize();
    DrawArrow( to - endSegment * 10.f * Pixels(), to, 10.f * Pixels() );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawCircle
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawCircle( const Point2f& center, float radius /*= -1.f*/, E_Unit unit /*= meters*/ ) const
{
    if( radius < 0 )
        radius = 10.f * Pixels();
    else if( unit == pixels )
        radius *= Pixels();

    glEnable( GL_LINE_SMOOTH );
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glTranslatef( center.X(), center.Y(), 0.f );
        glScalef    ( radius, radius, 1.f );
        glBegin( GL_LINE_STRIP );
            glCallList( circle_ );
        glEnd();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawDisc
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawDisc( const Point2f& center, float radius /*= -1.f*/, E_Unit unit /*= meters*/ ) const
{
    if( radius < 0 )
        radius = 10.f * Pixels();
    else if( unit == pixels )
        radius *= Pixels();

    glEnable( GL_LINE_SMOOTH );
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glTranslatef( center.X(), center.Y(), 0.f );
        glScalef    ( radius, radius, 1.f );
        glBegin( GL_TRIANGLE_FAN );
            glVertex2f( 0.f, 0.f );
            glCallList( circle_ );
        glEnd();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawLife
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void GlWidget::DrawLife( const Point2f& where, float h, float factor /*= 1.f*/ ) const
{
    // $$$$ AGE 2006-04-10: hard coded voodoo numbers
    factor *= GetZoomFactorAttenuation();
    const float halfWidth   = factor * 600.f * 0.5f * 0.92f;
    const float deltaHeight = factor * 600.f * 0.062f;

    const float y = where.Y() - deltaHeight;
    const float x = where.X();
    const float ydelta = factor * 20.f; // $$$$ SBO 2007-05-04: hard coded again
    const float xdelta = h * halfWidth * 2;
    glEnable( GL_LINE_SMOOTH );
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glLineWidth( 1 );
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

namespace
{
    int GenerateList( const QFont& font )
    {
        int result = glGenLists( 256 );
        HDC glHdc = qt_display_dc();
        SelectObject( glHdc, font.handle() );
        if( !wglUseFontBitmaps( glHdc, 0, 256, result ) )
            return 0;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: GlWidget::Print
// Created: AGE 2006-03-20
// -----------------------------------------------------------------------------
void GlWidget::Print( const std::string& message, const Point2f& where ) const
{
    glRasterPos2fv( (const float*)&where );
    if( !baseFont_ )
    {
        const_cast< GlWidget* >( this )->baseFont_ = GenerateList( QFont() );
        if( !baseFont_ ) // GenerateList can fail
            return;
    }
    glListBase( baseFont_ );
    glCallLists( message.length(), GL_UNSIGNED_BYTE, message.c_str() );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::Print
// Created: AGE 2007-06-20
// -----------------------------------------------------------------------------
void GlWidget::Print( const std::string& message, const geometry::Point2f& where, const QFont& font ) const
{
    glRasterPos2fv( (const float*)&where );
    int listBase = 0;
    CIT_Fonts it = fonts_.find( font );
    if( it == fonts_.end() )
    {
        listBase = GenerateList( font );
        if( !listBase ) // GenerateList can fail
            return;
        const_cast< GlWidget* >( this )->fonts_[ font ] = listBase;
    }
    else
        listBase = it->second;
    glListBase( listBase );
    glCallLists( message.length(), GL_UNSIGNED_BYTE, message.c_str() );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawApp6Symbolc
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void GlWidget::DrawApp6Symbol( const std::string& symbol, const Point2f& where, float factor /*= 1.f*/, float thickness /*= 1.f*/ ) const
{
    DrawApp6Symbol( symbol, DefaultStyle(), where, factor, thickness );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawApp6Symbol
// Created: AGE 2008-05-07
// -----------------------------------------------------------------------------
void GlWidget::DrawApp6Symbol( const std::string& symbol, const std::string& style, const geometry::Point2f& where, float factor /*= 1.f*/, float thickness /*= 1.f*/ ) const
{
    const float svgDeltaX = -20;
    const float svgDeltaY = -80;
    const float svgWidth = 360;
    if ( factor < 0 )   //zoom-adaptive view
        factor = - factor * GetZoomFactorAttenuation();
    const float expectedWidth  = 600.f * factor;
    const float expectedHeight = expectedWidth * 0.660f;
    const Point2f center = Point2f( where.X() - expectedWidth * 0.5f, where.Y() + expectedHeight );

    const float scaleRatio = ( expectedWidth / svgWidth );

    gl::Initialize();
    glEnable( GL_LINE_SMOOTH );
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
        glTranslatef( center.X(), center.Y(), 0.0f );
        glScalef( scaleRatio, -scaleRatio, 1 );
        glTranslatef( svgDeltaX, svgDeltaY, 0.0f );
        Base().PrintApp6( symbol, style, viewport_, int( windowWidth_ * thickness ), int( windowHeight_ * thickness ) );
    glPopMatrix();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawTacticalGraphics
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void GlWidget::DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, bool overlined ) const
{
    Base().DrawTacticalGraphics( symbol, location, viewport_, overlined );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawIcon
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void GlWidget::DrawIcon( const char** xpm, const Point2f& where, float size /*= -1.f*/, E_Unit unit /*= meters*/ ) const
{
    if( size < 0 )
        size = 32 * Pixels();
    else if( unit == pixels )
        size *= Pixels();

    size *= 0.7f;
    float factor = GetZoomFactorAttenuation();
    size *= factor;
    glPushMatrix();
    glPushAttrib( GL_TEXTURE_BIT );
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
void GlWidget::DrawSvg( const std::string& svg, const geometry::Point2f& center, float ratio /*= 1.f*/ ) const
{
    glPushMatrix();
    glTranslatef( center.X(), center.Y(), 0 );
    if( ratio != 1 )
        glScalef( ratio, ratio, ratio );
    Base().DrawSvg( svg, viewport_, windowWidth_, windowHeight_ );
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
