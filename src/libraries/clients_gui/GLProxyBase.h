// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef GLProxyBase_h
#define GLProxyBase_h

#include "GLView_ABC.h"

namespace kernel
{
    struct FrustumInfos;
}

namespace gui
{

// =============================================================================
/** @class  GLProxyBase
    @brief  GLProxyBase forwarded calls to GLTools_ABC methods to the current view
*/
// Created: ABR 2014-06-23
// =============================================================================
class GLProxyBase : public GLView_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GLProxyBase();
    virtual ~GLProxyBase();
    //@}

    //! @name Proxy -> implementation
    //@{
    virtual void Register( const T_View& view );
    virtual void Unregister( const T_View& view );
    //@}

    //! @name Layers -> forward to all children
    //@{
    virtual void AddLayers( const T_LayersVector& layers );
    virtual void RemoveLayer( const T_Layer& layer );
    //@}

    //! @name Frustum -> forward setter to active view, getter to current view
    //@{
    virtual FrustumInfos SaveFrustum() const;
    virtual void LoadFrustum( const FrustumInfos& infos );

    virtual void CenterOn( const geometry::Point2f& point );
    virtual geometry::Rectangle2f GetViewport() const;
    virtual void Zoom( float width );
    virtual float Zoom() const;
    virtual void SetZoom( float zoom );
    //@}

    //! @name Picking -> forward to active view
    //@{
    virtual void FillSelection( const geometry::Point2f& point,
                                T_ObjectsPicking& selection,
                                const boost::optional< E_LayerTypes >& type );
    virtual void Picking();
    virtual void WheelEvent( QWheelEvent* event );
    //@}

    //! @name Tooltip helpers -> forward to hovered view
    //@{
    virtual geometry::Point2f MapToterrainCoordinates( int x, int y );
    //@}

    //! @name Drawing tools -> forward to all children or throw if unused
    //@{
    virtual void PaintLayers();
    virtual void UpdateGL();
    virtual void SetCurrentCursor( const QCursor& cursor );
    //@}

    //! @name Drawing operations -> forward to current view
    //@{
    virtual float GetAdaptiveZoomFactor( bool bVariableSize = true ) const;
    virtual std::string GetCurrentPass() const;
    virtual float LineWidth( float base ) const;
    virtual float Pixels( const geometry::Point2f& at = geometry::Point2f() ) const;
    virtual uint16_t StipplePattern( int factor = 1 ) const;

    virtual void DrawApp6Symbol( const std::string& symbol,
                                 const geometry::Point2f& where,
                                 float factor = 1.f,
                                 float thickness = 1.f,
                                 unsigned int direction = 0 ) const;
    virtual void DrawApp6SymbolFixedSize( const std::string& symbol,
                                          const geometry::Point2f& where,
                                          float factor ) const;
    virtual void DrawApp6SymbolFixedSize( const std::string& symbol,
                                          const geometry::Point2f& where,
                                          float factor,
                                          unsigned int direction ) const;
    virtual void DrawArc( const geometry::Point2f& center,
                          const geometry::Point2f& p1,
                          const geometry::Point2f& p2,
                          float width = 1. ) const;
    virtual void DrawArrow( const geometry::Point2f& from,
                            const geometry::Point2f& to,
                            float size = -1.f,
                            E_Unit unit = meters ) const;
    virtual void DrawCell( const geometry::Point2f& center ) const;
    virtual void DrawCircle( const geometry::Point2f& center,
                             float radius = -1.f,
                             E_Unit unit = meters ) const;
    virtual void DrawCross( const geometry::Point2f& at,
                            float size = -1.f,
                            E_Unit unit = meters ) const;
    virtual void DrawCurvedArrow( const geometry::Point2f& from,
                                  const geometry::Point2f& to,
                                  float curveRatio = 0.2f,
                                  float size = -1.f,
                                  E_Unit unit = meters ) const;
    virtual void DrawDecoratedPolygon( const kernel::T_TessellatedPolygon& polygon,
                                       const T_PointVector& vertices,
                                       const geometry::Point2f& center,
                                       const std::string& name,
                                       unsigned int fontHeight );
    virtual void DrawDisc( const geometry::Point2f& center,
                           float radius = -1.f,
                           E_Unit unit = meters ) const;
    virtual void DrawHalfDisc( const geometry::Point2f& center,
                               float angleDegrees,
                               float radius = -1.f,
                               E_Unit unit = meters ) const;
    virtual void DrawInfrastructureSymbol( const std::string& symbol,
                                           const geometry::Point2f& where,
                                           float factor,
                                           float thickness ) const;
    virtual void DrawIcon( const char** xpm,
                           const geometry::Point2f& where,
                           float size = -1.f,
                           float factor = 1.f,
                           E_Unit unit = meters ) const;
    virtual void DrawImage( const QImage& image,
                            const geometry::Point2f& where ) const;
    virtual void DrawLife( const geometry::Point2f& center,
                           float height,
                           float factor = 1.f,
                           bool fixedSize = true ) const;
    virtual void DrawLine( const geometry::Point2f& from,
                           const geometry::Point2f& to,
                           float width = 1. ) const;
    virtual void DrawLines( const T_PointVector& points ) const;

    virtual void DrawPolygon( const T_PointVector& points ) const;
    virtual void DrawRectangle( const T_PointVector& points ) const;
    virtual void DrawSelectedPolygon( const T_PointVector& points ) const;
    virtual void DrawShapeText( const QImage& image,
                                const geometry::Point2f& where ) const;
    virtual void DrawStippledLine( const geometry::Point2f& from,
                                   const geometry::Point2f& to ) const;
    virtual void DrawSvg( const std::string& svg,
                          const geometry::Point2f& center,
                          float ratio = 1.f,
                          bool fixedSize = true ) const;
    virtual void DrawTacticalGraphics( const std::string& symbol,
                                       const kernel::Location_ABC& location,
                                       bool overlined,
                                       float pointSize,
                                       bool fixedSize = true ) const;
    virtual void DrawUnitSymbol( const std::string& symbol,
                                 const std::string& moveSymbol,
                                 const std::string& staticSymbol,
                                 const std::string& level,
                                 bool isMoving,
                                 const geometry::Point2f& where,
                                 float factor,
                                 unsigned int direction,
                                 float width,
                                 float depth ) const;
    virtual void DrawUnitSymbolAndTail( const std::string& symbol,
                                        const std::string& level,
                                        const T_PointVector& points ) const;
    virtual void Print( const std::string& message,
                        const geometry::Point2f& where ) const;
    virtual void Print( const std::string& message,
                        const geometry::Point2f& where,
                        const QFont& font ) const;
    //@}

protected:
    //! @name Member data
    //@{
    T_Views views_;
    std::shared_ptr< GLView_ABC > defaultView_; // first view defined, used by default
    //@}
};

} //! namespace gui

#endif // GLProxyBase_h
