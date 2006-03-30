// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Gl3dWidget.h"
#include "AgentsLayer.h"
#include "ObjectsLayer.h"
#include "ColorStrategy.h"
#include "GlFont.h"
#include "PopulationsLayer.h"
#include "LimitsLayer.h"
#include "ColorStrategy.h"
#include "Model.h"
#include "ParametersLayer.h"
#include "graphics/ElevationMap.h"
#include "graphics/ElevationTextureTree.h"
#include "graphics/Compass.h"
#include "graphics/Visitor3d.h"
#include "graphics/ViewFrustum.h"

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: Gl3dWidget constructor
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
Gl3dWidget::Gl3dWidget( QWidget* pParent, Controllers& controllers, const std::string& scipioXml, ElevationMap& elevation )
    : WorldParameters( scipioXml )
    , SetGlOptions()
    , Widget3D( context_, pParent )
    , GlTools_ABC( controllers )
    , elevation_( elevation )
    , last_( layers_.begin() )
    , default_( 0 )
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
    delete default_;
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::Register
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::Register( Layer_ABC& layer )
{
    layers_.push_back( & layer );
    last_ = layers_.begin();
}

// -----------------------------------------------------------------------------
// Name: Gl3dWidget::SetDefaultLayer
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Gl3dWidget::SetDefaultLayer( Layer_ABC& layer )
{
    default_ = & layer;
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
unsigned short Gl3dWidget::StipplePattern() const
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

    float px = radius, py = 0;
    for( float angle = twoPi / 40.f; angle < twoPi; angle += twoPi / 40.f + 1e-7 )
    {
        float cx = radius * std::cos( angle );
        float cy = radius * std::sin( angle );
        DrawLine( Point2f( px, py ), Point2f( cx, cy ) );
        px = cx; py = cy;
    }
    DrawLine( Point2f( px, py ), Point2f( radius, 0 ) );
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
// Name: Gl3dWidget::DrawRectangle
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
void Gl3dWidget::DrawRectangle( const Rectangle2f& rect ) const
{
    DrawLine( Point2f( rect.Left(), rect.Top() ), Point2f( rect.Right(), rect.Top() ) );
    DrawLine( Point2f( rect.Right(), rect.Top() ), Point2f( rect.Right(), rect.Bottom() ) );
    DrawLine( Point2f( rect.Right(), rect.Bottom() ), Point2f( rect.Left(), rect.Bottom() ) );
    DrawLine( Point2f( rect.Left(), rect.Bottom() ), Point2f( rect.Left(), rect.Top() ) );
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
void Gl3dWidget::DrawApp6Symbol( const std::string& symbol, const Point2f& where ) const
{

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

    bool found = false;
    IT_Layers last = last_;
    for( ; last_ != layers_.end() && ! found; ++last_ )
        if( found = (*last_)->HandleKeyPress( event ) )
            --last_;
    for( last_ = layers_.begin(); last_ != last && ! found; ++last_ )
         if( found = (*last_)->HandleKeyPress( event ) )
            --last_;
    if( ! found )
        default_->HandleKeyPress( event  );

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

        bool found = false;
        IT_Layers last = last_;
        for( ; last_ != layers_.end() && ! found; ++last_ )
            if( found = (*last_)->HandleMouseMove( event, point ) )
                --last_;
        for( last_ = layers_.begin(); last_ != last && ! found; ++last_ )
            if( found = (*last_)->HandleMouseMove( event, point ) )
                --last_;
        if( ! found )
            default_->HandleMouseMove( event, point );
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

        bool found = false;
        IT_Layers last = last_;
        for( ; last_ != layers_.end() && ! found; ++last_ )
            if( found = (*last_)->HandleMouseDoubleClick( event, point ) )
                --last_;
        for( last_ = layers_.begin(); last_ != last && ! found; ++last_ )
            if( found = (*last_)->HandleMouseDoubleClick( event, point ) )
                --last_;
        if( ! found )
            default_->HandleMouseDoubleClick( event, point );
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

        bool found = false;
        IT_Layers last = last_;
        for( ; last_ != layers_.end() && ! found; ++last_ )
            if( found = (*last_)->HandleMousePress( event, point ) )
                --last_;
        for( last_ = layers_.begin(); last_ != last && ! found; ++last_ )
            if( found = (*last_)->HandleMousePress( event, point ) )
                --last_;
        if( ! found )
            default_->HandleMousePress( event, point );
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
    Widget3D::CenterOn( eye );
    const Vector3f target = Vector3f( eye, Point3f( extent.Width() / 2, extent.Height() / 2, 0 ) ).Normalize();
    LookTowards( target );
}