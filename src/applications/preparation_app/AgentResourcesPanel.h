// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
    class Displayer_ABC;
}

namespace gui
{
    template< typename T > class ListDisplayer;
    class ValuedListItem;
    class ItemFactory_ABC;
}

class Dotation;
class Dotations;

// =============================================================================
/** @class  AgentResourcesPanel
    @brief  AgentResourcesPanel
*/
// Created: SBO 2006-10-11
// =============================================================================
class AgentResourcesPanel : public gui::InfoPanel_ABC
                          , public kernel::Observer_ABC
                          , public kernel::ElementObserver_ABC< Dotations >
                          , public kernel::SelectionObserver< kernel::Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentResourcesPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AgentResourcesPanel();
    //@}

    //! @name Operations
    //@{
    void Display( const Dotation& dotation, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentResourcesPanel( const AgentResourcesPanel& );            //!< Copy constructor
    AgentResourcesPanel& operator=( const AgentResourcesPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const Dotations& a );
    virtual void NotifySelected( const kernel::Agent_ABC* agent );

    template< typename T >
    bool ShouldUpdate( const T& a );
    template< typename T >
    void UpdateExtension( const kernel::Agent_ABC& agent );
    //@}

    //! @name Types
    //@{
    typedef gui::ListDisplayer< AgentResourcesPanel > T_ListView;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_ListView* pDotations_;
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    //@}
};

#endif // __AgentResourcesPanel_h_
