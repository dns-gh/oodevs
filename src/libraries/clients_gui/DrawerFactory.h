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

#include "DrawingFactory_ABC.h"

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
}

namespace gui
{
    class DrawingTypes;

// =============================================================================
/** @class  DrawerFactory
    @brief  Drawer factory
*/
// Created: SBO 2007-03-22
// =============================================================================
class DrawerFactory : public DrawingFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawerFactory( kernel::Controllers& controllers, const DrawingTypes& types, const kernel::CoordinateConverter_ABC& coordinateConverter, const kernel::EntityResolver_ABC& resolver );
    virtual ~DrawerFactory();
    //@}

    //! @name Operations
    //@{
    virtual Drawing_ABC* CreateShape( const DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity ) const;
    virtual Drawing_ABC* CreateShape( xml::xistream& xis ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const DrawingTypes& types_;
    const kernel::CoordinateConverter_ABC& coordinateConverter_;
    const kernel::EntityResolver_ABC& resolver_;
    //@}

private:
    //! @name Static members
    //@{
    static unsigned long idManager_;
    //@}
};

}

#endif // __DrawerFactory_h_
