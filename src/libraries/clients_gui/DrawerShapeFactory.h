// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerShapeFactory_h_
#define __DrawerShapeFactory_h_

namespace gui
{
    class DrawerShape;
    class DrawerStyle;

// =============================================================================
/** @class  DrawerShapeFactory
    @brief  DrawerShapeFactory
*/
// Created: SBO 2007-03-07
// =============================================================================
class DrawerShapeFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawerShapeFactory();
    virtual ~DrawerShapeFactory();
    //@}

    //! @name Operations
    //@{
    DrawerShape* CreateShape( const DrawerStyle& style, const QColor& color ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawerShapeFactory( const DrawerShapeFactory& );            //!< Copy constructor
    DrawerShapeFactory& operator=( const DrawerShapeFactory& ); //!< Assignment operator
    //@}
};

}

#endif // __DrawerShapeFactory_h_
