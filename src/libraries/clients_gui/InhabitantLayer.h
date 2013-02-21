// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef _gui_InhabitantLayer_h_
#define _gui_InhabitantLayer_h_

#include "EntityLayer.h"
#include "clients_kernel/Inhabitant_ABC.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;
    class GlTools_ABC;
    class Viewport_ABC;
    class View_ABC;
}

namespace gui
{
// =============================================================================
/** @class  InhabitantLayer
    @brief  Inhabitant layer
*/
// Created: SLG 2006-03-23
// =============================================================================
class InhabitantLayer : public EntityLayer< kernel::Inhabitant_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             InhabitantLayer( kernel::Controllers& controllers, GlTools_ABC& tools,
                              ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~InhabitantLayer();
    //@}
};

}
#endif // _gui_InhabitantLayer_h_
