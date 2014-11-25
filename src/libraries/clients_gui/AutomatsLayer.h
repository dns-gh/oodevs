// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_AutomatsLayer_h_
#define __gui_AutomatsLayer_h_

#include "EntityLayer.h"
#include "clients_kernel/Automat_ABC.h"

namespace gui
{

// =============================================================================
/** @class  AutomatsLayer
    @brief  AutomatsLayer
*/
// Created: SBO 2007-04-12
// =============================================================================
class AutomatsLayer : public EntityLayer< kernel::Automat_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AutomatsLayer( kernel::Controllers& controllers,
                            GLView_ABC& view,
                            ColorStrategy_ABC& strategy,
                            const kernel::Profile_ABC& profile );
    virtual ~AutomatsLayer();
    //@}

private:
    virtual void NotifyActivated( const kernel::Automat_ABC& automat );
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& );
};

} //! namespace gui

#endif // __gui_AutomatsLayer_h_
