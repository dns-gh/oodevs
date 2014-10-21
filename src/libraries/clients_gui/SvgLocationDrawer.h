// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SvgLocationDrawer_h_
#define __SvgLocationDrawer_h_

#include "DashStyle.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include <svgl/DashArray.h>

namespace kernel
{
    class Location_ABC;
}

namespace svg
{
    class RenderingContext;
}

namespace gui
{
    class DrawingTemplate;
    class GLView_ABC;

// =============================================================================
/** @class  SvgLocationDrawer
    @brief  SvgLocationDrawer
*/
// Created: SBO 2008-05-30
// =============================================================================
class SvgLocationDrawer : public kernel::LocationVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SvgLocationDrawer( const DrawingTemplate& style );
    virtual ~SvgLocationDrawer();
    //@}

    //! @name Operations
    //@{
    void Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport,
               const GLView_ABC& tools, const QColor& color, bool overlined, E_Dash_style dashStyle, float zoom = 1.f );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SvgLocationDrawer( const SvgLocationDrawer& );            //!< Copy constructor
    SvgLocationDrawer& operator=( const SvgLocationDrawer& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void VisitLines    ( const T_PointVector& points );
    virtual void VisitRectangle( const T_PointVector& points );
    virtual void VisitPolygon  ( const T_PointVector& points );
    virtual void VisitPath     ( const geometry::Point2f& point, const T_PointVector& points );
    virtual void VisitCircle   ( const geometry::Point2f& center, float radius );
    virtual void VisitCurve    ( const T_PointVector& points );
    virtual void VisitPoint    ( const geometry::Point2f& point );
    virtual void VisitText     ( const QString& text, const QFont& font, const geometry::Point2f& point );
    //@}

    //! @name Helpers
    //@{
    static void GenerateCircle();
    template< typename T > void DrawShape( const T& shape );
    void DrawOverlined( const T_PointVector& points );
    void DrawOverlined( const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    static std::vector< geometry::Vector2f > circle_;
    std::unique_ptr< svg::RenderingContext > context_;
    geometry::Rectangle2f viewport_;
    const DrawingTemplate& style_;
    const GLView_ABC* tools_;
    QColor color_;
    QColor complement_;
    float zoom_;
    bool overlined_;
    bool colorChanged_;
    E_Dash_style dashStyle_;
    svg::DashArray dashed_;
    svg::DashArray dashDot_;
    QImage imageText_;
    QString text_;
    QFont font_;
    //@}
};

}

#endif // __SvgLocationDrawer_h_
