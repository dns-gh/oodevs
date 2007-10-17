// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligenceListView_h_
#define __IntelligenceListView_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "ListView.h"

namespace kernel
{
    class Controllers;
    class Intelligence_ABC;
    class Profile_ABC;
    class Entity_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class EntitySymbols;

// =============================================================================
/** @class  IntelligenceListView
    @brief  IntelligenceListView
*/
// Created: SBO 2007-10-16
// =============================================================================
class IntelligenceListView : public ListView< IntelligenceListView >
                           , public kernel::Observer_ABC
                           , public kernel::ElementObserver_ABC< kernel::Intelligence_ABC >
                           , public kernel::ElementObserver_ABC< kernel::Profile_ABC >
                           , public kernel::ElementObserver_ABC< kernel::Entity_ABC >
                           , public kernel::ElementObserver_ABC< kernel::Team_ABC >
                           , public kernel::SelectionObserver< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             IntelligenceListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, EntitySymbols& symbols );
    virtual ~IntelligenceListView();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( QListViewItem* item );
    void OnRequestCenter();
    void OnContextMenuRequested( QListViewItem*, const QPoint&, int );
    void Update( QListViewItem* rootItem = 0 );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligenceListView( const IntelligenceListView& );            //!< Copy constructor
    IntelligenceListView& operator=( const IntelligenceListView& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Intelligence_ABC& element );
    virtual void NotifyDeleted( const kernel::Intelligence_ABC& element );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    void Display( const kernel::Intelligence_ABC& entity, ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ItemFactory_ABC& factory_;
    const kernel::Profile_ABC& profile_;
    EntitySymbols& symbols_;
    QTimer* timer_;
    //@}
};

}

#endif // __IntelligenceListView_h_
