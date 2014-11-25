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
#include "clients_kernel/Formation_ABC.h"

namespace gui
{

// =============================================================================
/** @class  FormationLayer
    @brief  Layer to display entities which have no real position
*/
// Created: AGE 2007-05-31
// =============================================================================
class FormationLayer : public EntityLayer< kernel::Formation_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FormationLayer( kernel::Controllers& controllers,
                             GLView_ABC& view,
                             ColorStrategy_ABC& strategy,
                             const kernel::Profile_ABC& profile );
    virtual ~FormationLayer();
    //@}

protected:
    virtual void NotifyActivated( const kernel::Formation_ABC& formation );
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& );
};

} //! namespace gui

#endif // __gui_FormationLayer_h_
