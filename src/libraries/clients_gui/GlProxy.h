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
#include "clients_kernel/GlTools_ABC.h"

namespace urban
{
    class UrbanDecoration;
}

namespace gui
{
    class GlWidget;
    class Gl3dWidget;
    class Layer_ABC;
    class TooltipsLayer_ABC;

// =============================================================================
/** @class  GlProxy
    @brief  GlProxy
*/
// Created: AGE 2006-03-29
// =============================================================================
class GlProxy : public View_ABC
              , public kernel::GlTools_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GlProxy();
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

    void Register( Layer_ABC& layer );
    void Register( TooltipsLayer_ABC& layer );

    virtual void    CenterOn( const geometry::Point2f& point );
    virtual void    Zoom( float width );
    virtual float   GetAdaptiveZoomFactor() const;

    virtual std::pair< bool, bool > UnSelect() const;
    virtual void Select( bool, bool ) const;

    virtual bool ShouldDisplay( const std::string& name = std::string() ) const;
    virtual bool ShouldDisplay( const std::string& name, bool autoCondition ) const;

    virtual unsigned short StipplePattern( int factor = 1 ) const;
    virtual float Pixels( const geometry::Point2f& at = geometry::Point2f() ) const;
    virtual float Zoom() const;
    virtual void SetCurrentColor  ( float r, float g, float b, float a = 1 );
    virtual void SetCurrentCursor ( const QCursor& cursor );
    virtual std::auto_ptr< kernel::GlTooltip_ABC > CreateTooltip() const;
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
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GlProxy( const GlProxy& );            //!< Copy constructor
    GlProxy& operator=( const GlProxy& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< Layer_ABC* > T_Layers;
    typedef T_Layers::iterator       IT_Layers;
    //@}

private:
    //! @name Member data
    //@{
    View_ABC*    view_;
    kernel::GlTools_ABC* tools_;
    T_Layers layers_;
    TooltipsLayer_ABC* tooltipLayer_;
    //@}
};

}

#endif // __GlProxy_h_
