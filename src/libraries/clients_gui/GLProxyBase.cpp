// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GLProxyBase.h"
#include "FrustumInfos.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GLProxyBase constructor
// Created: ABR 2014-06-23
// -----------------------------------------------------------------------------
GLProxyBase::GLProxyBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GLProxyBase destructor
// Created: ABR 2014-06-23
// -----------------------------------------------------------------------------
GLProxyBase::~GLProxyBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Proxy -> implementation
// -----------------------------------------------------------------------------
void GLProxyBase::Register( const std::shared_ptr< GLView_ABC >& view )
{
    if( !view )
        throw MASA_EXCEPTION( "Unable to register a null view" );
    // store this view
    if( std::find( views_.begin(), views_.end(), view ) != views_.end() )
        throw MASA_EXCEPTION( "GLView_ABC already registered" );
    views_.push_back( view );
    // initialize default view
    if( !defaultView_ )
        defaultView_ = view;
}

void GLProxyBase::Unregister( const std::shared_ptr< GLView_ABC >& view )
{
    if( !view )
        throw MASA_EXCEPTION( "Unable to unregister a null view" );
    // reset default view if needed
    if( defaultView_ == view )
        defaultView_.reset();
    // remove this view
    auto it = std::find( views_.begin(), views_.end(), view );
    if( it == views_.end() )
        throw MASA_EXCEPTION( "GLView_ABC not registered" );
    views_.erase( it );
}

// -----------------------------------------------------------------------------
// Layers -> forward to all children
// -----------------------------------------------------------------------------
void GLProxyBase::AddLayers( const T_LayersVector& layers )
{
    for( auto it = views_.begin(); it != views_.end(); ++it )
        ( *it )->AddLayers( layers );
}

void GLProxyBase::RemoveLayer( const T_Layer& layer )
{
    for( auto it = views_.begin(); it != views_.end(); ++it )
        ( *it )->RemoveLayer( layer );
}

// -----------------------------------------------------------------------------
// Frustum -> forwarded to active view
// -----------------------------------------------------------------------------
FrustumInfos GLProxyBase::SaveFrustum() const
{
    return GetActiveView().SaveFrustum();
}

void GLProxyBase::LoadFrustum( const FrustumInfos& infos )
{
    GetActiveView().LoadFrustum( infos );
}

void GLProxyBase::CenterOn( const geometry::Point2f& point )
{
    GetActiveView().CenterOn( point );
}

geometry::Point2f GLProxyBase::GetCenter() const
{
    return GetActiveView().GetCenter();
}

void GLProxyBase::Zoom( float width )
{
    GetActiveView().Zoom( width );
}

float GLProxyBase::Zoom() const
{
    return GetActiveView().Zoom();
}

void GLProxyBase::SetZoom( float zoom )
{
    GetActiveView().SetZoom( zoom );
}

// -----------------------------------------------------------------------------
// Picking -> forward to active view
// -----------------------------------------------------------------------------
void GLProxyBase::FillSelection( const geometry::Point2f& point,
                                 T_ObjectsPicking& selection,
                                 const boost::optional< E_LayerTypes >& type )
{
    GetHoveredView().FillSelection( point, selection, type );
}

void GLProxyBase::Picking()
{
    GetHoveredView().Picking();
}

void GLProxyBase::WheelEvent( QWheelEvent* event )
{
    GetHoveredView().WheelEvent( event );
}

// -----------------------------------------------------------------------------
// Tooltip helpers -> forward to hovered view
// -----------------------------------------------------------------------------
geometry::Point2f GLProxyBase::MapToterrainCoordinates( int x, int y )
{
    return GetHoveredView().MapToterrainCoordinates( x, y );
}

// -----------------------------------------------------------------------------
// Drawing tools -> forward to all children
// -----------------------------------------------------------------------------
void GLProxyBase::UpdateGL()
{
    for( auto it = views_.begin(); it != views_.end(); ++it )
        ( *it )->UpdateGL();
}

void GLProxyBase::SetCurrentCursor( const QCursor& cursor )
{
    std::for_each( views_.begin(), views_.end(), [ &cursor ]( const T_View& view ) {
        view->SetCurrentCursor( cursor );
    } );
}

// -----------------------------------------------------------------------------
// Drawing operations -> forward to current view
// -----------------------------------------------------------------------------
float GLProxyBase::GetAdaptiveZoomFactor( bool bVariableSize /* = true */ ) const
{
    return GetCurrentView().GetAdaptiveZoomFactor( bVariableSize );
}

std::string GLProxyBase::GetCurrentPass() const
{
    return GetCurrentView().GetCurrentPass();
}

float GLProxyBase::LineWidth( float base ) const
{
    return GetCurrentView().LineWidth( base );
}

unsigned short GLProxyBase::StipplePattern( int factor /* = 1*/ ) const
{
    return GetCurrentView().StipplePattern( factor );
}

float GLProxyBase::Pixels( const geometry::Point2f& at /* = geometry::Point2f() */ ) const
{
    return GetCurrentView().Pixels( at );
}

void GLProxyBase::DrawCross( const geometry::Point2f& at,
                             float size /* = -1.f*/,
                             E_Unit unit /* = meters*/ ) const
{
    GetCurrentView().DrawCross( at, size, unit );
}

void GLProxyBase::DrawLine( const geometry::Point2f& from,
                            const geometry::Point2f& to,
                            float width /* = 1. */ ) const
{
    GetCurrentView().DrawLine( from, to, width );
}

void GLProxyBase::DrawStippledLine( const geometry::Point2f& from,
                                    const geometry::Point2f& to ) const
{
    GetCurrentView().DrawStippledLine( from, to );
}

void GLProxyBase::DrawLines( const T_PointVector& points ) const
{
    GetCurrentView().DrawLines( points );
}

void GLProxyBase::DrawRectangle( const T_PointVector& points ) const
{
    GetCurrentView().DrawRectangle( points );
}

void GLProxyBase::DrawPolygon( const T_PointVector& points ) const
{
    GetCurrentView().DrawPolygon( points );
}

void GLProxyBase::DrawSelectedPolygon( const T_PointVector& points ) const
{
    GetCurrentView().DrawSelectedPolygon( points );
}

void GLProxyBase::DrawDecoratedPolygon( const kernel::T_TessellatedPolygon& polygon,
                                        const T_PointVector& vertices,
                                        const geometry::Point2f& center,
                                        const std::string& name,
                                        unsigned int fontHeight )
{
    GetCurrentView().DrawDecoratedPolygon( polygon, vertices, center, name, fontHeight );
}

void GLProxyBase::DrawArrow( const geometry::Point2f& from,
                             const geometry::Point2f& to,
                             float size /* = -1.f*/,
                             E_Unit unit /* = meters*/ ) const
{
    GetCurrentView().DrawArrow( from, to, size,unit );
}

void GLProxyBase::DrawCurvedArrow( const geometry::Point2f& from,
                                   const geometry::Point2f& to,
                                   float curveRatio /* = 0.2f*/,
                                   float size /* = -1.f*/,
                                   E_Unit unit /* = meters*/ ) const
{
    GetCurrentView().DrawCurvedArrow( from, to, curveRatio, size, unit );
}

void GLProxyBase::DrawArc( const geometry::Point2f& center,
                           const geometry::Point2f& p1,
                           const geometry::Point2f& p2,
                           float width /* = 1. */ ) const
{
    GetCurrentView().DrawArc( center, p1, p2, width );
}

void GLProxyBase::DrawCircle( const geometry::Point2f& center,
                              float radius /* = -1.f*/,
                              E_Unit unit /* = meters*/ ) const
{
    GetCurrentView().DrawCircle( center, radius, unit );
}

void GLProxyBase::DrawDisc( const geometry::Point2f& center,
                            float radius /* = -1.f*/,
                            E_Unit unit /* = meters*/ ) const
{
    GetCurrentView().DrawDisc( center, radius, unit );
}

void GLProxyBase::DrawLife( const geometry::Point2f& center,
                            float height,
                            float factor /* = 1.f*/,
                            bool fixedSize /* = true */ ) const
{
    GetCurrentView().DrawLife( center, height, factor, fixedSize );
}

void GLProxyBase::Print( const std::string& message,
                         const geometry::Point2f& where ) const
{
    GetCurrentView().Print( message, where );
}

void GLProxyBase::Print( const std::string& message,
                         const geometry::Point2f& where,
                         const QFont& font ) const
{
    GetCurrentView().Print( message, where, font );
}

void GLProxyBase::DrawInfrastructureSymbol( const std::string& symbol,
                                            const geometry::Point2f& where,
                                            float factor,
                                            float thickness ) const
{
    GetCurrentView().DrawInfrastructureSymbol( symbol, where, factor, thickness );
}

void GLProxyBase::DrawApp6Symbol( const std::string& symbol,
                                  const geometry::Point2f& where,
                                  float factor /* = 1.f */,
                                  float thickness /* = 1.f */,
                                  unsigned int direction /* = 0 */ ) const
{
    GetCurrentView().DrawApp6Symbol( symbol, where, factor, thickness, direction );
}

void GLProxyBase::DrawIcon( const char** xpm,
                            const geometry::Point2f& where,
                            float size /* = -1.f */,
                            float factor /* = 1.f */,
                            E_Unit unit /* = meters */ ) const
{
    GetCurrentView().DrawIcon( xpm, where, size, factor, unit );
}

void GLProxyBase::DrawImage( const QImage& image,
                             const geometry::Point2f& where ) const
{
    GetCurrentView().DrawImage( image, where );
}

void GLProxyBase::DrawCell( const geometry::Point2f& center ) const
{
    GetCurrentView().DrawCell( center );
}

void GLProxyBase::DrawShapeText( const QImage& image, const geometry::Point2f& where ) const
{
    GetCurrentView().DrawShapeText( image, where );
}

void GLProxyBase::DrawSvg( const std::string& svg,
                           const geometry::Point2f& center,
                           float ratio /* = 1.f*/,
                           bool fixedSize /* = true */ ) const
{
    GetCurrentView().DrawSvg( svg, center, ratio, fixedSize );
}

void GLProxyBase::DrawApp6SymbolFixedSize( const std::string& symbol,
                                           const geometry::Point2f& where,
                                           float factor ) const
{
    GetCurrentView().DrawApp6SymbolFixedSize( symbol, where, factor );
}

void GLProxyBase::DrawApp6SymbolFixedSize( const std::string& symbol,
                                           const geometry::Point2f& where,
                                           float factor,
                                           unsigned int direction ) const
{
    GetCurrentView().DrawApp6SymbolFixedSize( symbol, where, factor, direction );
}

void GLProxyBase::DrawUnitSymbol( const std::string& symbol,
                                  const std::string& moveSymbol,
                                  const std::string& staticSymbol,
                                  const std::string& level,
                                  bool isMoving,
                                  const geometry::Point2f& where,
                                  float factor,
                                  unsigned int direction,
                                  float width,
                                  float depth ) const
{
    GetCurrentView().DrawUnitSymbol( symbol,
                                     moveSymbol,
                                     staticSymbol,
                                     level,
                                     isMoving,
                                     where,
                                     factor,
                                     direction,
                                     width,
                                     depth );
}

void GLProxyBase::DrawUnitSymbolAndTail( const std::string& symbol,
                                         const std::string& level,
                                         const T_PointVector& points ) const
{
    GetCurrentView().DrawUnitSymbolAndTail( symbol, level, points );
}

void GLProxyBase::DrawTacticalGraphics( const std::string& symbol,
                                        const kernel::Location_ABC& location,
                                        bool overlined,
                                        float pointSize,
                                        bool fixedSize /* = true */ ) const
{
    GetCurrentView().DrawTacticalGraphics( symbol, location, overlined, pointSize, fixedSize );
}

void GLProxyBase::DrawHalfDisc( const geometry::Point2f& center,
                                float angleDegrees,
                                float radius /* = -1.f */,
                                E_Unit unit /* = meters */ ) const
{
    GetCurrentView().DrawHalfDisc( center, angleDegrees, radius, unit );
}
