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

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Agent_ABC;
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  AgentResourcesPanel
    @brief  Agent_ABC resources panel
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentResourcesPanel : public gui::InfoPanel_ABC
                          , public kernel::Observer_ABC
                          , public kernel::SelectionObserver< kernel::Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentResourcesPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AgentResourcesPanel();
    //@}

private:
    //! @name Copy / Assignment
    //@{
    AgentResourcesPanel( const AgentResourcesPanel& );
    AgentResourcesPanel& operator=( const AgentResourcesPanel& );
    //@}

private:
    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifySelected( const kernel::Agent_ABC* agent );
    //@}
    
private:
    //! @name Member Data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    //@}
};

#endif // __AgentResourcesPanel_h_
