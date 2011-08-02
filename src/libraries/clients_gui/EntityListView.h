// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_EntityListView_h
#define gui_EntityListView_h

#include "ListView.h"
#include "tools/Observer_ABC.h"
#include "tools/SelectionObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  EntityListView
    @brief  Entity list view
*/
// Created: LGY 2011-01-05
// =============================================================================
class EntityListView : public ListView< EntityListView >
                     , public tools::Observer_ABC
                     , public tools::SelectionObserver< kernel::Entity_ABC >
                     , public tools::ElementObserver_ABC< kernel::Profile_ABC >
                     , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                     , public tools::ElementObserver_ABC< kernel::Team_ABC >
                     , public kernel::ActivationObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             EntityListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~EntityListView();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnContextMenuRequested( Q3ListViewItem* i, const QPoint& pos, int );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( Q3ListViewItem* item );
    void OnRequestCenter();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyActivated( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    ItemFactory_ABC& factory_;
    //@}
};

}

#endif // gui_EntityListView_h
