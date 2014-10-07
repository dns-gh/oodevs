// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _ColorController_h
#define _ColorController_h

#include "clients_gui/ColorController.h"

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  ColorController
    @brief  Color controller
*/
// Created: LGY 2011-06-23
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
    virtual void ChangeColor( const kernel::Entity_ABC& entity );
    //@}
};

#endif // _ColorController_h