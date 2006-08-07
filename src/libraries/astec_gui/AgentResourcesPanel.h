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

#include "astec_gaming/Types.h"
#include "InfoPanel_ABC.h"
#include "astec_gaming/ElementObserver_ABC.h"
#include "astec_gaming/SelectionObserver_ABC.h"
#include "astec_gaming/SafePointer.h"

template< typename T > class ListDisplayer;
class Agent_ABC;
class Controllers;
class Dotation;
class Dotations;
class Equipments;
class Equipment;
class Loan;
class Borrowings;
class Lendings;
class Displayer_ABC;
class ValuedListItem;
class Humans;
class Troops;
class ItemFactory_ABC;

// =============================================================================
/** @class  AgentResourcesPanel
    @brief  Agent_ABC resources panel
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentResourcesPanel : public InfoPanel_ABC
                          , public Observer_ABC
                          , public ElementObserver_ABC< Dotations >
                          , public ElementObserver_ABC< Equipments >
                          , public ElementObserver_ABC< Lendings >
                          , public ElementObserver_ABC< Borrowings >
                          , public ElementObserver_ABC< Troops >
                          , public SelectionObserver< Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentResourcesPanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~AgentResourcesPanel();
    //@}

    //! @name Operations
    //@{
    void Display( const Dotation& dotation, Displayer_ABC& displayer, ValuedListItem* );
    void Display( const Equipment& equipment, Displayer_ABC& displayer, ValuedListItem* );
    void Display( const Loan& loan, Displayer_ABC& displayer, ValuedListItem* );
    void Display( const Humans& humans, Displayer_ABC& displayer, ValuedListItem* );
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
    virtual void NotifySelected( const Agent_ABC* agent );

    template< typename T >
    bool ShouldUpdate( const T& a );
    template< typename T >
    void UpdateExtension( const Agent_ABC& agent );
    //@}

    //! @name Types
    //@{
    typedef ListDisplayer< AgentResourcesPanel > T_ListView;
    //@}
    
private:
    //! @name Member Data
    //@{
    Controllers& controllers_;
    T_ListView* pEquipment_;
    T_ListView* pResources_;
    T_ListView* pTroops_;
    T_ListView* pLendings_;
    T_ListView* pBorrowings_;
    SafePointer< Agent_ABC > selected_;
    //@}
};

#endif // __AgentResourcesPanel_h_
