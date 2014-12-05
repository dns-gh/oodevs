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
    explicit ColorController( kernel::Controllers& controllers );
    virtual ~ColorController();

    virtual void Add( const kernel::Entity_ABC& entity, const QColor& newColor, bool applyToSubordinates = true, bool force = false );
    virtual void Remove( const kernel::Entity_ABC& entity );

private:
    virtual bool ApplyColor( const kernel::Color_ABC& color );
};

#endif // _ColorController_h
