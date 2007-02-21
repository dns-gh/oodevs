// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentResourcesPanel_h_
#define __AgentResourcesPanel_h_

#include "EntityPanel.h"

// =============================================================================
/** @class  AgentResourcesPanel
    @brief  Agent_ABC resources panel
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentResourcesPanel : public EntityPanel
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentResourcesPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AgentResourcesPanel();
    //@}
};

#endif // __AgentResourcesPanel_h_
