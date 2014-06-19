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
             DrawerFactory( kernel::Controllers& controllers, const DrawingTypes& types, const kernel::CoordinateConverter_ABC& coordinateConverter );
    virtual ~DrawerFactory();
    //@}

    //! @name Operations
    //@{
    virtual void CreateShape( const DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity,
                              E_Dash_style dashStyle, kernel::Location_ABC& location, const QString& name ) const;
    virtual void CreateShape( xml::xistream& xis, const kernel::Entity_ABC* entity ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const DrawingTypes& types_;
    const kernel::CoordinateConverter_ABC& coordinateConverter_;
    //@}

private:
    //! @name Static members
    //@{
    static unsigned long idManager_;
    //@}
};

}

#endif // __DrawerFactory_h_
