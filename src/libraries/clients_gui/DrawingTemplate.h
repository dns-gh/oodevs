// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerStyle_h_
#define __DrawerStyle_h_

#include "clients_kernel/Types.h"
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class Location_ABC;
}

namespace gui
{
    class GLView_ABC;
}

namespace svg
{
    class Color;
    class Node_ABC;
    class RenderingContext_ABC;
    class References;
    class TextRenderer;
}

#define SYMBOL_PIXMAP_SIZE           64
#define SYMBOL_ICON_SIZE             128
#define SYMBOL_ICON_MARGIN           24
#define SYMBOL_FRAME_SIZE             4
#define SYMBOL_SCALE_RATIO_FOR_METER 0.1f

namespace gui
{

// =============================================================================
/** @class  DrawingTemplate
    @brief  DrawingTemplate
*/
// Created: AGE 2006-08-31
// =============================================================================
class DrawingTemplate : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    enum Unit
    {
        eMeter,
        ePixel,
        ePercent,
        eNone
    };
    //@}

    //! @name Constructors/Destructor
    //@{
             DrawingTemplate( xml::xistream& xis, const QString& category, const QString& id, svg::TextRenderer& renderer );
    virtual ~DrawingTemplate();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    QString GetType() const;
    QString GetCode() const;
    QString GetDescription() const;
    QString GetCategory() const;
    QString GetId() const;
    Unit    GetUnit() const;
    const QPixmap& GetPixmap() const;
    const QPixmap& GetSamplePixmap() const;
    void GenerateSamplePixmap( const GLView_ABC& tools, float r = -1, float g = -1, float b = -1, float sampleMarkerRatio = 1.f );

    void Draw( const T_PointVector& points, svg::RenderingContext_ABC& context, const GLView_ABC& tools, float zoom = 1.f ) const;
    void Draw( const geometry::Point2f& point, svg::RenderingContext_ABC& context, const GLView_ABC& tools, float zoom = 1.f ) const;
    void Serialize( xml::xostream& xos ) const;
    kernel::Location_ABC* CreateLocation() const;
    //@}

private:
    //! @name Helpers
    //@{
    void DrawSegment     ( svg::RenderingContext_ABC& context, const GLView_ABC& tools, const geometry::Point2f& from, const geometry::Point2f& to ) const;
    void DrawStartMarker ( svg::RenderingContext_ABC& context, const GLView_ABC& tools, const geometry::Point2f& at, const geometry::Point2f& towards, float zoom ) const;
    void DrawMiddleMarker( svg::RenderingContext_ABC& context, const GLView_ABC& tools, const geometry::Point2f& at, const geometry::Point2f& from, const geometry::Point2f& to, float zoom, const svg::Node_ABC& marker, const Unit& unit ) const;
    void DrawEndMarker   ( svg::RenderingContext_ABC& context, const GLView_ABC& tools, const geometry::Point2f& at, const geometry::Point2f& from, float zoom ) const;
    void DrawMarker      ( svg::RenderingContext_ABC& context, const GLView_ABC& tools, const svg::Node_ABC& node, Unit unit, const geometry::Point2f& at, geometry::Vector2f direction, float zoom ) const;
    void Align           ( geometry::Vector2f u ) const;
    void ReadMarker      ( xml::xistream& input, svg::Node_ABC*& node, Unit& unit );
    Unit ReadUnit        ( xml::xistream& input );
    float ComputeFactor  ( Unit, float base, const GLView_ABC& tools ) const;
    //@}

    //! @name Sample helper
    //@{
    void DrawSample( const GLView_ABC& tools );
    void DrawOnPoint( const GLView_ABC& tools );
    void DrawOnLine( const GLView_ABC& tools );
    void DrawOnPolygon( const GLView_ABC& tools );
    void DrawOnCircle( const GLView_ABC& tools );
    void DrawOnCurve( const GLView_ABC& tools );
    void DrawItem( const T_PointVector& points, const GLView_ABC& tools );
    //@}

private:
    //! @name Member data
    //@{
    const QString category_;
    svg::TextRenderer& renderer_;
    QString name_;
    QString type_;
    QString code_;
    QString description_;
    QString id_;
    svg::References* references_;
    svg::Node_ABC* line_;
    svg::Node_ABC* markerStart_;
    svg::Node_ABC* markerMiddle_;
    svg::Node_ABC* markerEnd_;
    svg::Node_ABC* markerCorner_;
    svg::Node_ABC* marker_;

    Unit lineUnit;
    Unit startUnit;
    Unit middleUnit;
    Unit endUnit;
    Unit cornerUnit;
    Unit markerUnit;

    QPixmap linePixmap_;
    QPixmap pointPixmap_;
    QPixmap polygonPixmap_;
    QPixmap circlePixmap_;
    QPixmap samplePixmap_;
    QPixmap curvePixmap_;
    QPixmap rectanglePixmap_;
    QPixmap textPixmap_;
    std::unique_ptr< svg::Color > sampleColor_;
    float sampleMarkerRatio_;
    bool isDrawingSample_;
    //@}
};

}

#endif // __DrawerStyle_h_
