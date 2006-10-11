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

#include "LogisticPanel.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "gaming/LogSupplyConsign.h"

namespace kernel
{
    class Agent_ABC;
    class Displayer_ABC;
}

namespace gui
{
    class ValuedListItem;
    class DisplayBuilder;
}

class SupplyStates;
class Availability;
class DotationRequest;
class Dotation;
class Quotas;

// =============================================================================
/** @class  AgentSupplyPanel
    @brief  Agent_ABC supply panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentSupplyPanel : public LogisticPanel< AgentSupplyPanel, LogSupplyConsign >
                       , public kernel::ElementObserver_ABC< SupplyStates >
                       , public kernel::ElementObserver_ABC< Quotas >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentSupplyPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AgentSupplyPanel();
    //@}

    //! @name Operations
    //@{
    void Display( const LogSupplyConsign* consign,  kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    void Display( const Availability& availability, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    void Display( const Dotation& quota, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    void Display( const DotationRequest& request, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC& agent );
    virtual void NotifyUpdated( const SupplyStates& consigns );
    virtual void NotifyUpdated( const Quotas& consigns );

    virtual void DisplayRequested( const LogisticConsigns& consigns, gui::ListDisplayer< AgentSupplyPanel >* list );
    virtual void DisplayHandled( const LogisticConsigns& consigns, gui::ListDisplayer< AgentSupplyPanel >* list );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ItemFactory_ABC& factory_;
    gui::DisplayBuilder* display_;
    gui::ListDisplayer< AgentSupplyPanel >* pStocks_;
    gui::ListDisplayer< AgentSupplyPanel >* pQuotas_;
    gui::ListDisplayer< AgentSupplyPanel >* pDispoTransporters_;
    //@}
};

#endif // __AgentSupplyPanel_h_
