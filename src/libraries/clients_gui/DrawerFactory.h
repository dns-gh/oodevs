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
    class Controller;
}

namespace gui
{
    class DrawingTypes;

// =============================================================================
/** @class  DrawerFactory
    @brief  DrawerFactory
*/
// Created: SBO 2007-03-22
// =============================================================================
class DrawerFactory : public DrawingFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DrawerFactory( kernel::Controller& controller, const DrawingTypes& types );
    virtual ~DrawerFactory();
    //@}

    //! @name Operations
    //@{
    virtual Drawing_ABC* CreateShape( const DrawingTemplate& style, const QColor& color ) const;
    virtual Drawing_ABC* CreateShape( xml::xistream& xis ) const;
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
    kernel::Controller& controller_;
    const DrawingTypes& types_;
    //@}

private:
    //! @name Static members
    //@{
    static unsigned long idManager_;
    //@}
};

}

#endif // __DrawerFactory_h_
