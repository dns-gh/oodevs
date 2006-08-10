// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentStatePanel_h_
#define __AgentStatePanel_h_

#include "astec_kernel/ElementObserver_ABC.h"
#include "astec_kernel/SelectionObserver_ABC.h"
#include "InfoPanel_ABC.h"
#include "astec_kernel/SafePointer.h"

class Agent_ABC;
class ReportListView;
class DisplayBuilder;
class Controllers;
class Attributes;
class Contaminations;
class HumanFactors;
class Reinforcements;
class LogisticLinks;
class Transports;
class ItemFactory_ABC;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class AgentStatePanel : public InfoPanel_ABC
                      , public Observer_ABC
                      , public ElementObserver_ABC< Agent_ABC >
                      , public ElementObserver_ABC< Attributes >
                      , public ElementObserver_ABC< Contaminations >
                      , public ElementObserver_ABC< HumanFactors >
                      , public ElementObserver_ABC< Reinforcements >
                      , public ElementObserver_ABC< LogisticLinks >
                      , public ElementObserver_ABC< Transports >
                      , public SelectionObserver< Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentStatePanel( PanelStack_ABC* pParent, Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~AgentStatePanel();
    //@}

    virtual void NotifySelected( const Agent_ABC* element );

private:
    //! @name Copy / Assignment
    //@{
    AgentStatePanel( const AgentStatePanel& );
    AgentStatePanel& operator=( const AgentStatePanel& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const Agent_ABC& );
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
    template< typename Extension >
    bool UpdateExtension( const Agent_ABC& agent );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    DisplayBuilder* display_;
    SafePointer< Agent_ABC > selected_;
    //@}
};

#endif // __AgentStatePanel_h_
