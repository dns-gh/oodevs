// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gui_HierarchyListView_ABC_h_
#define __Gui_HierarchyListView_ABC_h_

#include "ListView.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/ActivationObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Hierarchies;
    class Entity_ABC;
    class Profile_ABC;
}

namespace gui
{
    class ValuedListItem;
    class ItemFactory_ABC;

// =============================================================================
/** @class  HierarchyListView_ABC
    @brief  Displays the agent list sorted according to a hierarchy
*/
// Created: APE 2004-03-15
// =============================================================================
class HierarchyListView_ABC : public ListView< HierarchyListView_ABC >
                            , public kernel::Observer_ABC
                            , public kernel::SelectionObserver< kernel::Entity_ABC >
                            , public kernel::ActivationObserver_ABC< kernel::Entity_ABC >
                            , public kernel::ElementObserver_ABC< kernel::Profile_ABC >
{
   Q_OBJECT;

public:
    //! @name Constants
    //@{
    static const char* agentMimeType_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             HierarchyListView_ABC( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~HierarchyListView_ABC();

    QSize sizeHint() const;
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& entity, ValuedListItem* item );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    HierarchyListView_ABC( const HierarchyListView_ABC& );
    HierarchyListView_ABC& operator=( const HierarchyListView_ABC& );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( QListViewItem* item );
    void OnContextMenuRequested( QListViewItem*, const QPoint&, int );
    void OnRequestCenter();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual const kernel::Hierarchies* RetrieveHierarchy( const kernel::Entity_ABC& entity ) = 0;
    virtual void NotifyCreated( const kernel::Hierarchies& hierarchies );
    virtual void NotifyUpdated( const kernel::Hierarchies& hierarchies );
    virtual void NotifyDeleted( const kernel::Hierarchies& hierarchies );

    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    //@}

private:
    //! @name Helpers
    //@{
    ValuedListItem* FindOrCreate( const kernel::Entity_ABC* entity );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyActivated( const kernel::Entity_ABC& element );

    virtual QDragObject* dragObject();
    virtual void dropEvent( QDropEvent* pEvent );
    virtual void dragEnterEvent( QDragEnterEvent* pEvent );

    bool Drop( ValuedListItem& item, ValuedListItem& target );
    virtual bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    static void SetVisible( QListViewItem* item, bool visible );
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

#endif // __Gui_HierarchyListView_ABC_h_
