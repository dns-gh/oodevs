// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticPanel_h_
#define __LogisticPanel_h_

#include "InfoPanel_ABC.h"
#include "ElementObserver_ABC.h"
#include "SelectionObserver_ABC.h"
#include "ListDisplayer.h"
#include "SafePointer.h"
#include "SubItemDisplayer.h"
#include "LogisticConsigns.h"

class ItemFactory_ABC;
class Controllers;
class Agent_ABC;

// =============================================================================
/** @class  LogisticPanel
    @brief  LogisticPanel
*/
// Created: AGE 2006-07-04
// =============================================================================
template< typename ConcretePanel, typename Consign >
class LogisticPanel : public InfoPanel_ABC
                    , public Observer_ABC
                    , public SelectionObserver< Agent_ABC >
                    , public ElementObserver_ABC< LogisticConsigns >
                    , public ElementObserver_ABC< Consign >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticPanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory, const QString& tabName );
    virtual ~LogisticPanel();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticPanel( const LogisticPanel& );            //!< Copy constructor
    LogisticPanel& operator=( const LogisticPanel& ); //!< Assignement operator
    //@}

protected:
    //! @name Types
    //@{
    typedef LogisticPanel< ConcretePanel, Consign > Parent;
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void NotifySelected( const Agent_ABC* agent );
    virtual void NotifySelected( const Agent_ABC& agent ) = 0;
    virtual void NotifyUpdated( const LogisticConsigns& consigns );
    virtual void NotifyUpdated( const Consign& consigns );

    template< typename Extension >
    bool ShouldUpdate( const Extension& e );

    void AddConsignColumn( const char* column );

    virtual void DisplayRequested( const LogisticConsigns& consigns, ListDisplayer< ConcretePanel >* list ) = 0;
    virtual void DisplayHandled( const LogisticConsigns& consigns, ListDisplayer< ConcretePanel >* list ) = 0;

    Displayer_ABC& GetDisplayer( ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    SafePointer< Agent_ABC > selected_;
    ListDisplayer< ConcretePanel >* pConsignListView_;
    ListDisplayer< ConcretePanel >* pConsignHandledListView_;
    SubItemDisplayer* logDisplay_;
    //@}
};

#include "LogisticPanel.inl"

#endif // __LogisticPanel_h_
