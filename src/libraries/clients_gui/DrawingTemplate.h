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

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class GlTools_ABC;
    class Location_ABC;
}

namespace svg
{
    class Node_ABC;
    class RenderingContext_ABC;
    class References;
    class TextRenderer;
}

namespace gui
{

// =============================================================================
/** @class  DrawingTemplate
    @brief  DrawingTemplate
    // $$$$ AGE 2006-09-01: eventuellement, mettre les références au niveau du panel
*/
// Created: AGE 2006-08-31
// =============================================================================
class DrawingTemplate
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
             DrawingTemplate( xml::xistream& xis, const QString& category, svg::TextRenderer& renderer );
    virtual ~DrawingTemplate();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    QString GetType() const;
    QString GetCode() const;
    QString GetDescription() const;
    QString GetCategory() const;
    QPixmap GetPixmap() const;
    Unit    GetUnit() const;
    void Draw( const T_PointVector& points, svg::RenderingContext_ABC& context, const kernel::GlTools_ABC& tools ) const;
    void Draw( const geometry::Point2f& point, svg::RenderingContext_ABC& context, const kernel::GlTools_ABC& tools ) const;
    void Serialize( xml::xostream& xos ) const;
    kernel::Location_ABC* CreateLocation() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawingTemplate( const DrawingTemplate& );            //!< Copy constructor
    DrawingTemplate& operator=( const DrawingTemplate& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void DrawSegment     ( svg::RenderingContext_ABC& context, const kernel::GlTools_ABC& tools, const geometry::Point2f& from, const geometry::Point2f& to ) const;
    void DrawStartMarker ( svg::RenderingContext_ABC& context, const kernel::GlTools_ABC& tools, const geometry::Point2f& at, const geometry::Point2f& towards ) const;
    void DrawMiddleMarker( svg::RenderingContext_ABC& context, const kernel::GlTools_ABC& tools, const geometry::Point2f& at, const geometry::Point2f& from, const geometry::Point2f& to ) const;
    void DrawEndMarker   ( svg::RenderingContext_ABC& context, const kernel::GlTools_ABC& tools, const geometry::Point2f& at, const geometry::Point2f& from ) const;
    void DrawMarker      ( svg::RenderingContext_ABC& context, const kernel::GlTools_ABC& tools, svg::Node_ABC& node, Unit unit, const geometry::Point2f& at, geometry::Vector2f direction ) const;
    void Align           ( geometry::Vector2f u ) const;
    void ReadMarker      ( xml::xistream& input, svg::Node_ABC*& node, Unit& unit );
    Unit ReadUnit        ( xml::xistream& input );
    float ComputeFactor  ( Unit, float base, const kernel::GlTools_ABC& tools ) const;
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
    svg::References* references_;
    svg::Node_ABC* line_;
    svg::Node_ABC* markerStart_;
    svg::Node_ABC* markerMiddle_;
    svg::Node_ABC* markerEnd_;
    svg::Node_ABC* marker_;

    Unit lineUnit, startUnit, middleUnit, endUnit, markerUnit;
    QPixmap linePixmap_, pointPixmap_, polygonPixmap_;
    //@}
};

}

#endif // __DrawerStyle_h_
