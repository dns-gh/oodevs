// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentSupplyPanel_h_
#define __AgentSupplyPanel_h_

#include "EntityPanel.h"

// =============================================================================
/** @class  AgentSupplyPanel
    @brief  Agent_ABC supply panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentSupplyPanel : public EntityPanel
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentSupplyPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AgentSupplyPanel();
    //@}
};

#endif // __AgentSupplyPanel_h_
