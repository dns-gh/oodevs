// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlProxy_h_
#define __GlProxy_h_

#include "View_ABC.h"
#include "GlTools_ABC.h"

namespace kernel
{
    class GraphicalEntity_ABC;
    class Logger_ABC;
}

namespace gui
{
    class GlWidget;
    class Gl3dWidget;
    class Layer;
    class TooltipsLayer_ABC;

// =============================================================================
/** @class  GlProxy
    @brief  GlProxy
*/
// Created: AGE 2006-03-29
// =============================================================================
class GlProxy : public View_ABC
              , public GlTools_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit GlProxy( kernel::Logger_ABC& logger );
    virtual ~GlProxy();
    //@}

    //! @name Operations
    //@{
    void RegisterTo( Gl3dWidget* newWidget );
    void RegisterTo( GlWidget* newWidget );
    void ChangeTo( Gl3dWidget* newWidget );
    void ChangeTo( GlWidget* newWidget );

    void Reset2d();
    void Reset3d();

    void Register( Layer& layer );
    void Register( TooltipsLayer_ABC& layer );
    void Unregister( Layer& layer );

    virtual void    CenterOn( const geometry::Point2f& point );
    virtual void    Zoom( float width );
    virtual float   GetAdaptiveZoomFactor( bool bVariableSize = true ) const;
    virtual geometry::Point2f GetCenter() const;

    virtual boost::tuple< bool, bool, bool > UnSelect() const;
    virtual void Select( bool, bool, bool ) const;

    virtual bool ShouldDisplay( const std::string& name = std::string() ) const;
    virtual bool ShouldDisplay( const std::string& name, bool autoCondition ) const;
    virtual bool ShouldDisplay( E_LayerTypes type ) const;
    virtual bool ShouldEdit( const kernel::GraphicalEntity_ABC& selectable ) const;

    virtual unsigned short StipplePattern( int factor = 1 ) const;
    virtual float Pixels( const geometry::Point2f& at = geometry::Point2f() ) const;
    virtual float LineWidth( float base ) const;
    virtual float Zoom() const;
    virtual void SetCurrentColor  ( float r, float g, float b, float a = 1 );
    virtual void SetCurrentCursor ( const QCursor& cursor );
    virtual std::unique_ptr< GlTooltip_ABC > CreateTooltip() const;
    virtual void DrawCross        ( const geometry::Point2f& at, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawLine         ( const geometry::Point2f& from, const geometry::Point2f& to, float width = 1. ) const;
    virtual void DrawStippledLine ( const geometry::Point2f& from, const geometry::Point2f& to ) const;
    virtual void DrawLines        ( const T_PointVector& points ) const;
    virtual void DrawRectangle    ( const T_PointVector& points ) const;
    virtual void DrawPolygon      ( const T_PointVector& points ) const;
    virtual void DrawSelectedPolygon( const T_PointVector& points ) const;
    virtual void DrawDecoratedPolygon( const geometry::Polygon2f& polygon, const std::string& name, unsigned int fontHeight );
    virtual void DrawArrow        ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawArc          ( const geometry::Point2f& center, const geometry::Point2f& p1, const geometry::Point2f& p2, float width = 1. ) const;
    virtual void DrawCurvedArrow  ( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.2f, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawCircle       ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawDisc         ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawHalfDisc     ( const geometry::Point2f& center, float angleDegrees, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawLife         ( const geometry::Point2f& center, float height, float factor = 1.f, bool fixedSize = true ) const;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where ) const;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where, const QFont& font ) const;
    virtual void DrawInfrastructureSymbol( const std::string& symbol, const geometry::Point2f& where, float factor, float thickness ) const;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const std::string& style, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f, unsigned int direction = 0 ) const;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f, unsigned int direction = 0 ) const;
    virtual void DrawIcon         ( const char** xpm, const geometry::Point2f& where, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawImage        ( const QImage& image, const geometry::Point2f& where ) const;
    virtual void DrawCell         ( const geometry::Point2f& center ) const;
    virtual void DrawSvg          ( const std::string& svg, const geometry::Point2f& center, float ratio = 1.f, bool fixedSize = true ) const;
    virtual void DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, bool overlined, bool fixedSize = true ) const;
    virtual void DrawApp6SymbolFixedSize( const std::string& symbol, const geometry::Point2f& where, float factor, unsigned int direction ) const;
    virtual void DrawUnitSymbol( const std::string& symbol, const std::string& moveSymbol, const std::string& staticSymbol, const std::string& level, bool isMoving, const geometry::Point2f& where, float factor, unsigned int direction, float width, float depth ) const;
    virtual void DrawUnitSymbolAndTail( const std::string& symbol, const std::string& level, const T_PointVector& points ) const;
    virtual void DrawShapeText( const QImage& image, const geometry::Point2f& where ) const;

    virtual void FillSelection( const geometry::Point2f& point, T_ObjectsPicking& selection );
    virtual void FillSelection( const geometry::Point2f& point, T_ObjectsPicking& selection, E_LayerTypes type );
    virtual void Picking();
    virtual void RenderPicking( const T_ObjectPicking& object );
    virtual bool IsPickingMode() const;
    virtual QColor GetPickingColor() const;
    virtual geometry::Point2f MapToterrainCoordinates( int x, int y );
    virtual bool HasFocus();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Layer* > T_Layers;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Logger_ABC& logger_;
    View_ABC* view_;
    GlTools_ABC* tools_;
    T_Layers layers_;
    TooltipsLayer_ABC* tooltipLayer_;
    //@}
};

}

#endif // __GlProxy_h_
