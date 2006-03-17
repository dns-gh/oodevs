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
#include "ElevationLayer.h"
#include "AgentsLayer.h"
#include "SelectionProxy.h"
#include "Agent.h"
//#include "SelectionLayer.h"

using namespace geometry;

namespace
{
    struct SpyLayer : public MapLayer_ABC
    {
        SpyLayer( Rectangle2f& viewport ) : viewport_( &viewport ) {};
        virtual void Paint( const Rectangle2f& viewport )
        {
            * viewport_ = viewport;
        }
        Rectangle2f* viewport_;
    };
};

// -----------------------------------------------------------------------------
// Name: GlWidget::GlWidget
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
GlWidget::GlWidget( QWidget* pParent, const std::string& scipioXml, Controller& controller, ActionController& actions, const CoordinateConverter& converter )
    : WorldParameters( scipioXml )
    , MapWidget( pParent, width_, height_ )
    , proxy_( *new SelectionProxy() )
    , windowHeight_( 0 )
    , windowWidth_ ( 0 )
    , circle_( 0 )
{
    Register( *new SpyLayer( viewport_ ) );
    Register( *new ElevationLayer( detection_ ) );
    Register( *new TerrainLayer( graphicsDirectory_ ) );
//    Register( *new SelectionLayer< Agent, AgentDrawer >( controller, actions, proxy_, converter ) );
    Register( *new AgentsLayer( controller, actions, proxy_, converter, *this ) );
}

// -----------------------------------------------------------------------------
// Name: GlWidget::~GlWidget
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
GlWidget::~GlWidget()
{
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
        for( float angle = 0; angle < twoPi; angle += 0.3142f )
            glVertex2f( std::cos( angle ), std::sin( angle ) );
        glVertex2f( 0.f, 0.f );
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
// Name: GlWidget::DrawArrow
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawArrow( const Point2f& from, const Point2f& to, float size /*= -1.f*/ ) const
{
    if( size < 0 )
        size = 10.f * Pixels();

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
void GlWidget::DrawCurvedArrow( const Point2f& from, const Point2f& to, float curveRatio /*= 0.3f*/, float size /*= -1.f*/ ) const
{
    if( curveRatio == 0 )
    {
        DrawArrow( from, to, size );
        return;
    }

    const Vector2f u( from, to );
    const Vector2f v( u.Normal() );
    const Point2f middle = from + 0.5f * u;
    const Point2f center = middle + v * curveRatio;
    const float radius = center.Distance( from );
    if( radius == 0.f )
        return;

    Vector2f v1( center, from ); v1.Normalize();
    float minAngle = std::acos( v1.X() ) * ( v1.Y() > 0 ? 1.f : -1.f );
    Vector2f v2( center, to ); v2.Normalize();
    float maxAngle = std::acos( v2.X() ) * ( v2.Y() > 0 ? 1.f : -1.f );
    if( minAngle > maxAngle )
        std::swap( minAngle, maxAngle );

    const float deltaAngle = ( maxAngle - minAngle ) / 24.f + 1e-6;
    glMatrixMode(GL_MODELVIEW);	
    glPushMatrix();
        glTranslatef( center.X(), center.Y(), 0.f );
        glScalef    ( radius, radius, 1.f );
        float x, y, lastX, lastY;
        glBegin( GL_LINES );
            for( float angle = minAngle; angle < maxAngle; angle += deltaAngle )
            {
                x = std::cos( angle ); y = std::sin( angle );
                glVertex2f( x, y );
            }
            lastX = std::cos( maxAngle ); lastY = std::sin( maxAngle );
            glVertex2f( lastX, lastY );
        glEnd();
        DrawArrow( Point2f( x, y ), Point2f( lastX, lastY ) );
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawCircle
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawCircle( const geometry::Point2f& center, float radius /*= -1.f*/ ) const
{
    if( radius < 0 )
        radius = 10.f * Pixels();

    glMatrixMode(GL_MODELVIEW);	
    glPushMatrix();
        glTranslatef( center.X(), center.Y(), 0.f );
        glScalef    ( radius, radius, 1.f );
        glBegin( GL_LINES );
            glCallList( circle_ );
        glEnd();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: GlWidget::DrawDisc
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void GlWidget::DrawDisc( const geometry::Point2f& center, float radius /*= -1.f*/ ) const
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
        glBegin( GL_LINES );
            glCallList( circle_ );
        glEnd();
    glPopMatrix();
}

