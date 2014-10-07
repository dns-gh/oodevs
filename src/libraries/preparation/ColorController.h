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

    //! @name Operations
    //@{
    virtual void Add( const kernel::Entity_ABC& entity, const QColor& newColor, bool applyToSubordinates = true, bool force = false );
    virtual void Remove( const kernel::Entity_ABC& entity, bool applyToSubordinates = true, bool force = false );
    //@}

private:
    //! @name Helpers
    //@{
    void AddObjects( const kernel::Entity_ABC& entity, const QColor& newColor );
    void RemoveObjects( const kernel::Entity_ABC& entity );
    //@}
};

#endif // _ColorController_h
