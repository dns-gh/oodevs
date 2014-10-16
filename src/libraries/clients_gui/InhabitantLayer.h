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
    class GLView_ABC;
    class Viewport_ABC;
    class GLView_ABC;

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
             InhabitantLayer( kernel::Controllers& controllers,
                              GLView_ABC& view,
                              ColorStrategy_ABC& strategy,
                              const kernel::Profile_ABC& profile );
    virtual ~InhabitantLayer();
    //@}
};

}
#endif // _gui_InhabitantLayer_h_
