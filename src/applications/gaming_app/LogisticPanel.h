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

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_gui/ListDisplayer.h"
#include "clients_gui/SubItemDisplayer.h"
#include "gaming/LogisticConsigns.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Agent_ABC;
    class Automat_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  LogisticPanel
    @brief  LogisticPanel
*/
// Created: AGE 2006-07-04
// =============================================================================
template< typename ConcretePanel, typename Consign >
class LogisticPanel : public gui::InfoPanel_ABC
                    , public kernel::Observer_ABC
                    , public kernel::SelectionObserver_ABC
                    , public kernel::SelectionObserver_Base< kernel::Agent_ABC >
                    , public kernel::SelectionObserver_Base< kernel::Automat_ABC >
                    , public kernel::ElementObserver_ABC< LogisticConsigns >
                    , public kernel::ElementObserver_ABC< Consign >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const QString& tabName );
    virtual ~LogisticPanel();
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
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Automat_ABC& agent );
    virtual void Select( const kernel::Agent_ABC&   agent );
    virtual void NotifySelected( const kernel::Entity_ABC& agent ) = 0;

    virtual void NotifyUpdated( const LogisticConsigns& consigns );
    virtual void NotifyUpdated( const Consign& consigns );

    template< typename Extension >
    bool ShouldUpdate( const Extension& e );

    void AddConsignColumn( const char* column );

    virtual void DisplayRequested( const LogisticConsigns& consigns, gui::ListDisplayer< ConcretePanel >* list ) = 0;
    virtual void DisplayHandled( const LogisticConsigns& consigns, gui::ListDisplayer< ConcretePanel >* list ) = 0;

    kernel::Displayer_ABC& GetDisplayer( gui::ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC* potential_;
    kernel::SafePointer< kernel::Entity_ABC >   selected_;

    gui::ListDisplayer< ConcretePanel >* pConsignListView_;
    gui::ListDisplayer< ConcretePanel >* pConsignHandledListView_;
    gui::SubItemDisplayer* logDisplay_;
    //@}
};

#include "LogisticPanel.inl"

#endif // __LogisticPanel_h_
