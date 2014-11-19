// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GL2DWidget_h_
#define __GL2DWidget_h_

#include "GLView_ABC.h"
#include "GLViewBase.h"
#include "MT_Tools/MT_Profiler.h"
#include <graphics/MapWidget.h>

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class IconLayout;
    class GlRenderPass_ABC;
    class FrameCounter;

// =============================================================================
/** @class  GL2DWidget
    @brief  GL2DWidget
*/
// Created: AGE 2006-03-15
// =============================================================================
class GL2DWidget : public GLViewBase
                 , public MapWidget
{
public:
    //! @name Constructors/Destructor
    //@{
             GL2DWidget( QWidget* parentWidget,
                         GLView_ABC& parentView,
                         float width,
                         float height,
                         const std::shared_ptr< IconLayout >& iconLayout,
                         QGLWidget* shareWidget = 0 );
    virtual ~GL2DWidget();
    //@}

    //! @name Passes
    //@{
    void SetPassOrder( const std::string& names );
    void AddPass( GlRenderPass_ABC& pass );
    void RemovePass( GlRenderPass_ABC& pass );
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

    //! @name Tooltips helpers -> implementation
    //@{
    virtual geometry::Point2f MapToterrainCoordinates( int x, int y );
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
                           float factore = 1.f,
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
    float ComputeZoomFactor( float& factor, bool bVariableSize = true ) const;
    void DrawApp6( const std::string& symbol,
                                const geometry::Point2f& where,
                                float expectedWidth,
                                const geometry::Rectangle2f& viewport,
                                unsigned int printWidth,
                                unsigned int printHeight,
                                unsigned int direction,
                                float xFactor = 1.,
                                float yFactor = 1.,
                                float svgDeltaX = -20,
                                float svgDeltaY = -80 ) const;
    void DrawApp6SymbolScaledSize( const std::string& symbol,
                                   const geometry::Point2f& where,
                                   float factor,
                                   unsigned int direction,
                                   float width,
                                   float depth ) const;
    void DrawTail( const T_PointVector& points, float width ) const;
    void DrawTextLabel( const std::string& message,
                        const geometry::Point2f& where,
                        int baseSize = 12 );
    bool IsInSelectionViewport( const geometry::Point2f& point ) const;
    void RenderPass( GlRenderPass_ABC& pass );
    void UpdateStipple() const;
    //@}

    //! @name Qt events
    //@{
    virtual void dropEvent( QDropEvent* event );
    virtual void keyPressEvent( QKeyEvent* );
    virtual void mousePressEvent( QMouseEvent* );
    virtual void enterEvent( QEvent* event );
    virtual void leaveEvent( QEvent* event );
    //@}

    //! @name OpenGL
    //@{
    virtual void ComputeData();
    virtual void PaintLayers();
    virtual void UpdateGL();
    virtual void paintGL();
    virtual void PickGL();
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    //@}

private:
    //! @name Types
    //@{
    struct passLess
    {
        explicit passLess( const std::string& names ) : names_( names ) {}
        bool operator()( GlRenderPass_ABC* lhs, GlRenderPass_ABC* rhs ) const;
        std::string names_;
    };
    typedef std::multiset< GlRenderPass_ABC*, passLess > T_RenderPasses;
    //@}

private:
    //! @name Member data
    //@{
    unsigned circle_;
    unsigned halfCircle_;
    int minVisuScale_;
    int maxVisuScale_;
    unsigned frame_;
    std::shared_ptr< IconLayout > iconLayout_;
    T_RenderPasses passes_;
    std::string currentPass_;
    QFont currentFont_;
    bool hasMultiTexturing_;
    std::unique_ptr< FrameCounter > fps_;
    QPoint clickedPoint_;
    float adaptiveZoom_;
    float fixedZoom_;
    float pixels_;
    float symbolSize_;
    bool drawUrbanLabel_;
    //@}
};

} //! namespace gui

#endif // __GL2DWidget_h_
