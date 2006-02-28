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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentSupplyPanel.h $
// $Author: Age $
// $Modtime: 6/04/05 10:41 $
// $Revision: 2 $
// $Workfile: AgentSupplyPanel.h $
//
// *****************************************************************************

#ifndef __AgentSupplyPanel_h_
#define __AgentSupplyPanel_h_

#include "InfoPanel_ABC.h"
#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "SelectionObserver_ABC.h"

class DisplayBuilder;
class Controller;
class ActionController;
template< typename T > class ListDisplayer;
class Displayer_ABC;
class ValuedListItem;

class LogisticConsigns;
class SupplyStates;
class Agent;
class LogSupplyConsign;
class Availability;
class SubItemDisplayer;
class Dotation;

// =============================================================================
/** @class  AgentSupplyPanel
    @brief  Agent supply panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentSupplyPanel : public InfoPanel_ABC
                       , private Observer_ABC
                       , public SelectionObserver< Agent >
                       , public ElementObserver_ABC< LogisticConsigns >
                       , public ElementObserver_ABC< SupplyStates >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentSupplyPanel( InfoPanel* pParent, Controller& controller, ActionController& actionController );
    virtual ~AgentSupplyPanel();
    //@}

    //! @name Operations
    //@{
    void Display( const LogSupplyConsign* consign,  Displayer_ABC& displayer, ValuedListItem* );
    void Display( const Availability& availability, Displayer_ABC& displayer, ValuedListItem* );
    void Display( const Dotation& quota, Displayer_ABC& displayer, ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    void NotifySelected( const Agent* agent );
    void NotifyUpdated( const LogisticConsigns& consigns );
    void NotifyUpdated( const SupplyStates& consigns );
    template< typename Extension >
    bool ShouldUpdate( const Extension& e );
    void showEvent( QShowEvent* );
    //@}

private:
    //! @name Member data
    //@{
    const Agent* selected_;
    ListDisplayer< AgentSupplyPanel >* pConsignListView_;
    ListDisplayer< AgentSupplyPanel >* pConsignHandledListView_;
    SubItemDisplayer* logDisplay_;

    DisplayBuilder* display_;

    ListDisplayer< AgentSupplyPanel >* pStocks_;
    ListDisplayer< AgentSupplyPanel >* pQuotas_;
    ListDisplayer< AgentSupplyPanel >* pDispoTransporters_;
    ListDisplayer< AgentSupplyPanel >* pDispoCommanders_;
    //@}
};

#endif // __AgentSupplyPanel_h_
