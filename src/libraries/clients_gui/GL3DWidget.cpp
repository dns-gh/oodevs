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
#include "EntityLayer.h"
#include "clients_kernel/DetectionMap.h"

#include <graphics/Compass.h>
#include <graphics/Visitor3d.h>
#include <graphics/ViewFrustum.h>
#include <graphics/EventStrategy_ABC.h>

using namespace geometry;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: Gl3dWidget constructor
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
Gl3dWidget::Gl3dWidget( QWidget* pParent, Controllers& controllers, const tools::ExerciseConfig& config, DetectionMap& elevation, EventStrategy_ABC& strategy )
    : WorldParameters( config )
    , SetGlOptions()
    , Widget3D( context_, pParent )
    , GlToolsBase( controllers )
    , elevation_    ( elevation )
    , strategy_     ( strategy )
    , zRatio_       ( 5 )
    , frame_        ( 0 )
    , isInitialized_( false )
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
    if( !isInitialized_ )
    {
        const Rectangle2f viewport( 0, 0, width_, height_ );
        Widget3D::initializeGL();
        glEnableClientState( GL_VERTEX_ARRAY );
        for( CIT_Layers it = layers_.begin(); it != layers_.end(); ++it )
            (*it)->Initialize( viewport );
        isInitialized_ = true;
        CenterView();
    }
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::Paint
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::Paint( const ViewFrustum& view )
{
    current_ = view;
    ++frame_;
    glLineWidth( 1.f );
    glColor3f( 1, 1, 1 );
    glBindTexture( GL_TEXTURE_2D, 0 );
    for( CIT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        (*it)->Paint( view );
}

// $$$$ AGE 2006-04-04: ceci n'est pas un copy/paste

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
// Name: Gl3dWidget::Pixels
// Created: AGE 2007-04-05
// -----------------------------------------------------------------------------
float Gl3dWidget::Pixels( const Point2f& at ) const
{
    return current_.Pixels( Point3f( at.X(), at.Y(), 0.f ) );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::GetAdaptiveZoomFactor
// Created: RPD 2009-12-14
// -----------------------------------------------------------------------------
float Gl3dWidget::GetAdaptiveZoomFactor() const
{
    return 1.f;
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::Zoom
// Created: RPD 2009-12-14
// -----------------------------------------------------------------------------
float Gl3dWidget::Zoom() const
{
    return 1.f;
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawCross
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawCross( const Point2f& at, float size /*= -1.f*/, E_Unit unit /*= meters*/ ) const
{
    if( size < 0 )
        size = 10.f * Pixels( at );
    else if( unit == pixels )
        size *= Pixels( at );

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
float Gl3dWidget::ElevationAt( const Point2f& point ) const
{
    return ( elevation_.ElevationAt( point ) + 2.f ) * zRatio_;
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
// Name: Gl3dWidget::DrawRectangle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawRectangle( const T_PointVector& /*points*/ ) const
{
    //SLG TODO
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawConvexPolygon
// Created: AGE 2007-05-23
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawConvexPolygon( const T_PointVector& points ) const
{
    if( points.size() <= 2 )
        return;
    DrawLines( points );
    DrawLine( points.back(), points.front() );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawConvexPolygon
// Created: RPD 2009-10-05
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawConvexPolygon( const Polygon2f& polygon ) const
{
    const Polygon2f::T_Vertices& points = polygon.Vertices();
    DrawConvexPolygon( points );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawConvexPolygon
// Created: SBO 2010-06-09
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawConvexPolygon( const T_PointVector& /*points*/, bool /*selected*/ ) const
{
    //SLG TODO
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawDecoratedPolygon
// Created: SBO 2010-06-09
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawDecoratedPolygon( const geometry::Polygon2f& /*polygon*/, const urban::UrbanDecoration* /*decoration*/, const std::string& /*name*/, unsigned int /*height*/ ) const
{
    //SLG TODO
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawArrow
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawArrow( const Point2f& from, const Point2f& to, float size /*= -1.f*/, E_Unit unit /*= meters*/ ) const
{
    if( size < 0 )
        size = 15.f * Pixels( to );
    else if( unit == pixels )
        size *= Pixels( to );

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
void Gl3dWidget::DrawArc( const Point2f& center, const Point2f& from, const Point2f& to ) const
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
    glBegin( GL_LINE_STRIP );
    for( float angle = minAngle; angle < maxAngle; angle += deltaAngle )
    {
        const Point2f p = center + radius * Vector2f( std::cos( angle ), std::sin( angle ) );
        const float elevation = ElevationAt( from );
        glVertex3f( p.X(), p.Y(), elevation );
    }
    const Point2f p = center + radius * Vector2f( std::cos( maxAngle ), std::sin( maxAngle ) );
    const float elevation = ElevationAt( from );
    glVertex3f( p.X(), p.Y(), elevation );
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawCurvedArrow
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawCurvedArrow( const Point2f& from, const Point2f& to, float curveRatio /*= 0.2f*/, float size /*= -1.f*/, E_Unit unit /*= meters*/ ) const
{
    if( curveRatio == 0 )
    {
        DrawArrow( from, to, size, unit );
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
void Gl3dWidget::DrawCircle( const Point2f& center, float radius /*= -1.f*/, E_Unit unit /*= meters*/ ) const
{
    static const float twoPi = 2.f * std::acos( -1.f );
    if( radius < 0 )
        radius = 10.f * Pixels( center );
    else if( unit == pixels )
        radius *= Pixels( center );

    float px = center.X() + radius, py = center.Y();
    for( float angle = twoPi / 40.f; angle < twoPi; angle += twoPi / 40.f + 1e-7f )
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
void Gl3dWidget::DrawDisc( const Point2f& center, float radius /*= -1.f*/, E_Unit unit /*= meters*/ ) const
{

    if( radius < 0 )
        radius = 10.f * Pixels( center );
    else if( unit == pixels )
        radius *= Pixels( center );

    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
    glPushAttrib( GL_CURRENT_BIT );
        glTranslatef( center.X(), center.Y(), ElevationAt( center ) );
        glScalef( 1.f, 1.f, 0.5f );
        gluQuadricOrientation( quad, GLU_OUTSIDE );
        gluSphere( quad, radius, 16, 16 );
    glPopAttrib();
    glPopMatrix();
    gluDeleteQuadric( quad );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawLife
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawLife( const Point2f& center, float h, float factor /*= 1.f*/ ) const
{
    // $$$$ AGE 2006-09-11:
    const float halfWidth   = factor * 600.f * 0.5f * 0.92f;
    const float deltaHeight = factor * 600.f * 0.062f;
    const float xdelta = h * halfWidth;
    glPushMatrix();
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glTranslatef( center.X(), center.Y(), ElevationAt( center ) + 100.f );
        UndoRotations();
        glTranslatef( 0, 0, 1 );
        glLineWidth( 3 );
        glColor3f( 1 - h, h, 0.1f ); // $$$$ AGE 2006-09-11:
        glBegin( GL_LINES );
            glVertex2f( - xdelta, deltaHeight );
            glVertex2f(   xdelta, deltaHeight );
        glEnd();
    glPopAttrib();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::Print
// Created: AGE 2007-06-20
// -----------------------------------------------------------------------------
void Gl3dWidget::Print( const std::string& message, const geometry::Point2f& where ) const
{
    Print( message, where, QFont() );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::Print
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::Print( const std::string& message, const Point2f& where, const QFont& font ) const
{
    QGLWidget* that = const_cast< Gl3dWidget* >( this );
    that->renderText( where.X(), where.Y(), ElevationAt( where ), message.c_str(), font );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawApp6Symbol
// Created: AGE 2008-05-07
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawApp6Symbol( const std::string& symbol, const std::string& style, const geometry::Point2f& where, float factor /*= 1.f*/, float /*thickness = 1.f*/ ) const
{
    const float svgDeltaX = -20;
    const float svgDeltaY = -80;
    const float svgWidth = 360;
    const float expectedWidth  = 600.f * factor;
    const float expectedHeight = expectedWidth * 0.660f;
    const float scaleRatio = expectedWidth / svgWidth;

    glPushMatrix();
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glTranslatef( where.X(), where.Y(), ElevationAt( where ) + 100.f );
        UndoRotations();
        glTranslatef( - expectedWidth * 0.5f, expectedHeight, 0 );
        glScalef( scaleRatio, -scaleRatio, scaleRatio );
        glTranslatef( svgDeltaX, svgDeltaY, 0.0f );
        const Rectangle2f bbox( -10000,-10000,10000,10000 ); // $$$$ AGE 2006-09-11:
        Base().PrintApp6( symbol, style, bbox );
    glPopAttrib();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawApp6Symbol
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawApp6Symbol( const std::string& symbol, const Point2f& where, float factor /*= 1.f*/, float thickness /*= 1.f*/ ) const
{
    DrawApp6Symbol( symbol, DefaultStyle(), where, factor, thickness );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawTacticalGraphics
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawTacticalGraphics( const std::string& /*symbol*/, const kernel::Location_ABC& /*location*/, bool /*overlined*/ ) const
{
    // $$$$ SBO 2009-05-29: TODO
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawIcon
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawIcon( const char** /*xpm*/, const Point2f& /*where*/, float /*size*/ /*= -1.f*/, E_Unit /*unit*/ /*= meters*/ ) const
{
    // $$$$ AGE 2006-05-16: ca va dégager de toute facon...
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawImage
// Created: AGE 2006-11-17
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawImage( const QImage& image, const Point2f& where ) const
{
    if( image.bits() )
    {
        glDisable( GL_DEPTH_TEST );
        glRasterPos3f( where.X(), where.Y(), ElevationAt( where ) + 100.f );
        glDrawPixels( image.width(), image.height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
        glEnable( GL_DEPTH_TEST );
    }
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawCell
// Created: AGE 2006-12-01
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawCell( const Point2f& center ) const
{
    static const float halfCellSize = elevation_.GetCellSize() * 0.5f;
    glBegin( GL_QUADS );
    {
        const Point2f p( center.X() - halfCellSize, center.Y() - halfCellSize );
        glVertex3f( p.X(), p.Y(), ElevationAt( p ) );
    }
    {
        const Point2f p( center.X() + halfCellSize, center.Y() - halfCellSize );
        glVertex3f( p.X(), p.Y(), ElevationAt( p ) );
    }
    {
        const Point2f p( center.X() + halfCellSize, center.Y() + halfCellSize );
        glVertex3f( p.X(), p.Y(), ElevationAt( p ) );
    }
    {
        const Point2f p( center.X() - halfCellSize, center.Y() + halfCellSize );
        glVertex3f( p.X(), p.Y(), ElevationAt( p ) );
    }
    glEnd();
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::DrawSvg
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawSvg( const std::string& svg, const geometry::Point2f& center, float ratio /*= 1.f*/ ) const
{
    glPushMatrix();
    glTranslatef( center.X(), center.Y(), ElevationAt( center ) );
    UndoRotations();
    if( ratio != 1.f )
        glScalef( ratio, ratio, ratio );
    const Rectangle2f bbox( -10000,-10000,10000,10000 ); // $$$$ AGE 2006-09-11:
    Base().DrawSvg( svg, bbox );
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::CenterOn
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::CenterOn( const Point2f& point )
{
    Widget3D::CenterOn( point );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::Zoom
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
void Gl3dWidget::Zoom( float /*width*/ )
{
    // $$$$ SBO 2008-07-07: TODO
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
        else if( event->key() == Qt::Key_Minus )
            zRatio_ *= 0.9f;
        else if( event->key() == Qt::Key_Asterisk )
            zRatio_ = 1.0f;
        else if( event->key() == Qt::Key_Home )
        {
            CenterView();
            zRatio_ = 5.0f;
        }
        else if( event->key() == Qt::Key_Left )
            Rotate( Vector3f( 0, 0, 1 ), 0.02f * speedFactor );
        else if( event->key() == Qt::Key_Right )
            Rotate( Vector3f( 0, 0, 1 ), -0.02f * speedFactor );
        else if( event->key() == Qt::Key_Up )
            Translate( Vector3f( 0, 1, 0 ), 500.f * speedFactor );
        else if( event->key() == Qt::Key_Down )
            Translate( Vector3f( 0, 1, 0 ), -500.f * speedFactor );
        else if( event->key() == Qt::Key_PageUp )
            Translate( Vector3f( 0, 0, 1 ), 500.f * speedFactor );
        else if( event->key() == Qt::Key_PageDown )
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
    const Point3f point3 = PointAt( event->pos() );
    if( point3.Z() > -1000 )
    {
        const Point2f point( point3.X(), point3.Y() );
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
    const Point3f point3 = PointAt( event->pos() );
    if( point3.Z() > -1000 )
    {
        const Point2f point( point3.X(), point3.Y() );
        strategy_.HandleMouseDoubleClick( event, point );
    }
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::mouseReleaseEvent
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Gl3dWidget::mouseReleaseEvent( QMouseEvent* event )
{
    const Point3f point3 = PointAt( event->pos() );
    if( point3.Z() > -1000 )
    {
        const Point2f point( point3.X(), point3.Y() );
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
    const Rectangle2f& extent = elevation_.Extent();
    const Point3f eye( extent.Width() / 2, -extent.Height() / 2, std::min( extent.Width(), extent.Height() ) );
    Widget3D::LookFrom( eye );
    const Vector3f target = Vector3f( eye, Point3f( extent.Width() / 2, extent.Height() / 2, 0 ) ).Normalize();
    LookTowards( target );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::UndoRotations
// Created: AGE 2006-06-28
// -----------------------------------------------------------------------------
void Gl3dWidget::UndoRotations() const
{
    float modelview[ 16 ];
    glGetFloatv( GL_MODELVIEW_MATRIX, modelview );
    for( uint i = 0; i < 3; i++ )
        for( uint j = 0; j < 3; j++ )
            modelview[ i * 4 + j ] = ( i == j ) ? 1.f : 0;
    glLoadMatrixf( modelview );
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::SetCurrentCursor
// Created: SBO 2008-02-12
// -----------------------------------------------------------------------------
void Gl3dWidget::SetCurrentCursor( const QCursor& cursor )
{
    setCursor( cursor );
}
