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

#include "game_asn/Messenger.h"
#include "clients_gui/DrawingFactory_ABC.h"

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
}

namespace gui
{
    class DrawingTypes;
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
             DrawingFactory( kernel::Controller& controller, const gui::DrawingTypes& types, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
    virtual ~DrawingFactory();
    //@}

    //! @name Operations
    //@{
    virtual gui::Drawing_ABC* CreateShape( const gui::DrawingTemplate& style, const QColor& color ) const;
    virtual gui::Drawing_ABC* CreateShape( xml::xistream& xis ) const;
    //@}

    //! @name Operations
    //@{
    gui::Drawing_ABC* CreateShape( const ASN1T_MsgShapeCreation& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawingFactory( const DrawingFactory& );            //!< Copy constructor
    DrawingFactory& operator=( const DrawingFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const gui::DrawingTypes& types_;
    Publisher_ABC& publisher_;
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

#endif // __DrawingFactory_h_
