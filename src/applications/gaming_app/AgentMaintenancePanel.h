// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentMaintenancePanel_h_
#define __AgentMaintenancePanel_h_

#include "EntityPanel.h"

// =============================================================================
/** @class  AgentMaintenancePanel
    @brief  Agent_ABC maintenance panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentMaintenancePanel : public EntityPanel
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentMaintenancePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AgentMaintenancePanel();
    //@}
};

#endif // __AgentMaintenancePanel_h_
