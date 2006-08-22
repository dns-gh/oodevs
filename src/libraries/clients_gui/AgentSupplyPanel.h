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
#include "clients_kernel/Agent_ABC.h"

namespace kernel
{
    class Displayer_ABC;
}

class SupplyStates;
class Availability;
class DotationRequest;
class Dotation;

namespace gui
{
    class ValuedListItem;
    class DisplayBuilder;

// =============================================================================
/** @class  AgentSupplyPanel
    @brief  Agent_ABC supply panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentSupplyPanel : public LogisticPanel< AgentSupplyPanel, LogSupplyConsign >
                       , public kernel::ElementObserver_ABC< SupplyStates >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentSupplyPanel( QWidget* parent, PanelStack_ABC& panel, kernel::Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~AgentSupplyPanel();
    //@}

    //! @name Operations
    //@{
    void Display( const LogSupplyConsign* consign,  kernel::Displayer_ABC& displayer, ValuedListItem* );
    void Display( const Availability& availability, kernel::Displayer_ABC& displayer, ValuedListItem* );
    void Display( const Dotation& quota, kernel::Displayer_ABC& displayer, ValuedListItem* );
    void Display( const DotationRequest& request, kernel::Displayer_ABC& displayer, ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Agent_ABC& agent );
    virtual void NotifyUpdated( const SupplyStates& consigns );

    virtual void DisplayRequested( const LogisticConsigns& consigns, ListDisplayer< AgentSupplyPanel >* list );
    virtual void DisplayHandled( const LogisticConsigns& consigns, ListDisplayer< AgentSupplyPanel >* list );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ItemFactory_ABC& factory_;
    DisplayBuilder* display_;
    ListDisplayer< AgentSupplyPanel >* pStocks_;
    ListDisplayer< AgentSupplyPanel >* pQuotas_;
    ListDisplayer< AgentSupplyPanel >* pDispoTransporters_;
    //@}
};

}

#endif // __AgentSupplyPanel_h_
