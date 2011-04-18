// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gl3dWidget_h_
#define __Gl3dWidget_h_

#include "tools/WorldParameters.h"
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

namespace urban
{
    class UrbanDecoration;
}

namespace gui
{
    class Layer_ABC;

// =============================================================================
/** @class  Gl3dWidget
    @brief  Gl3dWidget
*/
// Created: AGE 2006-03-28
// =============================================================================
class Gl3dWidget : private tools::WorldParameters
                 , private SetGlOptions
                 , public Widget3D
                 , public GlToolsBase
                 , public View_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Gl3dWidget( QWidget* pParent, kernel::Controllers& controllers, const tools::ExerciseConfig& config, kernel::DetectionMap& elevation, EventStrategy_ABC& strategy );
    virtual ~Gl3dWidget();
    //@}

    //! @name Operations
    //@{
    void Register( Layer_ABC& layer );

    virtual unsigned short  StipplePattern( int factor = 1 ) const;
    virtual float           Pixels( const geometry::Point2f& at = geometry::Point2f() ) const;
    virtual float           Zoom() const;
    virtual float           GetAdaptiveZoomFactor() const;

    virtual void DrawCross        ( const geometry::Point2f& at, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawLine         ( const geometry::Point2f& from, const geometry::Point2f& to ) const;
    virtual void DrawLines        ( const T_PointVector& points ) const;
    virtual void DrawRectangle    ( const T_PointVector& points ) const;
    virtual void DrawConvexPolygon( const T_PointVector& points ) const;
    virtual void DrawConvexPolygon( const geometry::Polygon2f& polygon    ) const;
    virtual void DrawConvexPolygon( const T_PointVector& points, bool selected ) const;
    virtual void DrawDecoratedPolygon( const geometry::Polygon2f& polygon, const urban::UrbanDecoration* decoration, const std::string& name ) const;
    virtual void DrawArrow        ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawArc          ( const geometry::Point2f& center, const geometry::Point2f& p1, const geometry::Point2f& p2 ) const;
    virtual void DrawCurvedArrow  ( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.2f, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawCircle       ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawDisc         ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawLife         ( const geometry::Point2f& center, float height, float factor = 1.f ) const;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where ) const;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where, const QFont& font ) const;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const std::string& style, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f ) const;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f ) const;
    virtual void DrawIcon         ( const char** xpm, const geometry::Point2f& where, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawImage        ( const QImage& image, const geometry::Point2f& where ) const;
    virtual void DrawCell         ( const geometry::Point2f& center ) const;
    virtual void DrawSvg          ( const std::string& svg, const geometry::Point2f& center, float ratio = 1.f ) const;
    virtual void DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, bool overlined ) const;

    virtual void CenterOn( const geometry::Point2f& point );
    virtual void Zoom( float width );
    virtual void CenterView();
    virtual void SetCurrentCursor( const QCursor& cursor );
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

    //! @name Types
    //@{
    typedef std::vector< Layer_ABC* >  T_Layers;
    typedef T_Layers::const_iterator CIT_Layers;
    //@}

private:
    //! @name Member data
    //@{
    kernel::DetectionMap& elevation_;
    EventStrategy_ABC& strategy_;
    T_Layers layers_;
    float zRatio_;
    unsigned int frame_;
    bool isInitialized_;
    ViewFrustum current_;
    //@}
};

}

#endif // __Gl3dWidget_h_
