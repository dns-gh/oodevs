// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InhabitantLayer_h_
#define __InhabitantLayer_h_

#include "EntityLayer.h"
#include "clients_kernel/Inhabitant_ABC.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Viewport_ABC;
    class Profile_ABC;
}

namespace gui
{
    class View_ABC;
    class ColorStrategy_ABC;

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
             InhabitantLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools,
                              ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter );
    virtual ~InhabitantLayer();
    //@}
};

}
#endif // __InhabitantLayer_h_
