// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingsModel_h_
#define __DrawingsModel_h_

#include "clients_gui/DrawerModel.h"
#include "protocol/MessengerSenders.h"

class DrawingFactory;

// =============================================================================
/** @class  DrawingsModel
    @brief  DrawingsModel
*/
// Created: SBO 2008-06-04
// =============================================================================
class DrawingsModel : public gui::DrawerModel
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawingsModel( kernel::Controllers& controllers, const DrawingFactory& factory, const kernel::EntityResolver_ABC& resolver );
    virtual ~DrawingsModel();
    //@}

    //! @name Operations
    //@{
    void Create( const sword::ShapeCreation& message );
    void Update( const sword::ShapeUpdate& message );
    void Delete( const sword::ShapeDestruction& message );
    //@}

private:
    //! @name Member data
    //@{
    const DrawingFactory& factory_;
    //@}
};

#endif // __DrawingsModel_h_
