// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentStatePanel.h $
// $Author: Nld $
// $Modtime: 8/07/05 15:25 $
// $Revision: 14 $
// $Workfile: AgentStatePanel.h $
//
// *****************************************************************************

#ifndef __AgentStatePanel_h_
#define __AgentStatePanel_h_

#include "Observer_ABC.h"
#include "ElementObserver_ABC.h"
#include "SelectionObserver_ABC.h"
#include "InfoPanel_ABC.h"

class Agent;
class ReportListView;
class DisplayBuilder;
class Controller;
class ActionController;
class Attributes;
class Contaminations;
class HumanFactors;
class Reinforcements;
class LogisticLinks;
class Transports;
class InfoPanels;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class AgentStatePanel : public InfoPanel_ABC
                      , public Observer_ABC
                      , public ElementObserver_ABC< Agent >
                      , public ElementObserver_ABC< Attributes >
                      , public ElementObserver_ABC< Contaminations >
                      , public ElementObserver_ABC< HumanFactors >
                      , public ElementObserver_ABC< Reinforcements >
                      , public ElementObserver_ABC< LogisticLinks >
                      , public ElementObserver_ABC< Transports >
                      , public SelectionObserver< Agent >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentStatePanel( InfoPanels* pParent, Controller& controller, ActionController& actionController );
    virtual ~AgentStatePanel();
    //@}

    virtual void NotifySelected( const Agent* element );

private:
    //! @name Copy / Assignment
    //@{
    AgentStatePanel( const AgentStatePanel& );
    AgentStatePanel& operator=( const AgentStatePanel& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Agent& );
    virtual void NotifyDeleted( const Agent& );
    virtual void NotifyUpdated( const Attributes& attributes );
    virtual void NotifyUpdated( const Contaminations& attributes );
    virtual void NotifyUpdated( const HumanFactors& attributes );
    virtual void NotifyUpdated( const Reinforcements& attributes );
    virtual void NotifyUpdated( const LogisticLinks& attributes );
    virtual void NotifyUpdated( const Transports& attributes );

    template< typename Extension >
    bool ShouldUpdate( const Extension& extension );
    template< typename Extension >
    void DisplayIfNeeded( const Extension& extension );
    //@}

private:
    //! @name Member data
    //@{
    DisplayBuilder* display_;
    const Agent* selected_;
    //@}
};

#endif // __AgentStatePanel_h_
