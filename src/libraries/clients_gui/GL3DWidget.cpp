// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Gl3dWidget.h"
#include "ColorStrategy.h"
#include "GlFont.h"
#include "EntityLayer.h"
#include "ColorStrategy.h"
#include "clients_kernel/DetectionMap.h"

#include "graphics/ElevationTextureTree.h"
#include "graphics/Compass.h"
#include "graphics/Visitor3d.h"
#include "graphics/ViewFrustum.h"
#include "graphics/EventStrategy_ABC.h"

using namespace geometry;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: Gl3dWidget constructor
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
Gl3dWidget::Gl3dWidget( QWidget* pParent, Controllers& controllers, const std::string& scipioXml, DetectionMap& elevation, EventStrategy_ABC& strategy )
    : WorldParameters( scipioXml )
    , SetGlOptions()
    , Widget3D( context_, pParent )
    , GlToolsBase( controllers )
    , elevation_( elevation )
    , strategy_( strategy )
    , zRatio_( 5 )
    , frame_( 0 )
    , pixels_( 100.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget destructor
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
Gl3dWidget::~Gl3dWidget()
{
    for( CIT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::Register
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::Register( Layer_ABC& layer )
{
    layers_.push_back( & layer );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::initializeGL
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::initializeGL()
{
    static bool isInitialized = false;
    if( !isInitialized )
    {
        const geometry::Rectangle2f viewport( 0, 0, width_, height_ );
        Widget3D::initializeGL();
        for( CIT_Layers it = layers_.begin(); it != layers_.end(); ++it )
            (*it)->Initialize( viewport );
        isInitialized = true;
        CenterView();
    }
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::Paint
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::Paint( const ViewFrustum& view )
{
    pixels_ = view.ExpectedDetailLevel();
    ++frame_;
    glLineWidth( 1.f );
    glColor3f( 1, 1, 1 );
    glBindTexture( GL_TEXTURE_2D, 0 );
    for( CIT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        (*it)->Paint( view );
}

// $$$$ AGE 2006-04-04: ceci n'est pas un copy/paste

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::Pixels
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
float Gl3dWidget::Pixels() const
{
    return pixels_;
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::StipplePattern
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
unsigned short Gl3dWidget::StipplePattern( int factor /*= 1*/ ) const
{
    static unsigned short pattern[] = {
        0x00FF, 0x01FE, 0x03FC, 0x07F8,
        0x0FF0, 0x1FE0, 0x3FC0, 0x7F80,
        0xFF00, 0xFE01, 0xFC03, 0xF807,
        0xF00F, 0xE01F, 0xC03F, 0x807F
    };
    return pattern[ ( factor * frame_ )%16 ]; 
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawCross
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawCross( const Point2f& at, float size /*= -1.f*/ ) const
{
    if( size < 0 )
        size = 10.f * Pixels();
    const Vector2f u( size, size );
    const Vector2f v( size, -size );
    DrawLine( at + u, at - u );
    DrawLine( at + v, at - v );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::ElevationAt
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
inline
float Gl3dWidget::ElevationAt( const geometry::Point2f& point ) const
{
    return ( elevation_.ElevationAt( point ) + 20.f ) * zRatio_;
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawLine
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawLine( const Point2f& from, const Point2f& to ) const
{
    Vector2f u( from, to );
    const float distance = u.Length();
    if( distance > 0 )
        u /= distance;
    Point2f last = from;
    glBegin( GL_LINE_STRIP );
    glVertex3f( last.X(), last.Y(), ElevationAt( last ) );
    for( float x = 100; x < distance; x += 100.f )
    {
        last = last + 100.f * u;
        glVertex3f( last .X(), last .Y(), ElevationAt( last ) );
    }
    glVertex3f( to.X(), to.Y(), ElevationAt( to ) );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawLines
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawLines( const T_PointVector& points ) const
{
    if( points.size() < 2 )
        return;
    for( CIT_PointVector it = points.begin(); it + 1 != points.end(); ++it )
        DrawLine( *it, *(it+1) );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawArrow
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawArrow( const Point2f& from, const Point2f& to, float size /*= -1.f*/ ) const
{
    if( size < 0 )
        size = 15.f * Pixels();

    const Vector2f u = Vector2f( from, to ).Normalize() * size;
    const Vector2f v = 0.5f * u.Normal();
    const Point2f left  = to - u + v;
    const Point2f right = to - u - v;

    DrawLine( from, to );
    DrawLine( to, left );
    DrawLine( to, right );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawArc
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawArc( const geometry::Point2f& center, const geometry::Point2f& p1, const geometry::Point2f& p2 ) const
{
    // $$$$ AGE 2006-05-17: 
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawCurvedArrow
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawCurvedArrow( const Point2f& from, const Point2f& to, float curveRatio /*= 0.2f*/, float size /*= -1.f*/ ) const
{
    if( curveRatio == 0 )
    {
        DrawArrow( from, to, size );
        return;
    }
    Vector2f u( from, to );
    const Vector2f v( u.Normal() );
    
    const Point2f center = from + 0.5f * u;
    const float distance = u.Length();
    const float zCenter = distance * ( 1.f - 1.f / curveRatio );
    if( distance )
        u /= distance;
    const float radius = Point3f( center.X(), center.Y(), zCenter ).Distance( Point3f( from.X(), from.Y(), ElevationAt( from ) ) );

    Point2f last = from;
    glBegin( GL_LINE_STRIP );
    glVertex3f( last.X(), last.Y(), ElevationAt( last ) );
    for( float x = 100; x < distance; x += 100.f )
    {
        last = last + 100.f * u;
        const float midDistance = x - distance / 2;
        const float elevation = std::sqrt( radius * radius - midDistance * midDistance ) + zCenter ;
        glVertex3f( last .X(), last .Y(), elevation );
    }
    glVertex3f( to.X(), to.Y(), ElevationAt( to ) );
    glEnd();
    // Arrow !
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawCircle
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawCircle( const Point2f& center, float radius /*= -1.f*/ ) const
{
    static const float twoPi = 2.f * std::acos( -1.f );
    if( radius < 0 )
        radius = 10.f * Pixels();

    float px = center.X() + radius, py = center.Y();
    for( float angle = twoPi / 40.f; angle < twoPi; angle += twoPi / 40.f + 1e-7 )
    {
        float cx = center.X() + radius * std::cos( angle );
        float cy = center.Y() + radius * std::sin( angle );
        DrawLine( Point2f( px, py ), Point2f( cx, cy ) );
        px = cx; py = cy;
    }
    DrawLine( Point2f( px, py ), Point2f( center.X() + radius, center.Y() ) );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawDisc
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawDisc( const Point2f& center, float radius /*= -1.f*/ ) const
{
    // $$$$ AGE 2006-03-30: sphere ?
    DrawCircle( center, radius );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawLife
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawLife( const geometry::Point2f& center, float h, float factor /*= 1.f*/ ) const
{
    // $$$$ AGE 2006-09-11: 
    const float halfWidth   = factor * 600.f * 0.5f * 0.92f;
    const float deltaHeight = factor * 600.f * 0.062f;
    glPushMatrix();
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glTranslatef( center.X(), center.Y(), ElevationAt( center ) + 100.f );
        UndoRotations();
        glLineWidth( 3 );
        glColor3f( 1 - h, h, 0.1f ); // $$$$ AGE 2006-09-11: 
        glBegin( GL_LINES );
            glVertex2f(   - halfWidth, deltaHeight );
            glVertex2f( h * halfWidth, deltaHeight );
        glEnd();
    glPopAttrib();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::Print
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::Print( const std::string& message, const Point2f& where ) const
{
    QGLWidget* that = const_cast< Gl3dWidget* >( this );
    that->renderText( where.X(), where.Y(), ElevationAt( where ), message.c_str() );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawApp6Symbol
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawApp6Symbol( const std::string& symbol, const Point2f& where, float factor /*= 1.f*/ ) const
{
    const float size = 600.f * factor;
    const float ratio = size / 1000;
    const float height = size * 0.660f; // $$$$ AGE 2006-09-11: 

    glPushMatrix();
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glTranslatef( where.X(), where.Y(), ElevationAt( where ) + 100.f );
        UndoRotations();
        glTranslatef( - size * 0.5f, height, 0 );
        glScalef( ratio, -ratio, ratio );
        const geometry::Rectangle2f bbox( -10000,-10000,10000,10000 ); // $$$$ AGE 2006-09-11: 
        Base().PrintApp6( symbol, bbox );
    glPopAttrib();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawRectangle
// Created: AGE 2006-09-11
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawRectangle( const geometry::Point2f& where, float factor /*= 1.f*/ ) const
{
    const float size = 600.f * factor;
    glPushMatrix();
        glTranslatef( where.X(), where.Y(), ElevationAt( where ) + 100.f );
        UndoRotations();
        const geometry::Point2f bottomLeft( - size*0.5f, 0 );
        const geometry::Point2f topRight  (   size*0.5f, size*0.66f );
        glRectfv( (const float*)&bottomLeft, (const float*)&topRight );
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawIcon
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawIcon( const char** xpm, const geometry::Point2f& where, float size /*= -1.f*/ ) const
{
    // $$$$ AGE 2006-05-16: 
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::CenterOn
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::CenterOn( const geometry::Point2f& point )
{
    Widget3D::CenterOn( point );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::keyPressEvent
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::keyPressEvent( QKeyEvent* event  )
{
    if( event )
    {
        const float speedFactor = ( event->state() == Qt::ShiftButton ) ? 10.f : 1.f;

        if( event->key() == Qt::Key_Plus )
            zRatio_ *= 1.1f;
        if( event->key() == Qt::Key_Minus )
            zRatio_ *= 0.9f;
        if( event->key() == Qt::Key_Home )
            CenterView();
        if( event->key() == Qt::Key_Left )
            Rotate( Vector3f( 0, 0, 1 ), 0.02f * speedFactor );
        if( event->key() == Qt::Key_Right )
            Rotate( Vector3f( 0, 0, 1 ), -0.02f * speedFactor );
        if( event->key() == Qt::Key_Up )
            Translate( Vector3f( 0, 1, 0 ), 500.f * speedFactor );
        if( event->key() == Qt::Key_Down )
            Translate( Vector3f( 0, 1, 0 ), -500.f * speedFactor );
        if( event->key() == Qt::Key_PageUp )
            Translate( Vector3f( 0, 0, 1 ), 500.f * speedFactor );
        if( event->key() == Qt::Key_PageDown )
            Translate( Vector3f( 0, 0, 1 ), -500.f * speedFactor );
    }
    strategy_.HandleKeyPress( event );
    Widget3D::keyPressEvent( event  );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::mouseMoveEvent
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Gl3dWidget::mouseMoveEvent( QMouseEvent* event )
{
    const geometry::Point3f point3 = PointAt( event->pos() );
    if( point3.Z() > -1000 )
    {
        const geometry::Point2f point( point3.X(), point3.Y() );
        strategy_.HandleMouseMove( event, point );
    }

    Widget3D::mouseMoveEvent( event );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::mouseDoubleClickEvent
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Gl3dWidget::mouseDoubleClickEvent( QMouseEvent* event )
{
    const geometry::Point3f point3 = PointAt( event->pos() );
    if( point3.Z() > -1000 )
    {
        const geometry::Point2f point( point3.X(), point3.Y() );
        strategy_.HandleMouseDoubleClick( event, point );
    }
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::mouseReleaseEvent
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Gl3dWidget::mouseReleaseEvent( QMouseEvent* event )
{
    const geometry::Point3f point3 = PointAt( event->pos() );
    if( point3.Z() > -1000 )
    {
        const geometry::Point2f point( point3.X(), point3.Y() );
        strategy_.HandleMousePress( event, point );
    }
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::mousePressEvent
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Gl3dWidget::mousePressEvent( QMouseEvent* event )
{
    Gl3dWidget::mouseReleaseEvent( event );
    Widget3D::mousePressEvent( event );
}   

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::CenterView
// Created: SBO 2006-03-30
// -----------------------------------------------------------------------------
void Gl3dWidget::CenterView()
{
    const geometry::Rectangle2f& extent = elevation_.Extent();
    const Point3f eye( extent.Width() / 2, -extent.Height() / 2, std::min( extent.Width(), extent.Height() ) );
    Widget3D::LookFrom( eye );
    const Vector3f target = Vector3f( eye, Point3f( extent.Width() / 2, extent.Height() / 2, 0 ) ).Normalize();
    LookTowards( target );
}

// -----------------------------------------------------------------------------
// Name: GL3DWidget::UndoRotations
// Created: AGE 2006-06-28
// -----------------------------------------------------------------------------
void Gl3dWidget::UndoRotations() const
{
    float modelview[ 16 ];
	glGetFloatv( GL_MODELVIEW_MATRIX, modelview );
	for( uint i = 0; i < 3; i++ ) 
	    for( uint j = 0; j < 3; j++ )
            modelview[ i * 4 + j ] = ( i == j ) ? 1.0 : 0.0;
	glLoadMatrixf( modelview );
}
