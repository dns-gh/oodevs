// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentMedicalPanel_h_
#define __AgentMedicalPanel_h_

#include "EntityPanel.h"

// =============================================================================
/** @class  AgentMedicalPanel
    @brief  Agent_ABC medical panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentMedicalPanel : public EntityPanel
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentMedicalPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AgentMedicalPanel();
    //@}
};

#endif // __AgentMedicalPanel_h_
