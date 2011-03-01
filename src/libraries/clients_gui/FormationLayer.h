// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_FormationLayer_h_
#define __gui_FormationLayer_h_

#include "EntityLayer.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/Formation_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Formation_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;

// =============================================================================
/** @class  FormationLayer
    @brief  Layer to display entities which have no real position
*/
// Created: AGE 2007-05-31
// =============================================================================
class FormationLayer : public EntityLayer< kernel::Formation_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             FormationLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view,
                             const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter );
    virtual ~FormationLayer();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void ActivateEntity( const kernel::Entity_ABC& );
    virtual void Draw( const kernel::Entity_ABC& entity, kernel::Viewport_ABC& viewport );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    ColorStrategy_ABC& strategy_;
    //@}
};

}

#endif // __gui_FormationLayer_h_
