// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerFactory_h_
#define __DrawerFactory_h_

namespace kernel
{
    class GlTools_ABC;
    class Controllers;
}

namespace xml
{
    class xistream;
}

namespace svg
{
    class TextRenderer;
}

namespace gui
{
    class DrawerCategory;
    class DrawerShape;
    class DrawerStyle;
    class DrawerModel;

// =============================================================================
/** @class  DrawerFactory
    @brief  DrawerFactory
*/
// Created: SBO 2007-03-22
// =============================================================================
class DrawerFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawerFactory( QWidget* parent, kernel::GlTools_ABC& tools, kernel::Controllers& controllers );
    virtual ~DrawerFactory();
    //@}

    //! @name Operations
    //@{
    DrawerCategory* CreateCategory( xml::xistream& xis ) const;
    DrawerShape* CreateShape( const DrawerStyle& style, const QColor& color ) const;
    DrawerShape* CreateShape( xml::xistream& xis, const DrawerModel& model ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawerFactory( const DrawerFactory& );            //!< Copy constructor
    DrawerFactory& operator=( const DrawerFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    QWidget* parent_;
    kernel::Controllers& controllers_;
    kernel::GlTools_ABC& tools_;
    svg::TextRenderer& renderer_;
    //@}
};

}

#endif // __DrawerFactory_h_
