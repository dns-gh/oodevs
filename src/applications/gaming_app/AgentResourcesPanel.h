// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentResourcesPanel_h_
#define __AgentResourcesPanel_h_

#include "clients_gui/Types.h"
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
class Equipments;
class Equipment;
class Loan;
class Borrowings;
class Lendings;
class Humans;
class Troops;

// =============================================================================
/** @class  AgentResourcesPanel
    @brief  Agent_ABC resources panel
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentResourcesPanel : public gui::InfoPanel_ABC
                          , public kernel::Observer_ABC
                          , public kernel::ElementObserver_ABC< Dotations >
                          , public kernel::ElementObserver_ABC< Equipments >
                          , public kernel::ElementObserver_ABC< Lendings >
                          , public kernel::ElementObserver_ABC< Borrowings >
                          , public kernel::ElementObserver_ABC< Troops >
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
    void Display( const Equipment& equipment, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    void Display( const Loan& loan, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    void Display( const Humans& humans, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    AgentResourcesPanel( const AgentResourcesPanel& );
    AgentResourcesPanel& operator=( const AgentResourcesPanel& );
    //@}

private:
    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const Dotations& a );
    virtual void NotifyUpdated( const Equipments& a );
    virtual void NotifyUpdated( const Lendings& a );
    virtual void NotifyUpdated( const Borrowings& a );
    virtual void NotifyUpdated( const Troops& a );
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
    //! @name Member Data
    //@{
    kernel::Controllers& controllers_;
    T_ListView* pEquipment_;
    T_ListView* pResources_;
    T_ListView* pTroops_;
    T_ListView* pLendings_;
    T_ListView* pBorrowings_;
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    //@}
};

#endif // __AgentResourcesPanel_h_
