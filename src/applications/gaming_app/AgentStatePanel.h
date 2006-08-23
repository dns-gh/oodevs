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
    class ReportListView;
    class DisplayBuilder;
    class ItemFactory_ABC;
}

class Attributes;
class Contaminations;
class HumanFactors;
class Reinforcements;
class LogisticLinks;
class Transports;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class AgentStatePanel : public gui::InfoPanel_ABC
                      , public kernel::Observer_ABC
                      , public kernel::ElementObserver_ABC< kernel::Agent_ABC >
                      , public kernel::ElementObserver_ABC< Attributes >
                      , public kernel::ElementObserver_ABC< Contaminations >
                      , public kernel::ElementObserver_ABC< HumanFactors >
                      , public kernel::ElementObserver_ABC< Reinforcements >
                      , public kernel::ElementObserver_ABC< LogisticLinks >
                      , public kernel::ElementObserver_ABC< Transports >
                      , public kernel::SelectionObserver< kernel::Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentStatePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AgentStatePanel();
    //@}

    virtual void NotifySelected( const kernel::Agent_ABC* element );

private:
    //! @name Copy / Assignment
    //@{
    AgentStatePanel( const AgentStatePanel& );
    AgentStatePanel& operator=( const AgentStatePanel& );
    //@}

private:
    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyDeleted( const kernel::Agent_ABC& );
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
    bool UpdateExtension( const kernel::Agent_ABC& agent );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::DisplayBuilder* display_;
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    //@}
};

#endif // __AgentStatePanel_h_
