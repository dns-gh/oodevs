// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_LogisticTreeView_h_
#define __gui_LogisticTreeView_h_

#include "HierarchyTreeView_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class LogisticLevel;
    class TacticalHierarchies;
}

namespace gui
{

// =============================================================================
/** @class  LogisticTreeView
    @brief  LogisticTreeView
*/
// Created: ABR 2012-09-19
// =============================================================================
class LogisticTreeView : public HierarchyTreeView_ABC
                       , public tools::ElementObserver_ABC< kernel::Ghost_ABC >
                       , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                       , public tools::ElementObserver_ABC< kernel::Formation_ABC >
                       , public tools::ElementObserver_ABC< kernel::TacticalHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, const EntitySymbols& symbols, QWidget* parent /*= 0*/ );
    virtual ~LogisticTreeView();
    //@}

protected:
    //! @name Abstract methods
    //@{
    virtual void SetSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC* superior ) = 0;
    virtual const kernel::Entity_ABC* RetrieveSuperior( const kernel::Entity_ABC& entity ) const = 0;
    //@}

    //! @name Operations
    //@{
    void CreateOrReplace( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name ItemDecorationGetter_ABC
    //@{
    virtual const QPixmap* GetDecoration( const QModelIndex &index );
    //@}

    //! @name TreeView reimplementation
    //@{
    virtual void OnActivate( const QModelIndex& index );
    //@}

    //! @name Helpers
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* event );

    void UpdateLongName( const kernel::Entity_ABC& entity );
    void NotifyDeletedInternal( const kernel::Entity_ABC& entity );

    QStandardItem& RetrieveTeamItem( const kernel::Entity_ABC& entity );
    QStandardItem& RetrieveTypeItem( const kernel::Entity_ABC& entity, QStandardItem& teamItem );

    const kernel::LogisticLevel& GetLogisticLevel( const kernel::Entity_ABC& entity ) const;
    //@}

    //! @name Observer implementation
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyUpdated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void NotifyCreated( const kernel::Formation_ABC& formation );
    virtual void NotifyUpdated( const kernel::Formation_ABC& formation );
    virtual void NotifyDeleted( const kernel::Formation_ABC& formation );
    virtual void NotifyCreated( const kernel::Automat_ABC& automat );
    virtual void NotifyUpdated( const kernel::Automat_ABC& automat );
    virtual void NotifyDeleted( const kernel::Automat_ABC& automat );
    virtual void NotifyCreated( const kernel::Ghost_ABC& ghost );
    virtual void NotifyUpdated( const kernel::Ghost_ABC& ghost );
    virtual void NotifyDeleted( const kernel::Ghost_ABC& ghost );
    virtual void NotifyUpdated( const kernel::TacticalHierarchies& hierarchy );
    //@}

    //! @name Drag N Drop
    //@{
    virtual QStringList MimeTypes() const;
    virtual void dragMoveEvent( QDragMoveEvent* pEvent );
    bool CanDrop( QDragMoveEvent* pEvent ) const;

    virtual void Drop( const QString& mimeType, void* data, QStandardItem& target );
    //@}
};

} //! namespace gui

#endif // __gui_LogisticTreeView_h_
