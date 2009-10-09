// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsListView_h_
#define __ActionsListView_h_

#include "clients_gui/ListDisplayer.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace actions
{
    class Action_ABC;
    class Parameter_ABC;
}

class ActionsToolbar;
class ActionTiming;

class ActionsListView;
typedef gui::ListView< ActionsListView > ActionsListViewBase;

// =============================================================================
/** @class  ActionsListView
    @brief  ActionsListView
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsListView : public ActionsListViewBase
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< actions::Action_ABC >
                      , public tools::ElementObserver_ABC< ActionTiming >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ActionsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~ActionsListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const actions::Parameter_ABC& param, gui::ValuedListItem* item );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemClicked( QListViewItem* item, const QPoint& point, int col );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsListView( const ActionsListView& );            //!< Copy constructor
    ActionsListView& operator=( const ActionsListView& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddColumn( const QString& column, int alignment = AlignAuto, int size = -1 );
    virtual void NotifyCreated( const actions::Action_ABC& action );
    virtual void NotifyUpdated( const actions::Action_ABC& action );
    virtual void NotifyDeleted( const actions::Action_ABC& action );
    virtual void NotifyUpdated( const ActionTiming& extension );
    void Display( QListViewItem* item, const actions::Action_ABC& action );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ItemFactory_ABC& factory_;
    gui::ListItemDisplayer* sub_;
    QPixmap mission_;
    QPixmap checkboxOn_, checkboxOff_;
    QPixmap parameter_;
    //@}
};

#endif // __ActionsListView_h_
