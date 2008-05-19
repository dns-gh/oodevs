// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GUI_DrawerShape_h_
#define __GUI_DrawerShape_h_

namespace svg
{
    class RenderingContext;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class Controller;
}

namespace gui
{
    class DrawerStyle;

// =============================================================================
/** @class  DrawerShape
    @brief  DrawerShape
*/
// Created: AGE 2006-08-31
// =============================================================================
class DrawerShape
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawerShape( kernel::Controller& controller, const DrawerStyle& style, const QColor& color );
             DrawerShape( kernel::Controller& controller, const DrawerStyle& style, xml::xistream& xis );
    virtual ~DrawerShape();
    //@}

    //! @name Operations
    //@{
    virtual void Create();

    virtual void PopPoint();
    virtual void AddPoint( const geometry::Point2f& point );

    virtual void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    bool IsAt( const geometry::Point2f& point, float precision = 100.f ) const;
    virtual void Draw( const geometry::Rectangle2f& viewport, bool overlined ) const;
    virtual void Draw( const geometry::Rectangle2f& viewport, const QColor& color, bool overlined ) const;

    void ChangeColor( const QColor& color );

    void ReadPoint( xml::xistream& xis );
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DrawerShape( const DrawerShape& );            //!< Copy constructor
    DrawerShape& operator=( const DrawerShape& ); //!< Assignement operator
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const DrawerStyle& style_;
    svg::RenderingContext* context_;
    QColor color_;
    QColor complement_;
    T_PointVector points_;
    //@}
};

}

#endif // __GUI_DrawerShape_h_
