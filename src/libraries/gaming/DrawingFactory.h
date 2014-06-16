// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingFactory_h_
#define __DrawingFactory_h_

#include "clients_gui/DrawingFactory_ABC.h"

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
}

namespace gui
{
    class DrawingTypes;
}

namespace sword
{
    class ShapeCreation;
}

class Publisher_ABC;

// =============================================================================
/** @class  DrawingFactory
    @brief  DrawingFactory
*/
// Created: SBO 2008-06-04
// =============================================================================
class DrawingFactory : public gui::DrawingFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawingFactory( kernel::Controllers& controllers, const gui::DrawingTypes& types, Publisher_ABC& publisher,
                             const kernel::CoordinateConverter_ABC& converter, const kernel::EntityResolver_ABC& resolver );
    virtual ~DrawingFactory();
    //@}

    //! @name Operations
    //@{
    virtual void CreateShape( const gui::DrawingTemplate& style, const QColor& color, const kernel::Entity_ABC* entity,
                              gui::E_Dash_style dashStyle, kernel::Location_ABC& location, const QString& name ) const;
    virtual void CreateShape( xml::xistream& xis, const kernel::Entity_ABC* entity ) const;
    //@}

    //! @name Operations
    //@{
    kernel::Drawing_ABC* CreateShape( const sword::ShapeCreation& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const gui::DrawingTypes& types_;
    Publisher_ABC& publisher_;
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::EntityResolver_ABC& resolver_;
    //@}
};

#endif // __DrawingFactory_h_
