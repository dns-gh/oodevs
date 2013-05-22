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
#include "clients_kernel/MultipleSelectionObserver_ABC.h"
#include "tools/Observer_ABC.h"

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
                     , public kernel::MultipleSelectionObserver< kernel::Entity_ABC >
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
    virtual void NotifySelectionChanged( const std::vector< const kernel::Entity_ABC* >& elements );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual bool IsTypeRejected( const kernel::Entity_ABC& entity ) const = 0;
    virtual void MultipleSelectionChanged();
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnContextMenuRequested( Q3ListViewItem* i, const QPoint& pos, int );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange();
    void OnRequestCenter();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyActivated( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    virtual void NotifyUpdated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    bool blockSelect_;
    //@}
};

}

#endif // gui_EntityListView_h