// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-01 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentMaintenancePanel.h $
// $Author: Nld $
// $Modtime: 27/04/05 18:27 $
// $Revision: 3 $
// $Workfile: AgentMaintenancePanel.h $
//
// *****************************************************************************

#ifndef __AgentMaintenancePanel_h_
#define __AgentMaintenancePanel_h_

#include "InfoPanel_ABC.h"
#include "ElementObserver_ABC.h"
#include "SelectionObserver_ABC.h"
#include "SafePointer.h"

class DisplayBuilder;
class Controllers;
template< typename T > class ListDisplayer;
class Displayer_ABC;
class ValuedListItem;
class SubItemDisplayer;

class LogisticConsigns;
class MaintenanceStates;
class LogMaintenanceConsign;
class Agent;
class Availability;

// =============================================================================
/** @class  AgentMaintenancePanel
    @brief  Agent maintenance panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentMaintenancePanel : public InfoPanel_ABC
                            , private Observer_ABC
                            , public SelectionObserver< Agent >
                            , public ElementObserver_ABC< LogisticConsigns >
                            , public ElementObserver_ABC< MaintenanceStates >
                            , public ElementObserver_ABC< LogMaintenanceConsign >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentMaintenancePanel( InfoPanels* pParent, Controllers& controllers );
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
    void NotifySelected( const Agent* agent );
    void NotifyUpdated( const LogisticConsigns& consigns );
    void NotifyUpdated( const MaintenanceStates& consigns );
    void NotifyUpdated( const LogMaintenanceConsign& consign );
    template< typename Extension >
    bool ShouldUpdate( const Extension& e );
    void showEvent( QShowEvent* );
    //@}

private:

    //! @name Member data
    //@{
    Controllers& controllers_;
    SafePointer< Agent > selected_;
    ListDisplayer< AgentMaintenancePanel >* pConsignListView_;
    ListDisplayer< AgentMaintenancePanel >* pConsignHandledListView_;
    SubItemDisplayer* logDisplay_;

    DisplayBuilder* display_;
    ListDisplayer< AgentMaintenancePanel >* dispoHaulers_;
    ListDisplayer< AgentMaintenancePanel >* dispoRepairers_;
    //@}
};

#endif // __AgentMaintenancePanel_h_
