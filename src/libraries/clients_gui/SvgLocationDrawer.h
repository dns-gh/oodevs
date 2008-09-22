// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SvgLocationDrawer_h_
#define __SvgLocationDrawer_h_

#include "clients_kernel/LocationVisitor_ABC.h"

namespace kernel
{
    class Location_ABC;
    class GlTools_ABC;
}

namespace svg
{
    class RenderingContext;
}

namespace gui
{
    class DrawingTemplate;

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
    void Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools, const QColor& color, bool overlined );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SvgLocationDrawer( const SvgLocationDrawer& );            //!< Copy constructor
    SvgLocationDrawer& operator=( const SvgLocationDrawer& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& points );
    virtual void VisitPath   ( const geometry::Point2f& point, const T_PointVector& points );
    virtual void VisitCircle ( const geometry::Point2f& center, float radius );
    virtual void VisitPoint  ( const geometry::Point2f& point );
    //@}

    //! @name Helpers
    //@{
    void SetColor( const QColor& color );
    template< typename T > void DrawShape( const T& shape );
    void DrawOverlined( const T_PointVector& points );
    void DrawOverlined( const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< svg::RenderingContext > context_;
    geometry::Rectangle2f viewport_;
    const DrawingTemplate& style_;
    const kernel::GlTools_ABC* tools_;
    QColor color_;
    QColor complement_;
    bool overlined_;
    //@}
};

}

#endif // __SvgLocationDrawer_h_
