// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GL2DWidget.h"

#include "FrustumInfos.h"
#include "GLOptions.h"
#include "GlRenderPass_ABC.h"
#include "IconLayout.h"
#include "PickingSelector.h"
#include "SvglRenderer.h"
#include "FrameCounter.h"
#include "clients_kernel/OptionVariant.h"
#include <graphics/Scale.h>
#include <graphics/extensions.h>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>
#include <iterator>
#include <ctime>

using namespace geometry;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: GL2DWidget::passLess::operator()
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
bool GL2DWidget::passLess::operator()( GlRenderPass_ABC* lhs, GlRenderPass_ABC* rhs ) const
{
    return names_.find( lhs->GetName() ) < names_.find( rhs->GetName() );
}

// -----------------------------------------------------------------------------
// Name: GL2DWidget::GL2DWidget
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
GL2DWidget::GL2DWidget( QWidget* parentWidget,
                        GLView_ABC& parentView,
                        float width,
                        float height,
                        const std::shared_ptr< IconLayout >& iconLayout,
                        QGLWidget* shareWidget /* = 0 */ )
    : SetGlOptions()
    , MapWidget( context_, parentWidget, width, height, shareWidget )
    , GLViewBase( parentView )
    , windowHeight_( 0 )
    , windowWidth_ ( 0 )
    , circle_      ( 0 )
    , halfCircle_  ( 0 )
    , viewport_    ( 0, 0, width, height )
    , frame_       ( 0 )
    , iconLayout_  ( iconLayout )
    , passes_      ( passLess( "" ) )
    , currentPass_ ()
    , hasMultiTexturing_      ( false )
{
    setAcceptDrops( true );
    if( context() != context_ || ! context_->isValid() )
        throw MASA_EXCEPTION( "Unable to create context" );
}

// -----------------------------------------------------------------------------
// Name: GL2DWidget::~GL2DWidget
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
GL2DWidget::~GL2DWidget()
{
    glDeleteLists( halfCircle_, 1 );
    glDeleteLists( circle_, 1 );
}

// -----------------------------------------------------------------------------
// Passes
// -----------------------------------------------------------------------------
void GL2DWidget::SetPassOrder( const std::string& names )
{
    passLess comparator( names );
    T_RenderPasses passes( comparator );
    std::copy( passes_.begin(), passes_.end(), std::inserter( passes, passes.begin() ) );
    std::swap( passes_, passes );
}

void GL2DWidget::AddPass( GlRenderPass_ABC& pass )
{
    passes_.insert( &pass );
}

void GL2DWidget::RemovePass( GlRenderPass_ABC& pass )
{
    passes_.erase( &pass );
}

// -----------------------------------------------------------------------------
// Layers
// -----------------------------------------------------------------------------
void GL2DWidget::AddLayers( const T_LayersVector& layers )
{
    for( auto it = layers.begin(); it != layers.end(); ++it )
        MapWidget::Register( *it );
}

bool GL2DWidget::MoveBelow( const std::shared_ptr< Layer_ABC >& lhs,
                            const std::shared_ptr< Layer_ABC >& rhs )
{
    MapWidget::MoveBelow( lhs, rhs );
    return true;
}

void GL2DWidget::RemoveLayer( const T_Layer& layer )
{
    MapWidget::Unregister( layer );
}

// -----------------------------------------------------------------------------
// MapWidget_ABC
// -----------------------------------------------------------------------------
void GL2DWidget::PaintLayers()
{
    if( hasMultiTexturing_ )  // $$$$ LGY 2012-03-05: disable PaintLayers : crash in remote desktop
        MapWidget::PaintLayers();
}

unsigned int GL2DWidget::Width() const
{
    return width();
}

unsigned int GL2DWidget::Height() const
{
    return height();
}

Rectangle2f GL2DWidget::Viewport() const
{
    return GetViewport();
}

// -----------------------------------------------------------------------------
// Frustum
// -----------------------------------------------------------------------------
FrustumInfos GL2DWidget::SaveFrustum() const
{
    return FrustumInfos( center_, Zoom() );
}

void GL2DWidget::LoadFrustum( const FrustumInfos& infos )
{
    if( !infos.infos2D_ )
        return;
    SetZoom( infos.infos2D_->zoom_ );
    CenterOn( infos.infos2D_->center_ );
}

void GL2DWidget::CenterOn( const Point2f& point )
{
    Center( point );
}

Point2f GL2DWidget::GetCenter() const
{
    return center_;
}

void GL2DWidget::Zoom( float w )
{
    if( width() )
        MapWidget::Zoom( w * height() / width() );
}

float GL2DWidget::GetAdaptiveZoomFactor( bool bVariableSize /*= true*/ ) const
{
    if( !bVariableSize )
        return GetCurrentOptions().Get( "SymbolSize/CurrentFactor" ).To< float >() * Pixels() / 60;

    float zoom = Zoom();
    float pixels = Pixels();
    if( zoom <= .00024f )
        return 1;
    else if( zoom <= .002f )
        return pixels / 15;
    else
        return 0.12f;
}

float GL2DWidget::Zoom() const
{
    return rZoom_;
}

void GL2DWidget::SetZoom( float zoom )
{
    rZoom_ = zoom;
    UpdateViewport();
}

// -----------------------------------------------------------------------------
// Picking -> implementation
// -----------------------------------------------------------------------------
void GL2DWidget::FillSelection( const Point2f& point,
                                T_ObjectsPicking& selection,
                                const boost::optional< E_LayerTypes >& type )
{
    if( !IsInSelectionViewport( point ) )
        return;
    clickedPoint_ = CoordinatesToClient( point );
    GetPickingSelector().FillSelection( selection,
                                        type,
                                        boost::bind( &GL2DWidget::PickGL, this ) );
}

void GL2DWidget::Picking()
{
    GetPickingSelector().Picking( clickedPoint_, windowHeight_ );
}

void GL2DWidget::WheelEvent( QWheelEvent* event )
{
    MapWidget::wheelEvent( event );
}

// -----------------------------------------------------------------------------
// Tooltip helpers -> implementation
// -----------------------------------------------------------------------------
Point2f GL2DWidget::MapToterrainCoordinates( int x, int y )
{
    return RetrieveCoordinates( mapFromGlobal( QPoint( x, y ) ).x(),
                                mapFromGlobal( QPoint( x, y ) ).y() );
}

// -----------------------------------------------------------------------------
// Drawing -> implementation
// -----------------------------------------------------------------------------
std::string GL2DWidget::GetCurrentPass() const
{
    return currentPass_;
}

float GL2DWidget::LineWidth( float base ) const
{
    if( GetPickingSelector().IsPickingMode() )
        return 5.f * Pixels();
    return base;
}

uint16_t GL2DWidget::StipplePattern( int factor /* = 1*/ ) const
{
    static const uint16_t pattern[] = {
        0x00FF, 0x01FE, 0x03FC, 0x07F8,
        0x0FF0, 0x1FE0, 0x3FC0, 0x7F80,
        0xFF00, 0xFE01, 0xFC03, 0xF807,
        0xF00F, 0xE01F, 0xC03F, 0x807F
    };
    return pattern[ ( factor * frame_ ) % 16 ];
}

float GL2DWidget::Pixels( const Point2f& ) const
{
    if( windowWidth_ > 0 )
        return viewport_.Width() / windowWidth_;
    return 0.f;
}

void GL2DWidget::SetCurrentCursor( const QCursor& cursor )
{
    setCursor( cursor );
}

void GL2DWidget::DrawCross( const Point2f& at,
                            float size /* = -1.f*/,
                            E_Unit unit /* = meters*/ ) const
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

void GL2DWidget::DrawLine( const Point2f& from,
                           const Point2f& to,
                           float width ) const
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

void GL2DWidget::DrawStippledLine( const Point2f& from,
                                   const Point2f& to ) const
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

void GL2DWidget::DrawLines( const T_PointVector& points ) const
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

void GL2DWidget::DrawRectangle( const T_PointVector& points ) const
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

void GL2DWidget::DrawPolygon( const T_PointVector& points ) const
{
    if( points.empty() )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT );
    glEnable( GL_STENCIL_TEST );
    glStencilMask( 0xFF );
    glStencilFunc( GL_EQUAL, 0x0, 0x1 );
    glStencilOp( GL_KEEP, GL_INVERT, GL_INVERT );
    glClear( GL_STENCIL_BUFFER_BIT );
    glVertexPointer( 2, GL_FLOAT, 0, (const void*)(&points.front()) );
    float color[4];
    glGetFloatv( GL_CURRENT_COLOR, color );
    if( !GetPickingSelector().IsPickingMode() )
        color[3] *= 0.5;
    glColor4fv( color );
    glEnable( GL_POLYGON_SMOOTH );
    glDrawArrays( GL_TRIANGLE_FAN, 0, static_cast< GLsizei >( points.size() ) );
    glPopAttrib();
}

void GL2DWidget::DrawSelectedPolygon( const T_PointVector& points ) const
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

void GL2DWidget::DrawDecoratedPolygon( const T_TessellatedPolygon& polygon,
                                       const T_PointVector& vertices,
                                       const Point2f& center,
                                       const std::string& name,
                                       unsigned int fontHeight )
{
    if( polygon.empty() )
        return;

    for( auto it = polygon.begin(); it != polygon.end(); ++it )
    {
        glBegin( it->first );
        const auto& points = it->second;
        for( auto itPoint = points.begin(); itPoint != points.end(); ++itPoint )
            glVertex2dv( (const double*) &*itPoint );
        glEnd();
    }

    for( auto it = vertices.begin(); it + 1 != vertices.end(); ++it )
        DrawLine( *it, *( it + 1 ), 1. );
    DrawLine( vertices.back(), vertices.front(), 1. );

    if( !name.empty() )
        DrawTextLabel( name, center, fontHeight );
}

void GL2DWidget::DrawArrow( const Point2f& from,
                            const Point2f& to,
                            float size /* = -1.f*/,
                            E_Unit unit /* = meters*/ ) const
{
    Point2f end = to;
    float tipFactor = 1.f;
    if( unit == GLView_ABC::pixels )
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

void GL2DWidget::DrawArc( const Point2f& center,
                          const Point2f& from,
                          const Point2f& to,
                          float width ) const
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

void GL2DWidget::DrawCurvedArrow( const Point2f& from,
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
    const Vector2f u( from, to );
    const Vector2f v( u.Normal() );
    const Point2f middle = from + 0.5f * u;
    const Point2f center = middle + v * ( 1.f / curveRatio - 1.f );

    DrawArc( center, from, to, 3.f );
    Vector2f endSegment = Vector2f( center, to ).Normal();
    endSegment.Normalize();
    DrawArrow( to - endSegment * 10.f * Pixels(), to, 15.f * Pixels() );
}

namespace
{
    float Radius( float radius,
                  GLView_ABC::E_Unit unit,
                  float pixels )
    {
        if( radius < 0 )
            return 10.f * pixels;
        if( unit == GLView_ABC::pixels )
            return radius * pixels;
        return radius;
    }
    void DrawDiscPart( const Point2f& center,
                       int glList,
                       float angleDegrees,
                       float radius,
                       GLView_ABC::E_Unit unit,
                       float pixels )
    {
        radius = Radius( radius, unit, pixels );
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
}

void GL2DWidget::DrawCircle( const Point2f& center,
                             float radius /* = -1.f*/,
                             E_Unit unit /* = meters*/ ) const
{
    radius = Radius( radius, unit, Pixels() );
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


void GL2DWidget::DrawDisc( const Point2f& center,
                           float radius /* = -1.f*/,
                           E_Unit unit /* = meters*/ ) const
{
    DrawDiscPart( center, circle_, 0, radius, unit, Pixels() );
}

void GL2DWidget::DrawHalfDisc( const Point2f& center,
                               float angleDegrees,
                               float radius /*= -1.f*/,
                               E_Unit unit /*= meters*/ ) const
{
    DrawDiscPart( center, halfCircle_, angleDegrees, radius, unit, Pixels() );
}

void GL2DWidget::DrawLife( const Point2f& where,
                           float h,
                           float factor /* = 1.f*/,
                           bool fixedSize /*= true*/ ) const
{
    if( !GetCurrentOptions().ShouldDisplay( "UnitDetails" ) )
        return;
    // $$$$ AGE 2006-04-10: hard coded voodoo numbers
    factor *= GetAdaptiveZoomFactor( !fixedSize );
    const float halfWidth   = factor * 600.f * 0.5f * 0.92f;
    const float deltaHeight = factor * 600.f * 0.062f;

    const float y = where.Y() - deltaHeight;
    const float x = where.X();
    const float ydelta = factor * 20.f; // $$$$ SBO 2007-05-04: hard coded again
    const float xdelta = h * halfWidth * 2;
    const float alpha = GetCurrentAlpha();
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glEnable( GL_LINE_SMOOTH );
        glLineWidth( 1. );
        glColor4f( 0.8f, 0.8f, 0.8f, alpha );  // light gray
        glBegin( GL_QUADS );
            glVertex2f( x - halfWidth + xdelta, y - ydelta );
            glVertex2f( x + halfWidth, y - ydelta );
            glVertex2f( x + halfWidth, y + ydelta );
            glVertex2f( x - halfWidth + xdelta, y + ydelta );
        glEnd();
        glColor4f( 1 - h, h*h*h, 0.f, alpha ); // gradiation from green (full) to red (dead)
        glBegin( GL_QUADS );
            glVertex2f( x - halfWidth, y - ydelta );
            glVertex2f( x - halfWidth + xdelta, y - ydelta );
            glVertex2f( x - halfWidth + xdelta, y + ydelta );
            glVertex2f( x - halfWidth, y + ydelta );
        glEnd();
        glColor4f( 0.1f, 0.1f, 0.1f, alpha );   // almost black
        glBegin( GL_LINE_LOOP );
            glVertex2f( x - halfWidth, y - ydelta );
            glVertex2f( x + halfWidth, y - ydelta );
            glVertex2f( x + halfWidth, y + ydelta );
            glVertex2f( x - halfWidth, y + ydelta );
        glEnd();
    glPopAttrib();
}

void GL2DWidget::Print( const std::string& message, const Point2f& where ) const
{
   Print( message, where, QFont() );
}

void GL2DWidget::Print( const std::string& message,
                        const Point2f& where,
                        const QFont& /*font*/ ) const
{
   const_cast< GL2DWidget*>( this )->renderText( where.X(), where.Y(), 2, message.c_str(), QFont() );
}

namespace
{
    const float baseWidth = 600.f;
}

void GL2DWidget::DrawApp6Symbol( const std::string& symbol,
                                 const Point2f& where,
                                 float factor /* = 1.f*/,
                                 float thickness /* = 1.f*/,
                                 unsigned int direction /*= 0*/ ) const
{
    const auto width = static_cast< unsigned int >( windowWidth_ * thickness );
    const auto height = static_cast< unsigned int >( windowHeight_ * thickness );
    thickness *= ComputeZoomFactor( factor );
    DrawApp6( symbol, where, baseWidth * factor, viewport_, width, height, direction );
}

void GL2DWidget::DrawInfrastructureSymbol( const std::string& symbol,
                                           const Point2f& where,
                                           float factor,
                                           float thickness ) const
{
    const auto width = static_cast< unsigned int >( windowWidth_ * thickness );
    const auto height = static_cast< unsigned int >( windowHeight_ * thickness );
    thickness *= ComputeZoomFactor( factor );
    DrawApp6( symbol, where, baseWidth * factor, viewport_,width, height, 0, false );
}

void GL2DWidget::DrawApp6SymbolFixedSize( const std::string& symbol,
                                          const geometry::Point2f& where,
                                          float factor ) const
{
    const Rectangle2f viewport( 0, 0, 600, 600 );
    DrawApp6( symbol, where, baseWidth * factor, viewport, 900, 900, 0, 1, 1 );
}

void GL2DWidget::DrawApp6SymbolFixedSize( const std::string& symbol,
                                          const Point2f& where,
                                          float factor,
                                          unsigned int direction ) const
{
    const Rectangle2f viewport( 0, 0, 256, 256 );
    ComputeZoomFactor( factor, false );
    DrawApp6( symbol, where, baseWidth * factor, viewport, 4, 4, direction );
}

namespace
{
    const float defaultSymbolSize = 3.f;
    const float symbolDepth = 240;
}

void GL2DWidget::DrawUnitSymbol( const std::string& symbol,
                                 const std::string& moveSymbol,
                                 const std::string& staticSymbol,
                                 const std::string& level,
                                 bool isMoving,
                                 const Point2f& where,
                                 float factor,
                                 unsigned int direction,
                                 float width,
                                 float depth ) const
{
    const float xFactor = direction > 180 ? -1.f : 1.f;
    if( isMoving )
    {
        if( !moveSymbol.empty() )
        {
            const auto scaledDepth = symbolDepth * GetCurrentOptions().Get( "SymbolSize/CurrentFactor" ).To< float >() / defaultSymbolSize;
            geometry::Vector2f directionVector( 0, 1 );
            directionVector.Rotate( -3.14f * direction / 180 );
            const geometry::Point2f arrowTail = where - directionVector * depth;
            const geometry::Point2f arrowHead = where;
            const geometry::Point2f symbolTail = arrowHead - directionVector * scaledDepth;
            const geometry::Vector2f symbolVector( symbolTail, arrowHead );
            const geometry::Point2f symbolPosition = symbolTail + symbolVector / 2;
            DrawApp6SymbolScaledSize( moveSymbol, symbolPosition, factor, direction, xFactor, 1 );
            if( depth && depth > scaledDepth )
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
        if( staticSymbol.empty() )
            DrawApp6SymbolFixedSize( symbol, where, factor, 0 );
        else
            DrawApp6SymbolScaledSize(
                staticSymbol, where, factor, direction,
                ( width ? width / 360 : 1 ) * xFactor,
                depth ? depth / symbolDepth : 1 );
    }
    const bool app6 = isMoving ? moveSymbol.empty() : staticSymbol.empty();
    if( app6 )
        DrawApp6SymbolFixedSize( level, where, -std::abs( factor ), 0 );
}

void GL2DWidget::DrawUnitSymbolAndTail( const std::string& symbol,
                                        const std::string& level,
                                        const T_PointVector& points ) const
{
    Point2f penultimatePoint = points.at( points.size() -2 );
    Point2f lastPoint = points.back();
    float symbolDepth = 240;
    Vector2f directionVector( penultimatePoint, lastPoint );
    directionVector.Normalize();
    Vector2f vertical( 0.f, 1.f );
    float radians = Angle( vertical, directionVector );
    int direction = static_cast< int >( radians * 180 / 3.14f );
    if( direction < 0 )
        direction = 360 + direction;
    bool mirror = direction > 180;
    float xFactor = mirror ? -1.f : 1.f;
    unsigned int udirection = 360 - static_cast< unsigned int >( direction );
    Point2f symbolTail = lastPoint + directionVector * (-symbolDepth/2);
    DrawApp6SymbolScaledSize( symbol, lastPoint, -1.f, udirection, xFactor, 1 );
    DrawApp6SymbolScaledSize( level, lastPoint, -1.f, udirection, xFactor, 1 );
    T_PointVector arrowPoints( points );
    arrowPoints.pop_back();
    arrowPoints.push_back( symbolTail );
    DrawTail( arrowPoints, 3.f );
}

void GL2DWidget::DrawTacticalGraphics( const std::string& symbol,
                                       const Location_ABC& location,
                                       bool overlined,
                                       float pointSize,
                                       bool fixedSize /*= true*/ ) const
{
    parent_.DrawTacticalGraphics( symbol,
                                  location,
                                  viewport_,
                                  overlined,
                                  pointSize,
                                  GetAdaptiveZoomFactor( !fixedSize ) );
}

void GL2DWidget::DrawIcon( const char** xpm,
                           const Point2f& where,
                           float size /* = -1.f */,
                           float factor /* = 1.f */, 
                           E_Unit unit /* = meters */ ) const
{
    if( !GetCurrentOptions().ShouldDisplay( "UnitDetails" ) || !iconLayout_ )
        return;
    if( size < 0 )
        size = 32 * Pixels();

    factor *= GetAdaptiveZoomFactor( unit != GLView_ABC::pixels );
    size *= 0.7f * factor;
    glPushMatrix();
    glPushAttrib( GL_TEXTURE_BIT );
        glEnable( GL_TEXTURE_2D );
        glDisable( GL_TEXTURE_GEN_S );
        glDisable( GL_TEXTURE_GEN_T );
        BindIcon( xpm );
        const Point2f iconTranslation = iconLayout_->IconLocation( xpm );
        glTranslatef( where.X() + iconTranslation.X()*factor, where.Y() + iconTranslation.Y()*factor, 0.f );
        glScalef( size, size, size );
        DrawBillboardRect();
        glPopMatrix();
    glPopAttrib();
}

void GL2DWidget::DrawImage( const QImage& image, const Point2f& where ) const
{
    if( image.bits() )
    {
        glRasterPos2f( where.X(), where.Y() );
        glDrawPixels( image.width(), image.height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
    }
}

void GL2DWidget::DrawCell( const Point2f& center ) const
{
    glVertex2fv( (const float*)&center );
}

void GL2DWidget::DrawSvg( const std::string& svg,
                          const Point2f& center,
                          float ratio /* = 1.f*/,
                          bool fixedSize /*= true*/ ) const
{
    glPushMatrix();
    glTranslatef( center.X(), center.Y(), 0 );
    if( ratio != 1 )
        glScalef( ratio, ratio, ratio );
    DrawSvgInViewport( svg,
                       fixedSize ? Rectangle2f( Point2f( 0.f, 0.f ), Point2f( 5000, 5000 ) ) : viewport_,
                       windowWidth_,
                       windowHeight_ );
    glPopMatrix();
}

void GL2DWidget::DrawShapeText( const QImage& image,
                                const Point2f& where ) const
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
        glGenTextures( 1, &texture );
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexImage2D( GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        glTranslatef( where.X(), where.Y(), 300.f );
        glScalef( ( float ) image.width() *  Pixels(), ( float ) image.height() * Pixels(), 1.f );
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
        glPopAttrib();
        glPopMatrix();

        glDeleteTextures( 1, &texture );
    }
}

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------
float GL2DWidget::ComputeZoomFactor( float& factor,
                                     bool bVariableSize /*= true*/ ) const
{
    float adaptiveZoomFactor = factor < 0.f ? GetAdaptiveZoomFactor( bVariableSize ) : 1.f;
    factor = fabs( factor ) * adaptiveZoomFactor;
    return adaptiveZoomFactor;
}

void GL2DWidget::DrawApp6( const std::string& symbol,
                           const geometry::Point2f& where,
                           float expectedWidth,
                           const geometry::Rectangle2f& viewport,
                           unsigned int printWidth,
                           unsigned int printHeight,
                           unsigned int direction,
                           bool checkAlpha /* = true */,
                           float xFactor /* = 1. */,
                           float yFactor /* = 1. */,
                           float svgDeltaX /* = -20 */,
                           float svgDeltaY /* = -80 */ ) const
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
            PrintApp6( symbol, SvglRenderer::DefaultStyle(), viewport, printWidth, printHeight, checkAlpha );
        glPopMatrix();
    glPopAttrib();
}

namespace
{
    const float zoomFactor = 0.6f;// 1 pixel <=> 1 metre
}

void GL2DWidget::DrawApp6SymbolScaledSize( const std::string& symbol,
                                           const Point2f& where,
                                           float factor,
                                           unsigned int direction,
                                           float width,
                                           float depth ) const
{
    const float symbolSize = GetCurrentOptions().Get( "SymbolSize/CurrentFactor" ).To< float >();
    const float svgDeltaX = -20; // Offset of 20 in our svg files...
    const float svgDeltaY = -80 + 120; // Offset of 80 in our svg files + half of 240 which is the default height...
    const Rectangle2f viewport( 0, 0, 256, 256 );
    factor = fabs( factor ) * zoomFactor * symbolSize / defaultSymbolSize;
    DrawApp6( symbol, where, baseWidth * factor, viewport, 4, 4, direction, true, width, depth, svgDeltaX, svgDeltaY );
}

void GL2DWidget::DrawTail( const T_PointVector& points, float width ) const
{
    glPushAttrib( GL_LINE_BIT );
    glLineWidth( std::abs( width ) );
    DrawLines( points );
    glPopAttrib();
}

void GL2DWidget::DrawTextLabel( const std::string& content, const Point2f& where, int /*baseSize = 12*/ )
{
    HDC screen = GetDC( NULL );
    const int hSize = GetDeviceCaps( screen, HORZSIZE );
    const int hRes = GetDeviceCaps( screen, HORZRES );
    ReleaseDC( NULL, screen );
    const float scale = Pixels() * 1000.f * hRes / hSize;

    if( scale < GetCurrentOptions().Get( "VisualisationScales/urban_blocks/Min" ).To< int >() ||
        scale >= GetCurrentOptions().Get( "VisualisationScales/urban_blocks/Max" ).To< int >() )
        return;

    QFontMetrics fm( currentFont_ );
    QRect rc = fm.boundingRect( content.c_str() );

    QPoint point = CoordinatesToClient( where );
    Point2f leftBottom = RetrieveCoordinates( point.x() - 4, int( point.y() + rc.height() * 0.25f ) );
    Point2f rightTop = RetrieveCoordinates( point.x() + rc.width() + 4, int( point.y() - rc.height() * 0.75f ) );

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

bool GL2DWidget::IsInSelectionViewport( const Point2f& point ) const
{
    // Is in the scene && Is in the terrain
    return rect().contains( mapFromGlobal( QCursor::pos() ) ) && extent_.IsInside( point );
}

void GL2DWidget::UpdateStipple() const
{
    glEnable( GL_LINE_STIPPLE );
    auto time = clock();
    auto shift = ( uint16_t ) ( ( int64_t ) ( time / 8 ) % 128 ) / 8;
    glLineStipple( 1, 0x0FFF << shift | 0x0FFF >> ( 16 - shift ) );
}

// -----------------------------------------------------------------------------
// Qt events
// -----------------------------------------------------------------------------
void GL2DWidget::keyPressEvent( QKeyEvent* event )
{
    if( event->key() == Qt::Key_F )
        fps_.reset( fps_ ? 0 : new FrameCounter( *this, 20, -20 ) );
    else
        MapWidget::keyPressEvent( event );
}

void GL2DWidget::mousePressEvent( QMouseEvent* event )
{
    SetActiveView( *this );
    MapWidget::mousePressEvent( event );
}

void GL2DWidget::dropEvent( QDropEvent* event )
{
    SetActiveView( *this );
    MapWidget::dropEvent( event );
}

void GL2DWidget::enterEvent( QEvent* event )
{
    SetHoveredView( this );
    MapWidget::enterEvent( event );
}

void GL2DWidget::leaveEvent( QEvent* event )
{
    SetHoveredView( 0 );
    MapWidget::enterEvent( event );
}

// -----------------------------------------------------------------------------
// OpenGL
// -----------------------------------------------------------------------------
void GL2DWidget::UpdateGL()
{
    if( isVisible() )
        MapWidget::updateGL();
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
    unsigned int GenerateCircle()
    {
        return GenerateCirclePart( 2.f * std::acos( -1.f ) );
    }
    unsigned int GenerateHalfCircle()
    {
        return GenerateCirclePart( std::acos( -1.f ) );
    }
}

void GL2DWidget::initializeGL()
{
    glEnable( GL_TEXTURE_2D );
    MapWidget::initializeGL();
    circle_ = GenerateCircle();
    halfCircle_ = GenerateHalfCircle();
    glEnableClientState( GL_VERTEX_ARRAY );
    gl::Initialize();
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    hasMultiTexturing_ = gl::HasMultiTexturing();
}

void GL2DWidget::resizeGL( int w, int h )
{
    windowHeight_ = h;
    windowWidth_ = w;
    MapWidget::resizeGL( w, h );
}

void GL2DWidget::paintGL()
{
    if( !hasMultiTexturing_ ) // $$$$ LGY 2012-03-05: disable painGL : crash in remote desktop
        return;
    SetCurrentView( this );
    ApplyOptions();
    for( auto it = passes_.begin(); it != passes_.end(); ++it )
        RenderPass( **it );
    if( fps_ )
        fps_->Update();
    Scale().Draw( 20, 20, *this );
    DrawActiveFrame();
    SetCurrentView( 0 );
}

void GL2DWidget::PickGL()
{
    if( !hasMultiTexturing_ ) // $$$$ LGY 2012-03-05: disable painGL : crash in remote desktop
        return;
    makeCurrent();
    SetCurrentView( this );
    ApplyOptions();
    for( auto it = passes_.begin(); it != passes_.end(); ++it )
        if( ( *it )->GetName() == "main" )
            RenderPass( **it );
    SetCurrentView( 0 );
}

void GL2DWidget::RenderPass( GlRenderPass_ABC& pass )
{
    ++frame_;
    currentPass_ = pass.GetName();
    const Rectangle2f viewport = GetViewport();
    const int windowHeight = windowHeight_;
    const int windowWidth = windowWidth_;
    viewport_ = pass.Viewport();
    SetViewport( viewport_ );
    windowWidth_ = pass.Width();
    windowHeight_ = pass.Height();

    MapWidget::resizeGL( windowWidth_, windowHeight_ );
    pass.Render( *this );

    SetViewport( viewport );
    viewport_ = viewport;
    windowHeight_ = windowHeight;
    windowWidth_ = windowWidth;
    MapWidget::resizeGL( windowWidth_, windowHeight_ );
}
