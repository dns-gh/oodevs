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
#include "ItemDecorationGetter_ABC.h"

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
                            , public ItemDecorationGetter_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             HierarchyTreeView_ABC( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile,
                                    ModelObserver_ABC& modelObserver, const EntitySymbols& symbols, QWidget* parent = 0 );
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
    QStandardItem* AddItem( QStandardItem* parent, const kernel::Entity_ABC& entity );
    virtual void AdditionalUpdateItem( QStandardItem& /*entityItem*/, const kernel::Entity_ABC& /*entity*/ ) {}
    template< typename Entity >
    bool AddItemIfPossible( const kernel::Entity_ABC& entity, QStandardItem* parent, QStandardItem*& ret );
    //@}

    //! @name Operations
    //@{
    virtual const QPixmap* GetDecoration( const QModelIndex &index );
    virtual void focusInEvent( QFocusEvent* event );
    virtual void drawRow( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex &index ) const;
    //@}

protected:
    //! @name Drag n drop
    //@{
    virtual bool CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const = 0;
    virtual Qt::ItemFlags ItemSpecificFlags( const kernel::Entity_ABC& entity ) const;
    virtual QStringList MimeTypes() const;
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

private:
    //! @name Member data
    //@{
    const EntitySymbols& symbols_;
    bool activated_;
    QPixmap* emptyPixmap_;
    //@}
};

} //! namespace gui

#endif // __HierarchyTreeView_ABC_h_
