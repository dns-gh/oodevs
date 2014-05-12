// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerLayer_h_
#define __DrawerLayer_h_

#include "clients_gui/DrawerLayer.h"

// =============================================================================
/** @class  DrawerLayer
    @brief  DrawerLayer
*/
// Created: LGY 2014-05-12
// =============================================================================
class DrawerLayer : public gui::DrawerLayer
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawerLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                          gui::ParametersLayer& parameters, gui::View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~DrawerLayer();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Drawing_ABC& shape, kernel::ContextMenu& menu );
    //@}
};

#endif // __DrawerLayer_h_
