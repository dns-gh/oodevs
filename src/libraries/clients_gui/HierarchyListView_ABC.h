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
#include "clients_kernel/ActivationObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include <boost/function.hpp>

namespace kernel
{
    class Controllers;
    class Hierarchies;
    class Entity_ABC;
    class Profile_ABC;
    class Symbol_ABC;
}

namespace gui
{
    class ValuedListItem;
    class ItemFactory_ABC;
    class EntitySymbols;

// =============================================================================
/** @class  HierarchyListView_ABC
    @brief  Displays the agent list sorted according to a hierarchy
*/
// Created: APE 2004-03-15
// =============================================================================
class HierarchyListView_ABC : public ListView< HierarchyListView_ABC >
                            , public tools::Observer_ABC
                            , public tools::SelectionObserver< kernel::Entity_ABC >
                            , public kernel::ActivationObserver_ABC< kernel::Entity_ABC >
                            , public tools::ElementObserver_ABC< kernel::Profile_ABC >
                            , public tools::ElementObserver_ABC< kernel::Symbol_ABC >
{
   Q_OBJECT

public:
    //! @name Constants
    //@{
    static const char* agentMimeType_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             HierarchyListView_ABC( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, EntitySymbols& symbols );
    virtual ~HierarchyListView_ABC();
    //@}

    //! @name Operations
    //@{
    QSize sizeHint() const;
    virtual void Display( const kernel::Entity_ABC& entity, ValuedListItem* item );
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnSelectionChange( QListViewItem* item );
    virtual void OnContextMenuRequested( QListViewItem*, const QPoint&, int );
    void OnRequestCenter();
    void Update();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual const kernel::Hierarchies* RetrieveHierarchy( const kernel::Entity_ABC& entity ) = 0;
    virtual void NotifyCreated( const kernel::Hierarchies& hierarchies );
    virtual void NotifyUpdated( const kernel::Hierarchies& hierarchies );
    virtual void NotifyDeleted( const kernel::Hierarchies& hierarchies );
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    virtual void NotifyUpdated( const kernel::Symbol_ABC& symbol );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyActivated( const kernel::Entity_ABC& element );
    virtual void focusInEvent( QFocusEvent* );
    virtual void DisplayIcon( const kernel::Entity_ABC& entity, ValuedListItem* item );
    static void SetVisible( QListViewItem* item, bool visible );
    void ApplyFilter( boost::function< bool ( gui::ValuedListItem* ) > func );
    //@}

private:
    //! @name Helpers
    //@{
    ValuedListItem* FindOrCreate( const kernel::Entity_ABC* entity );
    void UpdateItem( ValuedListItem* item );
    virtual QDragObject* dragObject();
    virtual void dropEvent( QDropEvent* pEvent );
    virtual void dragEnterEvent( QDragEnterEvent* pEvent );
    virtual void dragMoveEvent( QDragMoveEvent *pEvent );
    bool Drop( const kernel::Entity_ABC& entity, ValuedListItem& target );
    virtual bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    bool HasAnyChildVisible( gui::ValuedListItem* item, boost::function< bool ( gui::ValuedListItem* ) > func );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ItemFactory_ABC& factory_;
    const kernel::Profile_ABC& profile_;
    EntitySymbols& symbols_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QTimer* timer_;
    //@}
};

}

#endif // __Gui_HierarchyListView_ABC_h_
