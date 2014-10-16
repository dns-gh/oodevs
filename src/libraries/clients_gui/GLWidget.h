// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlWidget_h_
#define __GlWidget_h_

#include "SetGlOptions.h"
#include "GLView_ABC.h"
#include "GlToolsBase.h"
#include "MapWidget_ABC.h"
#include "MT_Tools/MT_Profiler.h"
#include <graphics/MapWidget.h>

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class DrawingTypes;
    class IconLayout;
    class GlRenderPass_ABC;
    class PickingSelector;
    class FrameCounter;

// =============================================================================
/** @class  GlWidget
    @brief  GlWidget
*/
// Created: AGE 2006-03-15
// =============================================================================
class GlWidget : private SetGlOptions
               , public MapWidget
               , public MapWidget_ABC
               , public GlToolsBase
{
public:
    //! @name Constructors/Destructor
    //@{
             GlWidget( QWidget* pParent,
                       kernel::Controllers& controllers,
                       float width,
                       float height,
                       IconLayout& iconLayout,
                       const DrawingTypes& drawingTypes );
    virtual ~GlWidget();
    //@}

    //! @name Operations
    //@{
    void SetPassOrder( const std::string& names );
    void AddPass( GlRenderPass_ABC& pass );
    void RemovePass( GlRenderPass_ABC& pass );

    std::string GetCurrentPass() const;
    virtual void PaintLayers();
    virtual unsigned int Width() const;
    virtual unsigned int Height() const;
    virtual void wheelEvent( QWheelEvent* event );
    virtual geometry::Rectangle2f Viewport() const;
    //@}

    //! @name Operations
    //@{
    virtual unsigned short StipplePattern( int factor = 1 ) const;
    virtual float LineWidth( float base ) const;
    virtual float Pixels( const geometry::Point2f& at = geometry::Point2f() ) const;
    virtual float Zoom() const;
    virtual float GetAdaptiveZoomFactor( bool bVariableSize = true ) const;
    virtual float GetActualZoomFactor() const;

    virtual void DrawCross        ( const geometry::Point2f& at, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawLine         ( const geometry::Point2f& from, const geometry::Point2f& to, float width = 1. ) const;
    virtual void DrawStippledLine ( const geometry::Point2f& from, const geometry::Point2f& to ) const;
    virtual void DrawLines        ( const T_PointVector& points ) const;
    virtual void DrawRectangle    ( const T_PointVector& points ) const;
    virtual void DrawPolygon      ( const T_PointVector& points ) const;
    virtual void DrawSelectedPolygon( const T_PointVector& points ) const;
    virtual void DrawDecoratedPolygon( const kernel::T_TessellatedPolygon& polygon, const T_PointVector& vertices,
                                       const geometry::Point2f& center, const std::string& name, unsigned int fontHeight );
    virtual void DrawArrow        ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawCurvedArrow  ( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.2f, float size = -1.f, E_Unit unit = meters ) const;    
    virtual void DrawArc          ( const geometry::Point2f& center, const geometry::Point2f& p1, const geometry::Point2f& p2, float width = 1. ) const;
    virtual void DrawCircle       ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawDisc         ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawHalfDisc     ( const geometry::Point2f& center, float angleDegrees, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawLife         ( const geometry::Point2f& center, float height, float factor = 1.f, bool fixedSize = true ) const;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where, const QFont& font ) const;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where ) const;
    virtual void DrawInfrastructureSymbol( const std::string& symbol, const geometry::Point2f& where, float factor, float thickness ) const;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const std::string& style, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f, unsigned int direction = 0 ) const;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f, unsigned int direction = 0 ) const;
    virtual void DrawApp6SymbolFixedSize( const std::string& symbol, const geometry::Point2f& where, float factor, unsigned int direction ) const;
    virtual void DrawApp6SymbolScaledSize( const std::string& symbol, const geometry::Point2f& where, float factor, unsigned int direction, float width, float depth ) const;
    virtual void DrawUnitSymbol( const std::string& symbol, const std::string& moveSymbol, const std::string& staticSymbol, const std::string& level, bool isMoving, const geometry::Point2f& where, float factor, unsigned int direction, float width, float depth ) const;
    virtual void DrawUnitSymbolAndTail( const std::string& symbol, const std::string& level, const T_PointVector& points ) const;
    virtual void DrawIcon         ( const char** xpm, const geometry::Point2f& where, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawImage        ( const QImage& image, const geometry::Point2f& where ) const;
    virtual void DrawCell         ( const geometry::Point2f& center ) const;
    virtual void DrawSvg          ( const std::string& svg, const geometry::Point2f& center, float ratio = 1.f, bool fixedSize = true ) const;
    virtual void DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, bool overlined, float pointSize, bool fixedSize = true ) const;
    virtual void DrawShapeText( const QImage& image, const geometry::Point2f& where ) const;

    virtual void FillSelection( const geometry::Point2f& point, T_ObjectsPicking& selection,
            const boost::optional< E_LayerTypes >& type );
    virtual void Picking();
    virtual void RenderPicking( const T_ObjectPicking& object );
    virtual bool IsPickingMode() const;
    virtual bool ShouldDisplay( E_LayerTypes type ) const;
    virtual QColor GetPickingColor() const;

    virtual void CenterOn( const geometry::Point2f& point );
    virtual void Zoom( float width );
    virtual void SetCurrentCursor( const QCursor& cursor );
    virtual geometry::Point2f GetCenter() const;
    virtual geometry::Point2f MapToterrainCoordinates( int x, int y );
    virtual bool HasFocus();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Helpers
    //@{
    float ComputeZoomFactor( float& factor, bool bVariableSize = true ) const;
    void DrawApp6Symbol( const std::string& symbol, const std::string& style, const geometry::Point2f& where,
        float expectedWidth, const geometry::Rectangle2f& viewport, unsigned int printWidth, unsigned int printHeight,
        unsigned int angle, float xFactor, float yFactor, float svgDeltaX = -20, float svgDeltaY = -80, bool checkAlpha = true ) const;
    void DrawTail( const T_PointVector& points, float width ) const;
    void DrawDiscPart( const geometry::Point2f& center, int glList, float angleDegrees, float radius, GLView_ABC::E_Unit unit ) const;
    float Radius( float radius, E_Unit unit ) const;
    //@}

private:
    //! @name Types
    //@{
    struct passLess
    {
        explicit passLess( const std::string& names ) : names_( names ){}
        bool operator()( GlRenderPass_ABC* lhs, GlRenderPass_ABC* rhs ) const;
        std::string names_;
    };
    typedef std::multiset< GlRenderPass_ABC*, passLess > T_RenderPasses;

    struct sFontLess : public std::binary_function< QFont, QFont, bool >
    {
        bool operator()( const QFont& lhs, const QFont& rhs ) const
        {
            return lhs.toString() < rhs.toString();
        }
    };

    typedef std::map< QFont, int, sFontLess > T_Fonts;
    typedef std::vector< GLdouble > T_Points;
    typedef std::vector< T_Points > T_Geometry;
    //@}

    //! @name Helpers
    //@{
    virtual void paintGL();
    virtual void PickGL();
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    virtual void updateGL();
    unsigned int GenerateCircle();
    unsigned int GenerateHalfCircle();
    void UpdateStipple() const;
    void DrawTextLabel( const std::string& message, const geometry::Point2f& where, int baseSize = 12);

    void RenderPass( GlRenderPass_ABC& pass );
    void PickingPass( GlRenderPass_ABC& pass );
    bool IsInSelectionViewport( const geometry::Point2f& point ) const;

    virtual void keyPressEvent( QKeyEvent* );
    //@}

private:
    //! @name Member data
    //@{
    const float baseWidth_;
    int windowHeight_;
    int windowWidth_;
    unsigned int circle_;
    unsigned int halfCircle_;
    int minVisuScale_;
    int maxVisuScale_;
    geometry::Rectangle2f viewport_;
    unsigned int frame_;
    IconLayout& iconLayout_;
    T_RenderPasses passes_;
    std::string currentPass_;
    QFont currentFont_;
    bool bMulti_;
    float SymbolSize_;
    std::unique_ptr< PickingSelector > pPickingSelector_;
    std::unique_ptr< FrameCounter > fps_;
    QPoint point_;
    //@}
};

}

#endif // __GlWidget_h_
