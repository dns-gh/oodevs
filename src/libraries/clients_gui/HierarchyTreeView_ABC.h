// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __HierarchyTreeView_ABC_h_
#define __HierarchyTreeView_ABC_h_

#include "EntityTreeView_ABC.h"
#include "DragAndDropObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Hierarchies;
    class Profile_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class Ghost_ABC;
    class KnowledgeGroup_ABC;
    class AgentType;
    class AutomatType;
}

namespace gui
{
    class EntitySymbols;
    class ModelObserver_ABC;

// =============================================================================
/** @class  HierarchyTreeView_ABC
    @brief  HierarchyTreeView_ABC
*/
// Created: ABR 2012-08-10
// =============================================================================
class HierarchyTreeView_ABC : public EntityTreeView_ABC
                            , public DragAndDropObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             HierarchyTreeView_ABC( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, const EntitySymbols& symbols, QWidget* parent = 0 );
    virtual ~HierarchyTreeView_ABC();
    //@}

    //! @name Operations
    //@{
    void ActivateSelection( bool activate );
    bool IsActivated() const;
    //@}

protected:
    //! @name Helpers
    //@{
    void InternalNotifyCreated( const kernel::Hierarchies& hierarchy );
    void InternalNotifyUpdated( const kernel::Hierarchies& hierarchy );
    void UpdateBackgroundColor( QStandardItem& entityItem, const kernel::Entity_ABC& entity );
    void UpdateSymbol( QStandardItem& entityItem, const kernel::Entity_ABC& entity );
    //@}

    //! @name Operations
    //@{
    virtual Qt::ItemFlags ItemSpecificFlags( const kernel::Entity_ABC& entity ) const;
    virtual void focusInEvent( QFocusEvent* event );
    //@}

private:
    //! @name Drag n drop
    //@{
    virtual bool CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const;
    virtual QStringList AdditionalMimeTypes() const;
    virtual void Drop( const QString& mimeType, void* data, QStandardItem& target );
    virtual void dragMoveEvent( QDragMoveEvent *pEvent );
    virtual void Drop( const kernel::Agent_ABC& /*item*/, const kernel::Entity_ABC& /*target*/ ) {}
    virtual void Drop( const kernel::Automat_ABC& /*item*/, const kernel::Entity_ABC& /*target*/ ) {}
    virtual void Drop( const kernel::Formation_ABC& /*item*/, const kernel::Entity_ABC& /*target*/ ) {}
    virtual void Drop( const kernel::Ghost_ABC& /*item*/, const kernel::Entity_ABC& /*target*/ ) {}
    virtual void Drop( const kernel::KnowledgeGroup_ABC& /*item*/, const kernel::Entity_ABC& /*target*/ ) {}
    virtual void Drop( const kernel::AgentType& /*item*/, kernel::Entity_ABC& /*target*/ ) {}
    virtual void Drop( const kernel::AutomatType& /*item*/, kernel::Entity_ABC& /*target*/ ) {}
    //@}

signals:
    void TreeViewFocusIn( gui::HierarchyTreeView_ABC* );

protected slots:
    //! @name Slots
    //@{
    void OnTimeOut();
    //@}

private:
    //! @name Member data
    //@{
    const EntitySymbols& symbols_;
    QTimer* timer_;
    bool activated_;
    std::set< const kernel::Entity_ABC* > waitingSymbols_;
    //@}
};

} //! namespace gui

#endif // __HierarchyTreeView_ABC_h_
