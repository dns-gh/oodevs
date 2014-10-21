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

#include "GLView_ABC.h"
#include "GLView_ABC.h"
#include "LayersHelpers.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class EntityResolver_ABC;
    class GraphicalEntity_ABC;
    class StaticModel;
    class Options;
    class OptionsController;
    class Profile_ABC;
}

class Lighting_ABC;

namespace gui
{
    class ColorStrategy_ABC;
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
class GlProxy : public GLView_ABC
              , public tools::Observer_ABC
              , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GlProxy( kernel::Controllers& controllers,
                      const kernel::Profile_ABC& profile,
                      const kernel::StaticModel& staticModel,
                      const kernel::EntityResolver_ABC& model,
                      const std::shared_ptr< Lighting_ABC >& lighting );
    virtual ~GlProxy();
    //@}

    //! @name OptionsObserver implementation
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

    //! @name Proxy
    //@{
    void Purge();
    void UpdateLayerOrder( kernel::Options& options );

    void SetWidget3D( const std::shared_ptr< Gl3dWidget >& newWidget );
    void SetWidget2D( const std::shared_ptr< GlWidget >& newWidget );

    void ChangeTo( const std::shared_ptr< GlWidget >& newWidget );
    void ChangeTo( const std::shared_ptr< Gl3dWidget >& newWidget );
    //@}

    //! @name Layers
    //@{
    void SetTooltipsLayer( const std::shared_ptr< TooltipsLayer_ABC >& layer );
    void AddLayers( const T_LayersVector& layers );
    void RemoveLayer( const T_Layer& layer );
    void ApplyToLayers( const T_LayerFunctor& functor ) const;

    bool MoveBelow( const T_Layer& lhs, const T_Layer& rhs );
    bool ShouldEdit( const kernel::GraphicalEntity_ABC& selectable ) const; // layer related, could be rename IsInAReadOnlyLayer
    //@}

    //! @name Options
    //@{
    virtual GLOptions& GetOptions();
    virtual const GLOptions& GetOptions() const;
    //@}

    //! @name Viewport
    //@{
    virtual void CenterOn( const geometry::Point2f& point );
    virtual geometry::Point2f GetCenter() const;

    virtual void Zoom( float width );
    virtual float Zoom() const;
    virtual float GetAdaptiveZoomFactor( bool bVariableSize = true ) const;
    //@}

    //! @name Picking
    //@{
    virtual bool ShouldDisplay( E_LayerTypes type ) const; // picking related
    virtual void FillSelection( const geometry::Point2f& point,
                                T_ObjectsPicking& selection,
                                const boost::optional< E_LayerTypes >& type );
    virtual void Picking();
    virtual void RenderPicking( const T_ObjectPicking& object );
    virtual bool IsPickingMode() const;
    virtual QColor GetPickingColor() const;
    //@}

    //! @name Tooltip
    //@{
    virtual std::unique_ptr< GlTooltip_ABC > CreateTooltip() const;
    virtual geometry::Point2f MapToterrainCoordinates( int x, int y );
    virtual bool HasFocus();
    //@}

    //! @name Drawing
    //@{
    virtual std::string GetCurrentPass() const;
    virtual unsigned short StipplePattern( int factor = 1 ) const;
    virtual float Pixels( const geometry::Point2f& at = geometry::Point2f() ) const;
    virtual float LineWidth( float base ) const;

    virtual float GetCurrentAlpha () const;
    virtual void SetCurrentColor  ( float r, float g, float b, float a = 1 );
    virtual void SetCurrentCursor ( const QCursor& cursor );

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
    virtual void DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, bool overlined, float pointSize, bool fixedSize = true ) const;
    virtual void DrawApp6SymbolFixedSize( const std::string& symbol, const geometry::Point2f& where, float factor, unsigned int direction ) const;
    virtual void DrawUnitSymbol( const std::string& symbol, const std::string& moveSymbol, const std::string& staticSymbol, const std::string& level, bool isMoving, const geometry::Point2f& where, float factor, unsigned int direction, float width, float depth ) const;
    virtual void DrawUnitSymbolAndTail( const std::string& symbol, const std::string& level, const T_PointVector& points ) const;
    virtual void DrawShapeText( const QImage& image, const geometry::Point2f& where ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OptionsController& optionsController_;
    std::shared_ptr< TooltipsLayer_ABC > tooltipLayer_;
    std::shared_ptr< GLView_ABC > view_;
    std::shared_ptr< GlWidget > widget2d_;
    std::shared_ptr< Gl3dWidget > widget3d_;
    std::unique_ptr< GLOptions > options_;
    T_LayersVector layers_;
    //@}
};

}

#endif // __GlProxy_h_
