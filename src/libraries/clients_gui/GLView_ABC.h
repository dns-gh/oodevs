// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GUI_GLVIEW_ABC_H__
#define __GUI_GLVIEW_ABC_H__

#include "LayersHelpers.h"
#include "clients_kernel/Types.h"
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <vector>

class QCursor;
class QFont;
class QImage;
class QWheelEvent;

namespace kernel
{
    class GraphicalEntity_ABC;
    class Location_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;
    class Layer_ABC;
    struct FrustumInfos;
    class GLOptions;
    class GlTooltip_ABC;
    class PickingSelector;

//----------------------------------------------------------------------------
// GLViews hierarchy
//----------------------------------------------------------------------------
//
//                              --------------
//                              | GLView_ABC |
//                              --------------
//                             /              \
//               --------------                ---------------
//               | GLViewBase |                | GLProxyBase |
//               --------------                ---------------
//              /              \              /               \
//    --------------    --------------   ---------------   ---------------
//    | GL2DWidget |    | GL3DWidget |   | GL2D3DProxy |   | GLMainProxy |
//    --------------    --------------   ---------------   ---------------
//
// GLView_ABC  -> pure abstract
//
// GLViewBase  -> common implementation of GL2DWidget and GL3DWidget
// GLProxyBase -> common implementation of GL2D3DProxy and GLMainProxy
//
// GL2DWidget  -> a view which implements 2d drawings operations
// GL3DWidget  -> a view which implements 3d drawings operations
//
// GL2D3DProxy -> a proxy which owns either a 2d or a 3d view
// GLMainProxy -> the proxy which owns every GL2D3DProxy
//
//----------------------------------------------------------------------------
// Use case with 3 views: 2 in 2d and 1 in 3d
//----------------------------------------------------------------------------
//
//                           ---------------
//                           | GLMainProxy |
//                           ---------------
//                         /        |        \
//          ---------------  ---------------  ---------------
//          | GL2D3DProxy |  | GL2D3DProxy |  | GL2D3DProxy |
//          ---------------  ---------------  ---------------
//                 |                |                |
//          --------------   ---------------   ---------------
//          | GL2DWidget |   | GL2DWidget  |   | GL3DWidget  |
//          --------------   ---------------   ---------------
//
//----------------------------------------------------------------------------
//
// GLMainProxy is the GLView_ABC used by the rest of the code to draw, set
// options, pick, etc.
//
// It offers 3 view accessors:
// - CurrentView, the view being currently drawn
// - ActiveView, the view that was clicked / dropped last (the view with the
//   red frame around)
// - HoveredView, the view overflown by the mouse
//
// When a call is made on the GLMainProxy, it forwards the request to the
// appropriate view:
// - draw operations are applied to current view
// - picking operations are applied to the active view (when a user clicks in
//   a view, it becomes the active one, then we process the picking)
// - frustum operations (zoom, center, etc) are applied to the active view
// - tooltips appear on the overflown view
// - etc.
//
// Some drawing operations are common for each view, 2d or 3d, so the
// GLMainProxy implements them itself.
//
//----------------------------------------------------------------------------
//
// GL2D3DProxies are views stocked in the GLMainProxy. They own either a
// GL2DWidget or a GL3DWidget, but never both.
//
// GLOptions are stored in theses proxies. They own every option needed for
// OpenGL rendering, such as textures, call lists, list of aggregated units,
// view options (2D/3D, fog, superior selected, etc.).
//
// Layers order is an exception in options management, because it's stored
// directly into GL2D3DProxy (and not GLOptions), due to an internal
// implementation in MapWidget and 3DWidget in the terrain solution.
// When terrain is in sword, we will fix that.
//
//----------------------------------------------------------------------------
// 
// GL2DWidget and GL3DWidget are openGL renderers.
// 
// Some operations are forwarded to their parent view, such as options
// accessors, view setters, common drawings operations, etc.
//
//----------------------------------------------------------------------------

// =============================================================================
/** @class  GLView_ABC
    @brief  GLView_ABC definition
*/
// Created: ABR 2014-11-05
// =============================================================================
class GLView_ABC : private boost::noncopyable
{

public:
    //! @name Types
    //@{
    enum E_Unit
    {
        pixels,
        meters
    };

    typedef std::pair< unsigned int, E_LayerTypes > T_ObjectPicking;
    typedef std::vector< T_ObjectPicking >          T_ObjectsPicking;

    typedef std::shared_ptr< GLView_ABC >           T_View;
    typedef std::vector< T_View >                   T_Views;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             GLView_ABC() {}
    virtual ~GLView_ABC() {}
    //@}

    //! @name Informations
    //!       GLMainProxy -> throw not implemented
    //!       GL2D3DProxy -> implementation
    //!       GLXWidget   -> forward to parent
    //@{
    virtual const QString& GetName() const = 0;
    virtual unsigned GetID() const = 0;
    //@}

    //! @name Proxy
    //!       GLXProxy  -> implementation
    //!       GLXWidget -> throw not implemented
    //@{
    virtual void Register( const T_View& view ) = 0;
    virtual void Unregister( const T_View& view ) = 0;

    virtual GLView_ABC& GetActiveView() = 0;
    virtual const GLView_ABC& GetActiveView() const = 0;
    virtual GLView_ABC& GetCurrentView() = 0;
    virtual const GLView_ABC& GetCurrentView() const = 0;
    virtual GLView_ABC& GetHoveredView() = 0;
    virtual const GLView_ABC& GetHoveredView() const = 0;
    //@}

    //! @name View setters
    //!       GLMainProxy -> implementation
    //!       GL2D3DProxy -> forward to parent
    //!       GLXWidget   -> forward to parent
    //@{
    virtual void SetActiveView( GLView_ABC& view ) = 0;
    virtual void SetCurrentView( GLView_ABC* view ) = 0;
    virtual void SetHoveredView( GLView_ABC* view ) = 0;
    //@}

    //! @name Options
    //!       GL2D3DProxy -> implementation
    //!       GLMainProxy -> forward to specific child
    //!       GLXWidget   -> forward to parent
    //@{
    virtual void ApplyOptions() = 0;

    virtual GLOptions& GetActiveOptions() = 0;
    virtual const GLOptions& GetActiveOptions() const = 0;

    virtual GLOptions& GetCurrentOptions() = 0;
    virtual const GLOptions& GetCurrentOptions() const = 0;
    //@}

    //! @name Layers
    //!       GLMainProxy -> forward to all children
    //!       GL2D3DProxy -> implementation & forward to all children
    //!       GLXWidget   -> implementation
    //@{
    virtual void AddLayers( const T_LayersVector& layers ) = 0;
    virtual void RemoveLayer( const T_Layer& layer ) = 0;
    //@}

    //! @name Layers order
    //!       GLMainProxy -> forward to active view
    //!       GL2D3DProxy -> implementation & forward to active view
    //!       GLXWidget   -> implementation
    //@{
    virtual bool MoveBelow( const T_Layer& lhs,
                            const T_Layer& rhs ) = 0;
    //@}

    //! @name Layers helpers
    //!       GLMainProxy -> forward to current view
    //!       GL2D3DProxy -> implementation
    //!       GLXWidget   -> throw not implemented
    //@{
    virtual const T_LayersVector& GetLayers() const = 0;
    virtual bool IsInAReadOnlyLayer( const kernel::GraphicalEntity_ABC& selectable ) const = 0;
    virtual void UpdateLayerOrder() = 0;
    //@}

    //! @name Frustum
    //!       GLXProxy  -> forward setter to active view, getter to current view
    //!       GLXWidget -> implementation
    //@{

    virtual FrustumInfos SaveFrustum() const = 0;
    virtual void LoadFrustum( const FrustumInfos& infos ) = 0;

    virtual void CenterOn( const geometry::Point2f& point ) = 0;
    virtual const geometry::Rectangle2f& GetViewport() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
    virtual void Zoom( float width ) = 0;
    virtual float Zoom() const = 0;
    virtual void SetZoom( float zoom ) = 0;
    //@}

    //! @name Common picking
    //!       GLMainProxy -> implementation
    //!       GL2D3DProxy -> forward to parent
    //!       GLXWidget   -> forward to parent
    //@{
    virtual PickingSelector& GetPickingSelector() = 0;
    virtual const PickingSelector& GetPickingSelector() const = 0;
    virtual void RenderPicking( const T_ObjectPicking& object ) = 0;
    //@}

    //! @name Picking
    //!       GLXProxy  -> forward to active view
    //!       GLXWidget -> implementation
    //@{
    virtual void FillSelection( const geometry::Point2f& point,
                                T_ObjectsPicking& selection,
                                const boost::optional< E_LayerTypes >& type ) = 0;
    virtual void Picking() = 0;
    virtual void WheelEvent( QWheelEvent* event ) = 0; // protected in MapWidget, public here
    //@}

    //! @name Tooltip creator
    //!       GLMainProxy -> implementation
    //!       GL2D3DProxy -> throw not implemented
    //!       GLXWidget   -> throw not implemented
    //@{
    virtual std::unique_ptr< GlTooltip_ABC > CreateTooltip() const = 0;
    //@}

    //! @name Tooltip helpers
    //!       GLXProxy  -> forward to hovered view
    //!       GLXWidget -> implementation
    //@{
    virtual geometry::Point2f MapToterrainCoordinates( int x, int y ) = 0;
    //@}

    //! @name Drawing tools
    //!       GLXWidget -> implementation
    //!       GLXProxy  -> forward to all children
    //@{
    virtual void PaintLayers() = 0;
    virtual void UpdateGL() = 0;
    virtual void SetCurrentCursor( const QCursor& cursor ) = 0;
    //@}

    //! @name Common drawing
    //!       GLMainProxy -> implementation
    //!       GL2D3DProxy -> forward to parent
    //!       GLXWidget   -> forward to parent
    //@{
    virtual ColorStrategy_ABC& GetColorStrategy() const = 0;
    virtual float GetCurrentAlpha() const = 0;
    virtual void SetCurrentColor( float r, float g, float b, float a = 1 ) = 0;

    virtual void BindIcon( const char** xpm ) const = 0;
    virtual void DrawActiveFrame() const = 0;
    virtual void DrawBillboardRect() const = 0;
    virtual void DrawSvgInViewport( const std::string& filename,
                                    const geometry::Rectangle2f& viewport,
                                    unsigned vWidth = 640,
                                    unsigned vHeight = 480 ) const = 0;
    virtual void DrawTacticalGraphics( const std::string& symbol,
                                       const kernel::Location_ABC& location,
                                       const geometry::Rectangle2f& viewport,
                                       bool overlined,
                                       float pointSize,
                                       float zoom ) const = 0;
    virtual void PrintApp6( const std::string& symbol,
                            const std::string& style,
                            const geometry::Rectangle2f& viewport,
                            unsigned vWidth = 640,
                            unsigned vHeight = 480,
                            bool checkAlpha = true ) const = 0;
    //@}

    //! @name Drawing
    //!       GLXWidget -> implementation
    //!       GLXProxy  -> forward to current view
    //@{
    virtual float GetAdaptiveZoomFactor( bool bVariableSize = true ) const = 0;
    virtual std::string GetCurrentPass() const = 0;
    virtual float LineWidth( float base ) const = 0;
    virtual float Pixels( const geometry::Point2f& at = geometry::Point2f() ) const = 0;
    virtual uint16_t StipplePattern( int factor = 1 ) const = 0;

    virtual void DrawApp6Symbol( const std::string& symbol,
                                 const geometry::Point2f& where,
                                 float factor = 1.f,
                                 float thickness = 1.f,
                                 unsigned int direction = 0 ) const = 0;
    virtual void DrawApp6SymbolFixedSize( const std::string& symbol,
                                          const geometry::Point2f& where,
                                          float factor ) const = 0;
    virtual void DrawApp6SymbolFixedSize( const std::string& symbol,
                                          const geometry::Point2f& where,
                                          float factor,
                                          unsigned int direction ) const = 0;
    virtual void DrawArc( const geometry::Point2f& center,
                          const geometry::Point2f& from,
                          const geometry::Point2f& to,
                          float width = 1. ) const = 0;
    virtual void DrawArrow( const geometry::Point2f& from,
                            const geometry::Point2f& to,
                            float size = -1.f,
                            E_Unit unit = meters ) const = 0;
    virtual void DrawCell( const geometry::Point2f& center ) const = 0;
    virtual void DrawCircle( const geometry::Point2f& center,
                             float radius = -1.f,
                             E_Unit unit = meters ) const = 0;
    virtual void DrawCross( const geometry::Point2f& at,
                            float size = -1.f,
                            E_Unit unit = meters ) const = 0;
    virtual void DrawCurvedArrow( const geometry::Point2f& from,
                                  const geometry::Point2f& to,
                                  float curveRatio = 0.2f,
                                  float size = -1.f,
                                  E_Unit unit = meters ) const = 0;
    virtual void DrawDecoratedPolygon( const kernel::T_TessellatedPolygon& polygon,
                                       const T_PointVector& vertices,
                                       const geometry::Point2f& center,
                                       const std::string& name,
                                       unsigned int fontHeight ) = 0;
    virtual void DrawDisc( const geometry::Point2f& center,
                           float radius = -1.f,
                           E_Unit unit = meters ) const = 0;
    virtual void DrawHalfDisc( const geometry::Point2f& center,
                               float angleDegrees,
                               float radius = -1.f,
                               E_Unit unit = meters ) const = 0;
    virtual void DrawIcon( const char** xpm,
                           const geometry::Point2f& where,
                           float size = -1.f,
                           float factor = 1.f,
                           E_Unit unit = meters ) const = 0;
    virtual void DrawImage( const QImage& image,
                            const geometry::Point2f& where ) const = 0;
    virtual void DrawInfrastructureSymbol( const std::string& symbol,
                                           const geometry::Point2f& where,
                                           float factor,
                                           float thickness ) const = 0;
    virtual void DrawLife( const geometry::Point2f& center,
                           float height,
                           float factor = 1.f,
                           bool fixedSize = true ) const = 0;
    virtual void DrawLine( const geometry::Point2f& from,
                           const geometry::Point2f& to,
                           float width = 1. ) const = 0;
    virtual void DrawLines( const T_PointVector& points ) const = 0;

    virtual void DrawPolygon( const T_PointVector& points ) const = 0;
    virtual void DrawRectangle( const T_PointVector& points ) const = 0;
    virtual void DrawSelectedPolygon( const T_PointVector& points ) const = 0;
    virtual void DrawShapeText( const QImage& image,
                                const geometry::Point2f& where ) const = 0;
    virtual void DrawStippledLine( const geometry::Point2f& from,
                                   const geometry::Point2f& to ) const = 0;
    virtual void DrawSvg( const std::string& svg,
                          const geometry::Point2f& center,
                          float ratio = 1.f,
                          bool fixedSize = true ) const = 0;
    virtual void DrawTacticalGraphics( const std::string& symbol,
                                       const kernel::Location_ABC& location,
                                       bool overlined,
                                       float pointSize,
                                       bool fixedSize = true ) const = 0;
    virtual void DrawUnitSymbol( const std::string& symbol,
                                 const std::string& moveSymbol,
                                 const std::string& staticSymbol,
                                 const std::string& level,
                                 bool isMoving,
                                 const geometry::Point2f& where,
                                 float factor,
                                 unsigned int direction,
                                 float width,
                                 float depth ) const = 0;
    virtual void DrawUnitSymbolAndTail( const std::string& symbol,
                                        const std::string& level,
                                        const T_PointVector& points ) const = 0;
    virtual void Print( const std::string& message,
                        const geometry::Point2f& where ) const = 0;
    virtual void Print( const std::string& message,
                        const geometry::Point2f& where,
                        const QFont& font ) const = 0;
    //@}
};

} //! namespace gui

#endif // __GUI_GLVIEW_ABC_H__
