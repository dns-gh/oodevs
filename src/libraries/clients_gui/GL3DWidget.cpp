// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GL3DWidget.h"

#include "EntityLayer.h"
#include "FrustumInfos.h"
#include "GLOptions.h"
#include "PickingSelector.h"
#include "SimpleLocationDrawer.h"
#include "SvglRenderer.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/OptionVariant.h"

#include <graphics/Compass.h>
#include <graphics/EventStrategy_ABC.h>
#include <graphics/ViewFrustum.h>
#include <graphics/Visitor3d.h>

#include <boost/bind.hpp>
#include <GL/glu.h>

using namespace geometry;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: GL3DWidget constructor
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
GL3DWidget::GL3DWidget( QWidget* pParent,
                        GLView_ABC& parent,
                        DetectionMap& elevation,
                        EventStrategy_ABC& strategy,
                        float width,
                        float height,
                        QGLWidget* shareWidget /* = 0 */ )
    : GLViewBase( parent )
    , Widget3D( context_, pParent, shareWidget )
    , extent_( 0, 0, width, height )
    , elevation_( elevation )
    , strategy_( strategy )
    , zRatio_( 5 )
    , frame_( 0 )
    , isInitialized_( false )
    , symbolSize_( 0.f )
{
    if( context() != context_ || !context_->isValid() )
        throw MASA_EXCEPTION( "Unable to create context" );
}

// -----------------------------------------------------------------------------
// Name: GL3DWidget destructor
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
GL3DWidget::~GL3DWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Layers -> implementation
// -----------------------------------------------------------------------------
void GL3DWidget::AddLayers( const T_LayersVector& layers )
{
    layers_.insert( layers_.end(), layers.begin(), layers.end() );
}

bool GL3DWidget::MoveBelow( const std::shared_ptr< Layer_ABC >&,
                            const std::shared_ptr< Layer_ABC >& )
{
    return false;
}

void GL3DWidget::RemoveLayer( const T_Layer& layer )
{
    auto it = std::find( layers_.begin(), layers_.end(), layer );
    if( it != layers_.end() )
        layers_.erase( it );
}

// -----------------------------------------------------------------------------
// Frustum -> implementation
// -----------------------------------------------------------------------------
FrustumInfos GL3DWidget::SaveFrustum() const
{
    return FrustumInfos( center_, front_, right_, top_ );
}

void GL3DWidget::LoadFrustum( const FrustumInfos& infos )
{
    if( !infos.infos3D_ )
        return;
    center_ = infos.infos3D_->center_;
    front_ = infos.infos3D_->front_;
    right_ = infos.infos3D_->right_;
    top_ = infos.infos3D_->top_;
}

void GL3DWidget::CenterOn( const Point2f& point )
{
    Widget3D::CenterOn( point );
}

const geometry::Rectangle2f& GL3DWidget::GetViewport() const
{
    return viewport_;
}

int GL3DWidget::GetWidth() const
{
    return width();
}

int GL3DWidget::GetHeight() const
{
    return height();
}

void GL3DWidget::Zoom( float /*width*/ )
{
    // $$$$ SBO 2008-07-07: TODO
}

float GL3DWidget::Zoom() const
{
    return 1.f;
}

void GL3DWidget::SetZoom( float )
{
    // NOTHING
}

float GL3DWidget::GetAdaptiveZoomFactor( bool bVariableSize /*= true*/ ) const
{
    return bVariableSize ? 1.f : symbolSize_;
}

// -----------------------------------------------------------------------------
// Picking -> implementation
// -----------------------------------------------------------------------------
void GL3DWidget::FillSelection( const geometry::Point2f& point,
                                T_ObjectsPicking& selection,
                                const boost::optional< E_LayerTypes >& type )
{
    if( !IsInSelectionViewport( point ) )
        return;
    GetPickingSelector().FillSelection( selection, type, [&](){
        makeCurrent();
        GLboolean depthTest;
        glGetBooleanv( GL_DEPTH_TEST, &depthTest );
        glDisable( GL_DEPTH_TEST );
        paintGL();
        if( depthTest != GL_FALSE )
            glEnable( GL_DEPTH_TEST );
    } );
}

void GL3DWidget::Picking()
{
    GetPickingSelector().Picking( clickedPoint_, GetHeight() );
}

void GL3DWidget::WheelEvent( QWheelEvent* event )
{
    wheelEvent( event );
}

// -----------------------------------------------------------------------------
// Tooltip helpers -> implementation
// -----------------------------------------------------------------------------
geometry::Point2f GL3DWidget::MapToterrainCoordinates( int x, int y )
{
    return geometry::Point2f( ( float ) x, ( float ) y );
}

// -----------------------------------------------------------------------------
// Common drawing tools -> hack
// -----------------------------------------------------------------------------
void GL3DWidget::SetCurrentColor( float r, float g, float b, float /*a*/ )
{
    // $$$$ MCO 2014-10-17: de-activating alpha in 3D for now
    // $$$$ MCO 2014-10-17: see http://jira.masagroup.net/browse/SWBUG-13308
    GLViewBase::SetCurrentColor( r, g, b, 1 );
}

// -----------------------------------------------------------------------------
// Drawing -> implementation
// -----------------------------------------------------------------------------
void GL3DWidget::SetCurrentCursor( const QCursor& cursor )
{
    setCursor( cursor );
}

std::string GL3DWidget::GetCurrentPass() const
{
    return "";
}

float GL3DWidget::LineWidth( float base ) const
{
    return base;
}

uint16_t GL3DWidget::StipplePattern( int factor /* = 1*/ ) const
{
    static const uint16_t pattern[ ] = {
        0x00FF, 0x01FE, 0x03FC, 0x07F8,
        0x0FF0, 0x1FE0, 0x3FC0, 0x7F80,
        0xFF00, 0xFE01, 0xFC03, 0xF807,
        0xF00F, 0xE01F, 0xC03F, 0x807F
    };
    return pattern[ ( factor * frame_ )%16 ];
}

float GL3DWidget::Pixels( const Point2f& at ) const
{
    return current_.Pixels( Point3f( at.X(), at.Y(), 0.f ) );
}

void GL3DWidget::DrawCross( const Point2f& at, float size /* = -1.f*/, E_Unit unit /* = meters*/ ) const
{
    if( size < 0 )
        size = 10.f * Pixels( at );
    else if( unit == pixels )
        size *= Pixels( at ) * GetAdaptiveZoomFactor( false ) / 100.f;

    const Vector2f u( size, size );
    const Vector2f v( size, -size );
    DrawLine( at + u, at - u );
    DrawLine( at + v, at - v );
}

void GL3DWidget::DrawLine( const Point2f& from, const Point2f& to, float ) const
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

void GL3DWidget::DrawStippledLine( const geometry::Point2f& /*from*/,
                                   const geometry::Point2f& /*to*/ ) const
{
    // TODO
}

void GL3DWidget::DrawLines( const T_PointVector& points ) const
{
    if( points.size() < 2 )
        return;
    for( auto it = points.begin(); it + 1 != points.end(); ++it )
        DrawLine( *it, *(it+1) );
}

void GL3DWidget::DrawRectangle( const T_PointVector& /*points*/, bool /*fill*/ ) const
{
    //SLG TODO
}

void GL3DWidget::DrawPolygon( const T_PointVector& points ) const
{
    if( points.size() <= 2 )
        return;
    DrawLines( points );
    DrawLine( points.back(), points.front() );
}

void GL3DWidget::DrawDecoratedPolygon( const kernel::T_TessellatedPolygon& /*polygon*/,
                                       const T_PointVector& /*vertices*/,
                                       const geometry::Point2f& /*center*/,
                                       const std::string& /*name*/,
                                       unsigned int /*fontHeight*/ )
{
    // NOTHING
}

void GL3DWidget::DrawSelectedPolygon( const T_PointVector& /*points*/ ) const
{
    // TODO
}

void GL3DWidget::DrawArrow( const Point2f& from,
                            const Point2f& to,
                            float size /* = -1.f*/,
                            E_Unit unit /* = meters*/ ) const
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

void GL3DWidget::DrawArc( const Point2f& center,
                          const Point2f& from,
                          const Point2f& to,
                          float ) const
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

void GL3DWidget::DrawCurvedArrow( const Point2f& from,
                                  const Point2f& to,
                                  float curveRatio /* = 0.2f*/,
                                  float size /* = -1.f*/,
                                  E_Unit unit /* = meters*/ ) const
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
}

void GL3DWidget::DrawCircle( const Point2f& center,
                             float radius /* = -1.f*/,
                             E_Unit unit /* = meters*/ ) const
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

void GL3DWidget::DrawDisc( const Point2f& center,
                           float radius /* = -1.f*/,
                           E_Unit unit /* = meters*/ ) const
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

void GL3DWidget::DrawHalfDisc( const geometry::Point2f& center,
                               float angleDegrees,
                               float radius /*= -1.f*/,
                               E_Unit unit /*= meters*/ ) const
{
    double clipPlane[ 4 ];
    clipPlane[ 1 ] = 0;
    clipPlane[ 2 ] = 0;
    const bool right = angleDegrees >= 0;
    if( center.X() == 0 )
    {
        clipPlane[ 0 ] = right ? 1. : -1;
        clipPlane[ 3 ] = 0;
    }
    else
    {
        const double invCenter = 1. / center.X();
        clipPlane[ 0 ] = right ?  invCenter : -invCenter;
        clipPlane[ 3 ] = right ? -1. : 1.;
    }
    glClipPlane( GL_CLIP_PLANE0, clipPlane );
    glEnable( GL_CLIP_PLANE0 );
    DrawDisc( center, radius, unit );
    glDisable( GL_CLIP_PLANE0 );
}

void GL3DWidget::DrawLife( const Point2f& center,
                           float h,
                           float factor /* = 1.f*/,
                           bool /*fixedSize = true */ ) const
{
    if( !GetCurrentOptions().ShouldDisplay( "UnitDetails" ) )
        return;
    if( factor < -1.f )
        factor = GetAdaptiveZoomFactor( false );
    else
        factor *= GetAdaptiveZoomFactor( false );
    // $$$$ AGE 2006-09-11:
    const float halfWidth   = factor * 600.f * 0.5f;
    const float deltaHeight = factor * 600.f * 0.04f;
    const float xdelta = ( 1 + h ) * halfWidth;
    const float barHeight = 60 * GetAdaptiveZoomFactor( false );
    const float alpha = GetCurrentAlpha();
    glPushMatrix();
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glTranslatef( center.X(), center.Y(), ElevationAt( center ) + 100.f );
        UndoRotations();
        glTranslatef( - halfWidth, - 35.f, 1 );
        glColor4f( 0.6f, 0.6f, 0.6f, alpha );
        glBegin( GL_QUADS );
            glVertex2f( 0.f, deltaHeight );
            glVertex2f( halfWidth * 2.f, deltaHeight );
            glVertex2f( halfWidth * 2.f, deltaHeight - barHeight );
            glVertex2f( 0.f, deltaHeight - barHeight );
        glEnd();
        glBegin( GL_QUADS );
            glColor4f( 1 - h, h * h * h, 0.f, alpha );
            glVertex2f( 0.f, deltaHeight );
            glVertex2f( xdelta, deltaHeight );
            glColor4f( 0.8f * ( 1 - h ), 0.8f * h * h * h, 0.f, alpha );
            glVertex2f( xdelta, deltaHeight - barHeight );
            glVertex2f( 0.f, deltaHeight - barHeight );
        glEnd();
        glColor4f( 0.f, 0.f, 0.f, alpha );
        glBegin( GL_LINE_LOOP );
            glVertex2f( 0, deltaHeight );
            glVertex2f( halfWidth * 2.f, deltaHeight );
            glVertex2f( halfWidth * 2.f, deltaHeight - barHeight );
            glVertex2f( 0, deltaHeight - barHeight );
        glEnd();
    glPopAttrib();
    glPopMatrix();
}

void GL3DWidget::Print( const std::string& message,
                        const geometry::Point2f& where ) const
{
    Print( message, where, QFont() );
}

void GL3DWidget::Print( const std::string& message,
                        const Point2f& where,
                        const QFont& font ) const
{
    QGLWidget* that = const_cast< GL3DWidget* >( this );
    that->renderText( where.X(), where.Y(), ElevationAt( where ), message.c_str(), font );
}

void GL3DWidget::DrawApp6Symbol( const std::string& symbol,
                                 const geometry::Point2f& where,
                                 float factor /* = 1.f*/,
                                 float /*thickness = 1.f*/,
                                 unsigned int direction /*= 0*/ ) const
{
    factor = std::abs( factor * GetAdaptiveZoomFactor( false ) );
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
            glRotatef( - (GLfloat)direction, 0, 0, 1 );
            glTranslatef( - expectedWidth * 0.5f, expectedHeight, 0 );
            glScalef( scaleRatio, - scaleRatio, scaleRatio );
            glTranslatef( svgDeltaX, svgDeltaY, 0.0f );
            const Rectangle2f bbox( -10000, -10000, 10000, 10000 );
            PrintApp6( symbol, SvglRenderer::DefaultStyle(), bbox, 640, 480 );
        glPopAttrib();
    glPopMatrix();
}

void GL3DWidget::DrawInfrastructureSymbol( const std::string& symbol,
                                           const geometry::Point2f& where,
                                           float factor,
                                           float /*thickness*/ ) const
{
    DrawApp6Symbol( symbol, where, factor, 1.f, 0u );
}

void GL3DWidget::DrawApp6SymbolFixedSize( const std::string& /* symbol */,
                                          const geometry::Point2f& /* where */,
                                          float /* factor */) const
{
    // NOTHING
}

void GL3DWidget::DrawApp6SymbolFixedSize( const std::string& symbol,
                                          const geometry::Point2f& where,
                                          float factor,
                                          unsigned int direction ) const
{
    DrawApp6Symbol( symbol, where, factor, 1.f, direction );
}

void GL3DWidget::DrawUnitSymbol( const std::string& symbol,
                                 const std::string& /*moveSymbol*/,
                                 const std::string& /*staticSymbol*/,
                                 const std::string& level,
                                 bool /*isMoving*/,
                                 const geometry::Point2f& where,
                                 float factor,
                                 unsigned int /*direction*/,
                                 float /*width*/,
                                 float /*depth*/ ) const
{
    DrawApp6Symbol( symbol, where, factor, 1.f, 0 );
    DrawApp6Symbol( level, where, factor, 1.f, 0 );
}

void GL3DWidget::DrawUnitSymbolAndTail( const std::string&,
                                        const std::string&,
                                        const T_PointVector& ) const
{
    // NOT implemented
}

void GL3DWidget::DrawTacticalGraphics( const std::string& /*symbol*/,
                                       const kernel::Location_ABC& location,
                                       bool /*overlined*/,
                                       float /*pointSize*/,
                                       bool /*fixedSize = true*/ ) const
{
    // $$$$ SBO 2009-05-29: TODO: use SVG renderer instead
    glPushAttrib( GL_LINE_BIT );
    glLineWidth( LineWidth( 3.f ) );
    gui::SimpleLocationDrawer drawer( *this );
    location.Accept( drawer );
    glPopAttrib();
}

void GL3DWidget::DrawIcon( const char** /*xpm*/,
                           const Point2f& /*where*/,
                           float /*size*/ /* = -1.f*/,
                           float /*factor*/ /* = 1.f*/,
                           E_Unit /*unit*/ /* = meters*/ ) const
{
    // $$$$ AGE 2006-05-16: ca va dégager de toute facon...
}

void GL3DWidget::DrawImage( const QImage& image,
                            const Point2f& where ) const
{
    if( image.bits() )
    {
        glRasterPos3f( where.X(), where.Y(), ElevationAt( where ) );
        glDrawPixels( image.width(), image.height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
    }
}

void GL3DWidget::DrawCell( const Point2f& center ) const
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

void GL3DWidget::DrawSvg( const std::string& svg,
                          const geometry::Point2f& center,
                          float ratio /* = 1.f*/,
                          bool /*fixedSize = true*/ ) const
{
    glPushMatrix();
    glTranslatef( center.X(), center.Y(), ElevationAt( center ) );
    UndoRotations();
    if( ratio != 1.f )
        glScalef( ratio, ratio, ratio );
    const Rectangle2f bbox( -10000,-10000,10000,10000 ); // $$$$ AGE 2006-09-11:
    DrawSvgInViewport( svg, bbox );
    glPopMatrix();
}

void GL3DWidget::DrawShapeText( const QImage& image,
                                const geometry::Point2f& where,
                                bool frame ) const
{
    if( image.bits() )
    {
        glPushMatrix();
        glTranslatef( where.X(), where.Y(), ElevationAt( where ) + 100.f );
        glScalef( ( float ) image.width() *  Pixels(), ( float ) image.height() * Pixels(), 1.f );

        if( frame )
        {
            glPushAttrib( GL_ENABLE_BIT );
            glLineStipple( 1, 0x0f0f );
            glEnable( GL_LINE_STIPPLE );
            glBegin( GL_LINE_LOOP );
                glVertex2f( 0.f, -1.f );
                glVertex2f( 0.f, 0 );
                glVertex2f( 1, 0 );
                glVertex2f( 1, -1.f );
            glEnd();
            glPopAttrib();
        }

        glPushAttrib( GL_TEXTURE_BIT | GL_CURRENT_BIT );
        glEnable( GL_TEXTURE_2D );
        glDisable( GL_TEXTURE_GEN_S );
        glDisable( GL_TEXTURE_GEN_T );
        glEnable( GL_ALPHA_TEST );
        glAlphaFunc( GL_GREATER, 0.1f );
        glColor3f( 1.f, 1.f, 1.f );

        unsigned int texture;
        glGenTextures( 1, &texture );
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexImage2D( GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

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

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------
void GL3DWidget::CenterView()
{
    const Rectangle2f& extent = elevation_.Extent();
    const Point3f eye( extent.Width() / 2, -extent.Height() / 2, std::min( extent.Width(), extent.Height() ) );
    Widget3D::LookFrom( eye );
    const Vector3f target = Vector3f( eye, Point3f( extent.Width() / 2, extent.Height() / 2, 0 ) ).Normalize();
    LookTowards( target );
}

bool GL3DWidget::IsInSelectionViewport( const geometry::Point2f& point ) const
{
    // Is in the scene && Is in the terrain
    return rect().contains( mapFromGlobal( QCursor::pos() ) ) && current_.IsVisible( point );
}

void GL3DWidget::UndoRotations() const
{
    float modelview[ 16 ];
    glGetFloatv( GL_MODELVIEW_MATRIX, modelview );
    for( uint i = 0; i < 3; i++ )
        for( uint j = 0; j < 3; j++ )
            modelview[ i * 4 + j ] = ( i == j ) ? 1.f : 0;
    glLoadMatrixf( modelview );
}

float GL3DWidget::ElevationAt( const Point2f& point ) const
{
    return ( static_cast< float >( elevation_.ElevationAt( point ) ) + 2.f ) * zRatio_;
}

// -----------------------------------------------------------------------------
// Qt events
// -----------------------------------------------------------------------------
void GL3DWidget::mousePressEvent( QMouseEvent* event )
{
    SetActiveView( *this );
    if( event )
        clickedPoint_ = event->pos();
    GL3DWidget::mouseReleaseEvent( event );
    Widget3D::mousePressEvent( event );
}

void GL3DWidget::dropEvent( QDropEvent* event )
{
    SetActiveView( *this );
    Widget3D::dropEvent( event );
}

void GL3DWidget::keyPressEvent( QKeyEvent* event )
{
    if( event )
    {
        const float speedFactor = ( event->modifiers() == Qt::ShiftModifier ) ? 10.f : 1.f;
        auto& options = GetActiveOptions();
        if( event->key() == Qt::Key_Plus )
            options.Set( "3DElevationRatio", options.Get( "3DElevationRatio" ).To< float >() * 1.1f );
        else if( event->key() == Qt::Key_Minus )
            options.Set( "3DElevationRatio", options.Get( "3DElevationRatio" ).To< float >() * 0.9f );
        else if( event->key() == Qt::Key_Asterisk )
            options.Set( "3DElevationRatio", 10.f );
        else if( event->key() == Qt::Key_Home )
            CenterView();
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
    Widget3D::keyPressEvent( event );
}

void GL3DWidget::mouseMoveEvent( QMouseEvent* event )
{
    const Point3f point3 = PointAt( event->pos() );
    if( point3.Z() > -1000 )
    {
        const Point2f point( point3.X(), point3.Y() );
        strategy_.HandleMouseMove( event, point );
    }

    Widget3D::mouseMoveEvent( event );
}

void GL3DWidget::mouseDoubleClickEvent( QMouseEvent* event )
{
    const Point3f point3 = PointAt( event->pos() );
    if( point3.Z() > -1000 )
    {
        const Point2f point( point3.X(), point3.Y() );
        strategy_.HandleMouseDoubleClick( event, point );
    }
}

void GL3DWidget::mouseReleaseEvent( QMouseEvent* event )
{
    const Point3f point3 = PointAt( event->pos() );
    if( point3.Z() > -1000 )
    {
        const Point2f point( point3.X(), point3.Y() );
        strategy_.HandleMousePress( event, point );
    }
}

void GL3DWidget::enterEvent( QEvent* event )
{
    SetHoveredView( this );
    Widget3D::enterEvent( event );
}

void GL3DWidget::leaveEvent( QEvent* event )
{
    SetHoveredView( 0 );
    Widget3D::leaveEvent( event );
}

// -----------------------------------------------------------------------------
// OpenGL
// -----------------------------------------------------------------------------
void GL3DWidget::ComputeData()
{
    ++frame_;
    const auto& options = GetCurrentOptions();
    symbolSize_ = options.Get( "SymbolSize/CurrentFactor" ).To< float >();
    zRatio_ = options.Get( "3DElevationRatio" ).To< float >();
}

void GL3DWidget::Paint( const ViewFrustum& view )
{
    current_ = view;
    ApplyOptions();
    ComputeData();
    glLineWidth( 1.f );
    glColor3f( 1, 1, 1 );
    glBindTexture( GL_TEXTURE_2D, 0 );
    PaintLayers();
    DrawActiveFrame();
}

void GL3DWidget::PaintLayers()
{
    for( auto it = layers_.begin(); it != layers_.end(); ++it )
        ( *it )->Paint( current_ );
}

void GL3DWidget::UpdateGL()
{
    if( isVisible() )
        Widget3D::updateGL();
}

void GL3DWidget::initializeGL()
{
    if( !isInitialized_ )
    {
        Widget3D::initializeGL();
        glEnableClientState( GL_VERTEX_ARRAY );
        for( auto it = layers_.begin(); it != layers_.end(); ++it )
            ( *it )->Initialize( extent_ );
        isInitialized_ = true;
        if( center_.IsZero() )
            CenterView();
    }
}

void GL3DWidget::resizeGL( int w, int h )
{
    makeCurrent();
    Widget3D::resizeGL( w, h );
    viewport_ = geometry::Rectangle2f( 0, 0, static_cast< float >( w ), static_cast< float >( h ) );
}

void GL3DWidget::paintGL()
{
    SetCurrentView( this );
    Widget3D::paintGL();
    SetCurrentView( 0 );
}
