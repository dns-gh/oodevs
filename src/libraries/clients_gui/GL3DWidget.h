// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Gl3dWidget_h_
#define __Gl3dWidget_h_

#include "GlToolsBase.h"
#include "View_ABC.h"
#include "SetGlOptions.h"
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
    class Layer;
    class PickingSelector;

// =============================================================================
/** @class  Gl3dWidget
    @brief  Gl3dWidget
*/
// Created: AGE 2006-03-28
// =============================================================================
class Gl3dWidget : private SetGlOptions
                 , public Widget3D
                 , public GlToolsBase
                 , public View_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Gl3dWidget( QWidget* pParent, kernel::Controllers& controllers, float width, float height,
                         kernel::DetectionMap& elevation, EventStrategy_ABC& strategy );
    virtual ~Gl3dWidget();
    //@}

    //! @name Operations
    //@{
    void Register( Layer& layer );
    void Unregister( Layer& layer );

    virtual unsigned short  StipplePattern( int factor = 1 ) const;
    virtual float           Pixels( const geometry::Point2f& at = geometry::Point2f() ) const;
    virtual float           LineWidth( float base ) const;
    virtual float           Zoom() const;
    virtual float           GetAdaptiveZoomFactor( bool bVariableSize = true ) const;

    virtual void DrawCross        ( const geometry::Point2f& at, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawLine         ( const geometry::Point2f& from, const geometry::Point2f& to ) const;
    virtual void DrawStippledLine ( const geometry::Point2f& from, const geometry::Point2f& to ) const;
    virtual void DrawLines        ( const T_PointVector& points ) const;
    virtual void DrawRectangle    ( const T_PointVector& points ) const;
    virtual void DrawPolygon      ( const T_PointVector& points ) const;
    virtual void DrawSelectedPolygon( const T_PointVector& points ) const;
    virtual void DrawDecoratedPolygon( const geometry::Polygon2f& polygon, const std::string& name, unsigned int fontHeight);
    virtual void DrawArrow        ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawArc          ( const geometry::Point2f& center, const geometry::Point2f& p1, const geometry::Point2f& p2 ) const;
    virtual void DrawCurvedArrow  ( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.2f, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawCircle       ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawDisc         ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawLife         ( const geometry::Point2f& center, float height, float factor = 1.f, bool fixedSize = true ) const;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where ) const;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where, const QFont& font ) const;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const std::string& style, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f, unsigned int direction = 0 ) const;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f, unsigned int direction = 0 ) const;
    virtual void DrawIcon         ( const char** xpm, const geometry::Point2f& where, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawImage        ( const QImage& image, const geometry::Point2f& where ) const;
    virtual void DrawCell         ( const geometry::Point2f& center ) const;
    virtual void DrawSvg          ( const std::string& svg, const geometry::Point2f& center, float ratio = 1.f, bool fixedSize = true ) const;
    virtual void DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, bool overlined, bool fixedSize = true ) const;
    virtual void DrawApp6SymbolFixedSize( const std::string&, const geometry::Point2f&, float factor, unsigned int direction ) const;
    virtual void DrawUnitSymbol( const std::string& symbol, const std::string& moveSymbol, const std::string& staticSymbol, bool isMoving, const geometry::Point2f& where, float factor, unsigned int direction ) const;

    virtual void CenterOn( const geometry::Point2f& point );
    virtual void Zoom( float width );
    virtual void CenterView();
    virtual void SetCurrentCursor( const QCursor& cursor );

    virtual void FillSelection( const geometry::Point2f& point, T_ObjectsPicking& selection );
    virtual void FillSelection( const geometry::Point2f& point, T_ObjectsPicking& selection, E_LayerTypes type );
    virtual void Picking();
    virtual void RenderPicking( const T_ObjectPicking& object );
    virtual bool IsPickingMode() const;
    virtual bool ShouldDisplay( E_LayerTypes type ) const;
    virtual QColor GetPickingColor() const;

    virtual void resizeGL( int w, int h );
    virtual void paintGL();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Gl3dWidget( const Gl3dWidget& );            //!< Copy constructor
    Gl3dWidget& operator=( const Gl3dWidget& ); //!< Assignment operator
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void mousePressEvent( QMouseEvent* );
    virtual void mouseMoveEvent ( QMouseEvent* );
    virtual void mouseReleaseEvent     ( QMouseEvent* );
    virtual void mouseDoubleClickEvent ( QMouseEvent* );
    virtual void keyPressEvent( QKeyEvent* );
    virtual void Paint( const ViewFrustum& view );
    virtual void initializeGL();
    float ElevationAt( const geometry::Point2f& point ) const;
    void UndoRotations() const;
    //@}

private:
    //! @name Helpers
    //@{
    bool IsInSelectionViewport( const geometry::Point2f& point ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< Layer* >      T_Layers;
    typedef T_Layers::iterator        IT_Layers;
    typedef T_Layers::const_iterator CIT_Layers;
    //@}

private:
    //! @name Member data
    //@{
    float width_;
    float height_;
    kernel::DetectionMap& elevation_;
    EventStrategy_ABC& strategy_;
    T_Layers layers_;
    float zRatio_;
    unsigned int frame_;
    bool isInitialized_;
    ViewFrustum current_;
    //picking
    std::auto_ptr< PickingSelector > pPickingSelector_;
    QPoint point_;
    float SymbolSize_;
    int windowHeight_;
    int windowWidth_;
    //@}
};

}

#endif // __Gl3dWidget_h_
