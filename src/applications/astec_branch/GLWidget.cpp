// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "GlWidget.h"
#include "TerrainLayer.h"
#include "AgentsLayer.h"
#include "ObjectsLayer.h"
#include "ColorStrategy.h"
#include "MetricsLayer.h"
#include "GlFont.h"
#include "DefaultLayer.h"
#include "PopulationsLayer.h"
#include "LimitsLayer.h"
#include "ParametersLayer.h"
#include "Model.h"
#include "graphics/ElevationLayer.h"
#include "graphics/ElevationMap.h"

using namespace geometry;

namespace
{
    struct SpyLayer : public MapLayer_ABC
    {
        SpyLayer( Rectangle2f& viewport, unsigned int& frame )
            : viewport_( &viewport )
            , frame_( &frame )
        {};
        virtual void Paint( const Rectangle2f& viewport )
        {
            * viewport_ = viewport;
            ++ *frame_;
        }
        Rectangle2f* viewport_;
        unsigned int* frame_;
    };
};

// -----------------------------------------------------------------------------
// Name: GlWidget::GlWidget
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
GlWidget::GlWidget( QWidget* pParent, const std::string& scipioXml, Controllers& controllers, Model& model )
    : WorldParameters( scipioXml )
    , MapWidget( pParent, width_, height_ )
    , strategy_( *new ColorStrategy( controllers ) )
    , elevation_( new ElevationMap( detection_ ) )
    , windowHeight_( 0 )
    , windowWidth_ ( 0 )
    , frame_( 0 )
    , circle_( 0 )
    , app6Font_( 0 )
    , app6OutlinedFont_( 0 )
{
    SetReverse( true );
    SetExclusive( true );
    SetLastFocusFirst( true );

    ParametersLayer* parameters = new ParametersLayer( *this );

    Register( *new SpyLayer( viewport_, frame_ ) );
    Register( *new ElevationLayer( *elevation_ ) );
    Register( *new TerrainLayer( graphicsDirectory_ ) );
    Register( *new MetricsLayer( controllers, *this ) );
    Register( *new LimitsLayer( controllers, *this, strategy_, *parameters, model.limits_ ) );
    Register( *new ObjectsLayer( controllers, *this, strategy_, *this ) );
    Register( *new PopulationsLayer( controllers, *this, strategy_, *this ) );
    Register( *new AgentsLayer( controllers, *this, strategy_, *this ) );
    Register( *parameters );
    
    SetDefaultLayer( *new DefaultLayer( controllers ) );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::~GlWidget
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
GlWidget::~GlWidget()
{
    delete app6Font_;
    delete app6OutlinedFont_;
    delete &strategy_;
    glDeleteLists( circle_, 1 );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::initializeGL
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::initializeGL()
{
    MapWidget::initializeGL();
    circle_ = GenerateCircle();
    app6Font_         = new GlFont( "Scipio" );
    app6OutlinedFont_ = new GlFont( "Scipio", true );
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
// Name: GlWidget::GenerateCircle
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
unsigned int GlWidget::GenerateCircle()
{
    static const float twoPi = 2.f * std::acos( -1.f );
    unsigned int id = glGenLists(1);
    glNewList( id, GL_COMPILE);
        for( float angle = 0; angle < twoPi; angle += twoPi / 40.f + 1e-7 )
            glVertex2f( std::cos( angle ), std::sin( angle ) );
        glVertex2f( 1.f, 0.f );
    glEndList();
    return id;
}

// -----------------------------------------------------------------------------
// Name: GlWidget::Pixels
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
float GlWidget::Pixels() const
{
    if( windowWidth_ > 0 )
        return viewport_.Width() / windowWidth_;
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: GlWidget::StipplePattern
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
unsigned short GlWidget::StipplePattern() const
{
    static unsigned short pattern[] = {
        0x00FF, 0x01FE, 0x03FC, 0x07F8,
        0x0FF0, 0x1FE0, 0x3FC0, 0x7F80,
        0xFF00, 0xFE01, 0xFC03, 0xF807,
        0xF00F, 0xE01F, 0xC03F, 0x807F
    };
    return pattern[ frame_%16 ]; 
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawCross
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawCross( const Point2f& at, float size /*= -1.f*/ ) const
{
    if( size < 0 )
        size = 10.f * Pixels();
    glBegin( GL_LINES );
        glVertex2f(  at.X() - size, at.Y() - size );
        glVertex2f(  at.X() + size, at.Y() + size );
        glVertex2f(  at.X() + size, at.Y() - size );
        glVertex2f(  at.X() - size, at.Y() + size );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawLine
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawLine( const Point2f& from, const Point2f& to ) const
{
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
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 2, GL_FLOAT, 0, (const void*)(&points.front()) );
    glDrawArrays( GL_LINE_STRIP, 0, points.size() );    
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawArrow
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawArrow( const Point2f& from, const Point2f& to, float size /*= -1.f*/ ) const
{
    if( size < 0 )
        size = 15.f * Pixels();

    const Vector2f u = Vector2f( from, to ).Normalize() * size;
    const Vector2f v = 0.5f * u.Normal();
    const Point2f left  = to - u + v;
    const Point2f right = to - u - v;
    
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
// Name: GlWidget::DrawCurvedArrow
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawCurvedArrow( const Point2f& from, const Point2f& to, float curveRatio /*= 0.2f*/, float size /*= -1.f*/ ) const
{
    if( curveRatio == 0 )
    {
        DrawArrow( from, to, size );
        return;
    }
    // $$$$ AGE 2006-03-23: revoir un peu...

    const Vector2f u( from, to );
    const Vector2f v( u.Normal() );
    const Point2f middle = from + 0.5f * u;
    const Point2f center = middle + v * ( 1.f / curveRatio - 1.f );
    const float radius = center.Distance( from );
    if( radius == 0.f )
        return;

    Vector2f v1( center, from ); v1.Normalize();
    float minAngle = std::acos( v1.X() ) * ( v1.Y() > 0 ? 1.f : -1.f );
    Vector2f v2( center, to ); v2.Normalize();
    float maxAngle = std::acos( v2.X() ) * ( v2.Y() > 0 ? 1.f : -1.f );
    if( minAngle > maxAngle )
    {
        static const float twoPi = 2.0 * std::acos( -1.0 );
        maxAngle = maxAngle + twoPi;
    }

    const float deltaAngle = ( maxAngle - minAngle ) / 24.f + 1e-6;
    glMatrixMode(GL_MODELVIEW);	
    glPushMatrix();
        glTranslatef( center.X(), center.Y(), 0.f );
        glScalef    ( radius, radius, 1.f );
        float x, y, lastX, lastY;
        glBegin( GL_LINE_STRIP );
            for( float angle = minAngle; angle < maxAngle; angle += deltaAngle )
            {
                x = std::cos( angle ); y = std::sin( angle );
                glVertex2f( x, y );
            }
            lastX = std::cos( maxAngle ); lastY = std::sin( maxAngle );
            glVertex2f( lastX, lastY );
        glEnd();
        DrawArrow( Point2f( x, y ), Point2f( lastX, lastY ), 10.f * Pixels() / radius );
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawCircle
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawCircle( const Point2f& center, float radius /*= -1.f*/ ) const
{
    if( radius < 0 )
        radius = 10.f * Pixels();

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
void GlWidget::DrawDisc( const Point2f& center, float radius /*= -1.f*/ ) const
{
    if( radius < 0 )
        radius = 10.f * Pixels();

    glMatrixMode(GL_MODELVIEW);	
    glPushMatrix();
        glTranslatef( center.X(), center.Y(), 0.f );
        glScalef    ( radius, radius, 1.f );
        glBegin( GL_TRIANGLE_FAN );
            glVertex2f( 0.f, 0.f );
            glCallList( circle_ );
        glEnd();
        glBegin( GL_LINE_STRIP );
            glCallList( circle_ );
        glEnd();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawRectangle
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void GlWidget::DrawRectangle( const Rectangle2f& rect ) const
{
    glBegin( GL_QUADS );
        glVertex2f( rect.Left(), rect.Bottom() );
        glVertex2f( rect.Left(), rect.Top() );
        glVertex2f( rect.Right(), rect.Top() );
        glVertex2f( rect.Right(), rect.Bottom() );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::Print
// Created: AGE 2006-03-20
// -----------------------------------------------------------------------------
void GlWidget::Print( const std::string& message, const Point2f& where ) const
{
    QGLWidget* that = const_cast< GlWidget* >( this );
    that->renderText( where.X(), where.Y(), 0, message.c_str() );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::DrawApp6Symbol
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void GlWidget::DrawApp6Symbol( const std::string& symbol, const Point2f& where ) const
{
    const Vector2f& fontSize = app6Font_->GetTextSize( symbol );
    const float size = 600.f;
    const Point2f center = Point2f( where.X() - fontSize.X() * size * 0.5f, where.Y() );

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );

    float shadowedColor[4];
    glGetFloatv( GL_CURRENT_COLOR, shadowedColor );
    for(unsigned i = 0;i < 3; ++i )
        shadowedColor[i]/=5.f;
    shadowedColor[3] = 0.9f;
    glColor4fv( shadowedColor );
    glLineWidth( 4.0f );
    app6OutlinedFont_->Print( center, symbol, size );
    glPopAttrib();

    app6Font_->Print( center, symbol, size );
}
