// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GL3DWidget_h_
#define __GL3DWidget_h_

#include "GLViewBase.h"
#include <graphics/Widget3D.h>
#include <graphics/ViewFrustum.h>

class EventStrategy_ABC;

namespace kernel
{
    class DetectionMap;
    class Controllers;
}

namespace tools
{
    class ExerciseConfig;
}

namespace gui
{
    class DrawingTypes;
    class Layer;
    class PickingSelector;

// =============================================================================
/** @class  GL3DWidget
    @brief  GL3DWidget
*/
// Created: AGE 2006-03-28
// =============================================================================
class GL3DWidget : public GLViewBase
                 , public Widget3D
{
public:
    //! @name Constructors/Destructor
    //@{
             GL3DWidget( QWidget* pParent,
                         GLView_ABC& parent,
                         kernel::DetectionMap& elevation,
                         EventStrategy_ABC& strategy,
                         float width,
                         float height,
                         QGLWidget* shareWidget = 0 );
    virtual ~GL3DWidget();
    //@}

private:
    //! @name Layers -> implementation
    //@{
    virtual void AddLayers( const T_LayersVector& layers );
    virtual bool MoveBelow( const std::shared_ptr< Layer_ABC >& lhs,
                            const std::shared_ptr< Layer_ABC >& rhs );
    virtual void RemoveLayer( const T_Layer& layer );
    //@}

    //! @name Frustum -> implementation
    //@{
    virtual FrustumInfos SaveFrustum() const;
    virtual void LoadFrustum( const FrustumInfos& infos );

    virtual void CenterOn( const geometry::Point2f& point );
    virtual const geometry::Rectangle2f& GetViewport() const;
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual void Zoom( float width );
    virtual float Zoom() const;
    virtual void SetZoom( float zoom );
    virtual float GetAdaptiveZoomFactor( bool bVariableSize = true ) const;
    //@}

    //! @name Picking -> implementation
    //@{
    virtual void FillSelection( const geometry::Point2f& point,
                                T_ObjectsPicking& selection,
                                const boost::optional< E_LayerTypes >& type );
    virtual void Picking();
    virtual void WheelEvent( QWheelEvent* event );
    //@}

    //! @name Tooltip helpers -> implementation
    //@{
    virtual geometry::Point2f MapToterrainCoordinates( int x, int y );
    //@}

    //! @name Common drawing tools -> hack
    //@{
    virtual void SetCurrentColor( float r, float g, float b, float a = 1 );
    //@}

    //! @name Drawing -> implementation
    //@{
    virtual void SetCurrentCursor( const QCursor& cursor );
    virtual std::string GetCurrentPass() const;
    virtual float LineWidth( float base ) const;
    virtual uint16_t StipplePattern( int factor = 1 ) const;
    virtual float Pixels( const geometry::Point2f& at = geometry::Point2f() ) const;

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

    //! @name Helpers
    //@{
    void CenterView();
    float ElevationAt( const geometry::Point2f& point ) const;
    bool IsInSelectionViewport( const geometry::Point2f& point ) const;
    void UndoRotations() const;
    //@}

    //! @name Qt events
    //@{
    virtual void dropEvent( QDropEvent* event );
    virtual void mousePressEvent( QMouseEvent* );
    virtual void mouseMoveEvent( QMouseEvent* );
    virtual void mouseReleaseEvent( QMouseEvent* );
    virtual void mouseDoubleClickEvent( QMouseEvent* );
    virtual void keyPressEvent( QKeyEvent* );
    virtual void enterEvent( QEvent* event );
    virtual void leaveEvent( QEvent* event );
    //@}

    //! @name OpenGL
    //@{
    virtual void ComputeData();
    virtual void Paint( const ViewFrustum& view );
    virtual void PaintLayers();
    virtual void UpdateGL();

    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL( int w, int h );
    //@}

private:
    //! @name Member data
    //@{
    kernel::DetectionMap& elevation_;
    EventStrategy_ABC& strategy_;
    T_LayersVector layers_;
    float zRatio_;
    unsigned int frame_;
    bool isInitialized_;
    ViewFrustum current_;
    float symbolSize_;
    //picking
    QPoint clickedPoint_;
    const geometry::Rectangle2f extent_;
    geometry::Rectangle2f viewport_;
    //@}
};

} //! namespace gui

#endif // __GL3DWidget_h_
