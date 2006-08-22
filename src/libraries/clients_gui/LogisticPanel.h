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
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "gaming/LogisticConsigns.h"
#include "clients_kernel/SafePointer.h"
#include "ListDisplayer.h"
#include "SubItemDisplayer.h"

namespace kernel
{
    class Controllers;
    class Agent_ABC;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  LogisticPanel
    @brief  LogisticPanel
*/
// Created: AGE 2006-07-04
// =============================================================================
template< typename ConcretePanel, typename Consign >
class LogisticPanel : public InfoPanel_ABC
                    , public kernel::Observer_ABC
                    , public kernel::SelectionObserver< kernel::Agent_ABC >
                    , public kernel::ElementObserver_ABC< LogisticConsigns >
                    , public kernel::ElementObserver_ABC< Consign >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory, const QString& tabName );
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
    virtual void NotifySelected( const kernel::Agent_ABC* agent );
    virtual void NotifySelected( const kernel::Agent_ABC& agent ) = 0;
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
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    ListDisplayer< ConcretePanel >* pConsignListView_;
    ListDisplayer< ConcretePanel >* pConsignHandledListView_;
    SubItemDisplayer* logDisplay_;
    //@}
};

}

#include "LogisticPanel.inl"

#endif // __LogisticPanel_h_
