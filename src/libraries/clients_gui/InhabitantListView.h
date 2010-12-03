// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_InhabitantListView_h_
#define __gui_InhabitantListView_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "ListView.h"

namespace kernel
{
    class Controllers;
    class Inhabitant_ABC;
    class Profile_ABC;
    class Entity_ABC;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  InhabitantListView
    @brief  InhabitantListView
*/
// Created: HME 2005-10-03
// =============================================================================
class InhabitantListView : public ListView< InhabitantListView >
                         , public tools::Observer_ABC
                         , public tools::ElementObserver_ABC< kernel::Inhabitant_ABC >
                         , public tools::ElementObserver_ABC< kernel::Profile_ABC >
                         , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                         , public tools::ElementObserver_ABC< kernel::Team_ABC >
                         , public tools::SelectionObserver< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~InhabitantListView();
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnContextMenuRequested( QListViewItem*, const QPoint&, int );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( QListViewItem* item );
    void OnRequestCenter();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    InhabitantListView( const InhabitantListView& );
    InhabitantListView& operator=( const InhabitantListView& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Inhabitant_ABC& popu );
    virtual void NotifyUpdated( const kernel::Entity_ABC& element );
    virtual void NotifyDeleted( const kernel::Inhabitant_ABC& popu );
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ItemFactory_ABC& factory_;
    const kernel::Profile_ABC& profile_;
    //@}
};

}

#endif // __gui_InhabitantListView_h_
