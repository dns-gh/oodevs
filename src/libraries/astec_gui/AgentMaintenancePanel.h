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
#include "astec_kernel/ElementObserver_ABC.h"
#include "astec_gaming/LogMaintenanceConsign.h"
#include "astec_kernel/Agent_ABC.h"

class MaintenanceStates;
class Availability;
class Displayer_ABC;
class ValuedListItem;
class DisplayBuilder;

// =============================================================================
/** @class  AgentMaintenancePanel
    @brief  Agent_ABC maintenance panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentMaintenancePanel : public LogisticPanel< AgentMaintenancePanel, LogMaintenanceConsign >
                            , public ElementObserver_ABC< MaintenanceStates >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentMaintenancePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~AgentMaintenancePanel();
    //@}

    //! @name Operations
    //@{
    void Display( const LogMaintenanceConsign* consign, Displayer_ABC& displayer, ValuedListItem* );
    void Display( const Availability& availability, Displayer_ABC& displayer, ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const Agent_ABC& agent );
    virtual void NotifyUpdated( const MaintenanceStates& consigns );

    virtual void DisplayRequested( const LogisticConsigns& consigns, ListDisplayer< AgentMaintenancePanel >* list );
    virtual void DisplayHandled( const LogisticConsigns& consigns, ListDisplayer< AgentMaintenancePanel >* list );
    //@}

private:

    //! @name Member data
    //@{
    Controllers& controllers_;
    DisplayBuilder* display_;
    ListDisplayer< AgentMaintenancePanel >* dispoHaulers_;
    ListDisplayer< AgentMaintenancePanel >* dispoRepairers_;
    //@}
};

#endif // __AgentMaintenancePanel_h_
