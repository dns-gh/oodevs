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

namespace xml
{
    class xistream;
}

namespace kernel
{
    class GlTools_ABC;
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
/** @class  DrawerStyle
    @brief  DrawerStyle
    // $$$$ AGE 2006-09-01: eventuellement, mettre les références au niveau du panel
*/
// Created: AGE 2006-08-31
// =============================================================================
class DrawerStyle
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawerStyle( xml::xistream& input, kernel::GlTools_ABC& tools, svg::TextRenderer& renderer );
    virtual ~DrawerStyle();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    QString GetType() const;
    QString GetDescription() const;
    void Draw( const T_PointVector& points, svg::RenderingContext_ABC& context ) const;
    void Draw( const geometry::Point2f& point, svg::RenderingContext_ABC& context ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DrawerStyle( const DrawerStyle& );            //!< Copy constructor
    DrawerStyle& operator=( const DrawerStyle& ); //!< Assignement operator
    //@}
    
    //! @name Types
    //@{
    // $$$$ AGE 2006-09-01: 
    enum Unit
    {
        eMeter,
        ePixel,
        ePercent,
    };
    //@}

    //! @name Helpers
    //@{
    void DrawSegment     ( svg::RenderingContext_ABC& context, const geometry::Point2f& from, const geometry::Point2f& to ) const;
    void DrawStartMarker ( svg::RenderingContext_ABC& context, const geometry::Point2f& at, const geometry::Point2f& towards ) const;
    void DrawMiddleMarker( svg::RenderingContext_ABC& context, const geometry::Point2f& at, const geometry::Point2f& from, const geometry::Point2f& to ) const;
    void DrawEndMarker   ( svg::RenderingContext_ABC& context, const geometry::Point2f& at, const geometry::Point2f& from ) const;
    void DrawMarker      ( svg::RenderingContext_ABC& context, svg::Node_ABC& node, Unit unit, const geometry::Point2f& at, geometry::Vector2f direction ) const;
    void Align           ( geometry::Vector2f u ) const;
    void ReadMarker      ( xml::xistream& input, svg::Node_ABC*& node, Unit& unit );
    Unit ReadUnit        ( xml::xistream& input );
    float ComputeFactor  ( Unit, float base ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::GlTools_ABC& tools_;
    svg::TextRenderer& renderer_;
    QString name_;
    QString type_;
    QString description_;
    svg::References* references_;
    svg::Node_ABC* line_;
    svg::Node_ABC* markerStart_;
    svg::Node_ABC* markerMiddle_;
    svg::Node_ABC* markerEnd_;

    Unit lineUnit, startUnit, middleUnit, endUnit;
    //@}
};

}

#endif // __DrawerStyle_h_
