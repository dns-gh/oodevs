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

#include "LogisticPanel.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "gaming/LogMaintenanceConsign.h"

namespace kernel
{
    class Displayer_ABC;
}

namespace gui
{
    class ValuedListItem;
    class DisplayBuilder;
}

class Availability;
class MaintenanceStates;

// =============================================================================
/** @class  AgentMaintenancePanel
    @brief  Agent_ABC maintenance panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentMaintenancePanel : public LogisticPanel< AgentMaintenancePanel, LogMaintenanceConsign >
                            , public kernel::ElementObserver_ABC< MaintenanceStates >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentMaintenancePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AgentMaintenancePanel();
    //@}

    //! @name Operations
    //@{
    void Display( const LogMaintenanceConsign* consign, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    void Display( const Availability& availability, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Agent_ABC& agent );
    virtual void NotifyUpdated( const MaintenanceStates& consigns );

    virtual void DisplayRequested( const LogisticConsigns& consigns, gui::ListDisplayer< AgentMaintenancePanel >* list );
    virtual void DisplayHandled( const LogisticConsigns& consigns, gui::ListDisplayer< AgentMaintenancePanel >* list );
    //@}

private:

    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::DisplayBuilder* display_;
    gui::ListDisplayer< AgentMaintenancePanel >* dispoHaulers_;
    gui::ListDisplayer< AgentMaintenancePanel >* dispoRepairers_;
    //@}
};

#endif // __AgentMaintenancePanel_h_
