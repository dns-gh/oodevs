// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_LogisticListView_h_
#define __gui_LogisticListView_h_

#include "HierarchyListView_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Formation_ABC;
    class LogisticLevel;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class EntitySymbols;

// =============================================================================
/** @class  LogisticListView
    @brief  LogisticListView
*/
// Created: ABR 2011-09-13
// =============================================================================
class LogisticListView : public HierarchyListView_ABC
                       , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                       , public tools::ElementObserver_ABC< kernel::Formation_ABC >
                       , public tools::ElementObserver_ABC< kernel::Team_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, const EntitySymbols& symbols, bool renamable = false );
    virtual ~LogisticListView();
    //@}

    //! @name Operations
    //@{
    void Purge();
    //@}

protected:
    //! @name Protected helpers
    //@{
    ValuedListItem* FindOrCreateOrReplace( const kernel::Entity_ABC* entity );
    //@}

private:
    //! @name Abstract methods
    //@{
    virtual void SetSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC* superior ) = 0;
    virtual const kernel::Entity_ABC* RetrieveSuperior( const kernel::Entity_ABC& entity ) const = 0;
    //@}

    //! @name Helpers
    //@{
    const kernel::LogisticLevel& GetLogisticLevel( const kernel::Entity_ABC& entity ) const;
    virtual const kernel::Hierarchies* RetrieveHierarchy( const kernel::Entity_ABC& entity ) const;

    ValuedListItem* CreateItem( ValuedListItem& parent, const kernel::Entity_ABC& entity );
    ValuedListItem* MoveSubTree( ValuedListItem& from, ValuedListItem& toParent );
    ValuedListItem* MoveSubTreeInternal( ValuedListItem& from, ValuedListItem& toParent );
    void DeleteSubTree( ValuedListItem* root );
    ValuedListItem& RetrieveTeamItem( const kernel::Entity_ABC& entity );
    ValuedListItem& RetrieveTypeItem( const kernel::Entity_ABC& entity, ValuedListItem& teamItem );
    //@}

    //! @name Observer implementation
    //@{
    virtual void NotifyCreated( const kernel::Automat_ABC& automat );
    virtual void NotifyDeleted( const kernel::Automat_ABC& automat );
    virtual void NotifyCreated( const kernel::Formation_ABC& formation );
    virtual void NotifyDeleted( const kernel::Formation_ABC& formation );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    void NotifyDeletedInternal( const kernel::Entity_ABC& entity );
    //@}

    //! @name Drag-N-Drop implementation
    //@{
    virtual void dragMoveEvent( QDragMoveEvent *pEvent );
    virtual bool Drop( const kernel::Entity_ABC& entity, ValuedListItem& target );
    bool Drop( const kernel::Entity_ABC& source, const kernel::Entity_ABC& target );
    bool Drop( const kernel::Entity_ABC& source, const kernel::LogisticLevel& target );
    bool CanDrop( const kernel::Entity_ABC* srcEntity, QPoint dstPosition ) const;
    //@}

private:
    //! @name Member data
    //@{
    ItemFactory_ABC&           factory_;
    const kernel::Profile_ABC& profile_;
    const kernel::Entity_ABC*  droppedUnit_;
    bool                       renamable_;
    //@}
};

}

#endif // __gui_LogisticListView_h_
