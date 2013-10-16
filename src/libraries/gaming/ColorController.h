// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ColorController_h_
#define __ColorController_h_

#include "clients_gui/ColorController.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Agent_ABC;
    class Formation_ABC;
    class Team_ABC;
    class Automat_ABC;
}

// =============================================================================
/** @class  ColorController
    @brief  Color controller
*/
// Created: LGY 2011-06-27
// =============================================================================
class ColorController : public gui::ColorController

{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ColorController( kernel::Controllers& controllers );
    virtual ~ColorController();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void UpdateHierarchies( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __ColorController_h_
