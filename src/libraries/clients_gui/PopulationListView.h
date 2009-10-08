// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_PopulationListView_h_
#define __gui_PopulationListView_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "ListView.h"

namespace kernel
{
    class Controllers;
    class Population_ABC;
    class Profile_ABC;
    class Entity_ABC;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  PopulationListView
    @brief  PopulationListView
*/
// Created: HME 2005-10-03
// =============================================================================
class PopulationListView : public ListView< PopulationListView >
                         , public tools::Observer_ABC
                         , public kernel::ElementObserver_ABC< kernel::Population_ABC >
                         , public kernel::ElementObserver_ABC< kernel::Profile_ABC >
                         , public kernel::ElementObserver_ABC< kernel::Entity_ABC >
                         , public kernel::ElementObserver_ABC< kernel::Team_ABC >
                         , public kernel::SelectionObserver< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PopulationListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~PopulationListView();
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
    PopulationListView( const PopulationListView& );
    PopulationListView& operator=( const PopulationListView& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Population_ABC& popu );
    virtual void NotifyUpdated( const kernel::Entity_ABC& element );
    virtual void NotifyDeleted( const kernel::Population_ABC& popu );
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

#endif // __gui_PopulationListView_h_
