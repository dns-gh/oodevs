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

#include "Drawing_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Extension_ABC.h"

namespace svg
{
    class RenderingContext;
}

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Controller;
    class LocationProxy;
}

namespace gui
{
    class DrawingTemplate;
    class DrawingTypes;
    class SvgLocationDrawer;

// =============================================================================
/** @class  DrawerShape
    @brief  DrawerShape
*/
// Created: AGE 2006-08-31
// =============================================================================
class DrawerShape : public kernel::EntityImplementation< Drawing_ABC >
                  , public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawerShape( kernel::Controller& controller, unsigned long id, const DrawingTemplate& style, const QColor& color, kernel::LocationProxy& location );
             DrawerShape( kernel::Controller& controller, unsigned long id, xml::xistream& xis, const DrawingTypes& types, kernel::LocationProxy& location );
    virtual ~DrawerShape();
    //@}

    //! @name Accessors
    //@{
    virtual QColor GetColor() const;
    //@}

    //! @name Operations
    //@{
    void Translate( const geometry::Point2f& from, const geometry::Vector2f& translation, float precision );
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools, bool overlined ) const;
    virtual void Handle( kernel::Location_ABC& location );
    virtual void Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f&, const kernel::GlTools_ABC& tools ) const;

    virtual void ChangeColor( const QColor& color );
    virtual void Edit( ParametersLayer& parameters );
    virtual void Serialize( xml::xostream& xos ) const;

    virtual const kernel::Location_ABC& GetLocation() const;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void Create();
    virtual void Update();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DrawerShape( const DrawerShape& );            //!< Copy constructor
    DrawerShape& operator=( const DrawerShape& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadPoint( xml::xistream& xis );
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned long id_;
    const DrawingTemplate& style_;
    kernel::LocationProxy& location_;
    QColor color_;
    std::auto_ptr< SvgLocationDrawer > drawer_;
    //@}

private:
    //! @name Static Member
    //@{
    static unsigned long idManager_;
    //@}
};

}

#endif // __GUI_DrawerShape_h_
