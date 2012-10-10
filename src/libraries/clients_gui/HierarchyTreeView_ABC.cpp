// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "HierarchyTreeView_ABC.h"
#include "moc_HierarchyTreeView_ABC.cpp"
#include "EntitySymbols.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC constructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
HierarchyTreeView_ABC::HierarchyTreeView_ABC( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, const EntitySymbols& symbols, QWidget* parent /*= 0*/ )
    : EntityTreeView_ABC( controllers, profile, modelObserver, parent )
    , symbols_( symbols )
    , activated_( true )
{
    dataModel_.SetDecorationGetter( this );
    EnableDragAndDrop( true );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC destructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
HierarchyTreeView_ABC::~HierarchyTreeView_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::ActivateSelection
// Created: JSR 2012-09-10
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::ActivateSelection( bool activate )
{
    if( activate && !activated_ )
    {
        activated_ = true;
        SetContextMenuBlocked( false );
        setSelectionMode( QAbstractItemView::SingleSelection );
    }
    else if( !activate && activated_ )
    {
        activated_ = false;
        //ClearSelection();
        clearSelection();
        setSelectionMode( QAbstractItemView::NoSelection );
        SetContextMenuBlocked( true );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::IsActivated
// Created: JSR 2012-09-11
// -----------------------------------------------------------------------------
bool HierarchyTreeView_ABC::IsActivated() const
{
    return activated_;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::InternalNotifyCreated
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::InternalNotifyCreated( const kernel::Hierarchies& hierarchy )
{
    const kernel::Entity_ABC& entity = hierarchy.GetEntity();
    const kernel::Entity_ABC* superior = hierarchy.GetSuperior();
    QStandardItem* entityItem = 0;

    if( superior )      // Child item
    {
        QStandardItem* superiorItem = dataModel_.FindDataItem( *superior );
        if( !superiorItem )
            superiorItem = AddItem( 0, *superior );
        entityItem = AddItem( superiorItem, entity );
    }
    else                // Root item
        entityItem = AddItem( 0, entity );

    if( entityItem )
        AdditionalUpdateItem( *entityItem, entity );
}


// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::AddItemIfPossible
// Created: JSR 2012-09-18
// -----------------------------------------------------------------------------
template< typename Entity >
bool HierarchyTreeView_ABC::AddItemIfPossible( const kernel::Entity_ABC& entity, QStandardItem* parent, QStandardItem*& ret )
{
    if( entity.GetTypeName() == Entity::typeName_ )
    {
        if( !parent )
            parent = dataModel_.invisibleRootItem();
        int row = parent->rowCount();
        ret = dataModel_.AddChildSafeItem( parent, row, 0, entity.GetName(), entity.GetTooltip(), static_cast< const Entity& >( entity ), Qt::ItemIsEditable | ItemSpecificFlags( entity ) );
        for( int col = 1; col < dataModel_.columnCount(); ++col )
            dataModel_.AddChildItem( parent, row, col );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::AddItem
// Created: JSR 2012-09-12
// -----------------------------------------------------------------------------
QStandardItem* HierarchyTreeView_ABC::AddItem( QStandardItem* parent, const kernel::Entity_ABC& entity )
{
    QStandardItem* ret = 0;
    if( AddItemIfPossible< kernel::Agent_ABC >( entity, parent, ret ) ||
        AddItemIfPossible< kernel::Automat_ABC >( entity, parent, ret ) ||
        AddItemIfPossible< kernel::Formation_ABC >( entity, parent, ret ) ||
        AddItemIfPossible< kernel::Team_ABC >( entity, parent, ret ) ||
        AddItemIfPossible< kernel::Ghost_ABC >( entity, parent, ret ) ||
        AddItemIfPossible< kernel::KnowledgeGroup_ABC >( entity, parent, ret ) )
        return ret;
    assert( false );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::InternalNotifyUpdated
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::InternalNotifyUpdated( const kernel::Hierarchies& hierarchy )
{
    const kernel::Entity_ABC& entity = hierarchy.GetEntity();
    QStandardItem* entityItem = dataModel_.FindDataItem( entity );
    if( entityItem )
    {
        AdditionalUpdateItem( *entityItem, entity );

        // Update superior if needed
        const kernel::Entity_ABC* newSuperior = hierarchy.GetSuperior();
        if( newSuperior )
        {
            QStandardItem* newSuperiorItem = dataModel_.FindDataItem( *newSuperior );
            if( newSuperiorItem )
            {
                QStandardItem* currentSuperiorItem = entityItem->parent();
                if( currentSuperiorItem && currentSuperiorItem != newSuperiorItem )
                {
                    QList< QStandardItem* > rowItems = currentSuperiorItem->takeRow( entityItem->row() );
                    newSuperiorItem->appendRow( rowItems );
                }
            }
            else
                throw std::exception( __FUNCTION__ "Error, superior not found." );
        }
    }
    else
        throw std::exception( __FUNCTION__ "Error, entity not found." );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::drawRow
// Created: JSR 2012-10-10
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::drawRow( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex &index ) const
{
    // TODO Move following to preparation or gui reimplementation, no ghost on gaming ... or not, all color could also be handle here (log missing, tacticaly destroyed, etc.), options are permissive for that, and it anticipate a gaming/prepa merge
    QStandardItem* item = dataModel_.GetItemFromIndex( dataModel_.GetMainModelIndex( index ) );
    if( item && item->data( StandardModel::MimeTypeRole).isValid() )
    {
        if( item->data( StandardModel::MimeTypeRole).toString() == typeid( kernel::Ghost_ABC ).name() )
        {
            QColor color( controllers_.options_.GetOption( "Color/Phantom", QString( "" ) ).To< QString >() );
            painter->fillRect( options.rect, color );
        }
    }
    RichTreeView::drawRow( painter, options, index );
}

namespace
{

    QPixmap* GetEmptyPixmap()
    {
        static QPixmap* emptyPixmap = 0;
        if( !emptyPixmap )
        {
            emptyPixmap = new QPixmap( 1, 32 );
            emptyPixmap->fill( Qt::transparent );
        }
        return emptyPixmap;
    }
}
// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::GetDecoration
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
const QPixmap* HierarchyTreeView_ABC::GetDecoration( const QModelIndex &index )
{
    const kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index );
    if( entity )
    {
        const kernel::Symbol_ABC* symbol = entity->Retrieve< kernel::TacticalHierarchies >();
        if( symbol )
        {
            const std::string symbolName = symbol->GetSymbol();
            const std::string levelName  = symbol->GetLevel();
            if( !symbolName.empty() || !levelName.empty() )
            {
                const QPixmap& pixmap = symbols_.GetSymbol( *entity, symbolName, levelName );
                if( !pixmap.isNull() )
                    return &pixmap;
                const_cast< HierarchyTreeView_ABC* >( this )->doItemsLayout();
            }
        }
        return GetEmptyPixmap();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::CanChangeSuperior
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
bool HierarchyTreeView_ABC::CanChangeSuperior( const kernel::Entity_ABC& /*entity*/, const kernel::Entity_ABC& /*superior*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::ItemSpecificFlags
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
Qt::ItemFlags HierarchyTreeView_ABC::ItemSpecificFlags( const kernel::Entity_ABC& entity ) const
{
    return entity.GetTypeName() == kernel::Team_ABC::typeName_ ? Qt::ItemIsDropEnabled : Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::MimeTypes
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
QStringList HierarchyTreeView_ABC::MimeTypes() const
{
    QStringList l;
    l << typeid( kernel::Agent_ABC ).name() << typeid( kernel::Automat_ABC ).name()
      << typeid( kernel::Formation_ABC ).name() << typeid( kernel::Ghost_ABC ).name()
      << typeid( kernel::KnowledgeGroup_ABC ).name() << typeid( kernel::AgentType ).name()
      << typeid( kernel::AutomatType ).name();
    return l;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::Drop
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::Drop( const QString& mimeType, void* data, QStandardItem& target )
{
    if( IsReadOnly() )
        return;
    kernel::Entity_ABC* entityTarget = dataModel_.GetDataFromItem< kernel::Entity_ABC >( target );
    if( !entityTarget )
        return;

    if( mimeType == typeid( kernel::AgentType ).name() )
    {
        const kernel::AgentType* type = reinterpret_cast< kernel::AgentType* >( data );
        if( type )
            Drop( *type, *entityTarget );
    }
    else if( mimeType == typeid( kernel::AutomatType ).name() )
    {
        const kernel::AutomatType* type = reinterpret_cast< kernel::AutomatType* >( data );
        if( type )
            Drop( *type, *entityTarget );
    }
    else
    {
        kernel::SafePointer< const kernel::Entity_ABC >* safePtr = reinterpret_cast< kernel::SafePointer< const kernel::Entity_ABC >* >( data );
        if( !safePtr || !*safePtr )
            return;
        if( mimeType == typeid( kernel::Agent_ABC ).name() )
            Drop( static_cast< const kernel::Agent_ABC& >( **safePtr ), *entityTarget );
        else if( mimeType == typeid( kernel::Automat_ABC ).name() )
            Drop( static_cast< const kernel::Automat_ABC& >( **safePtr ), *entityTarget );
        else if( mimeType == typeid( kernel::Formation_ABC ).name() )
            Drop( static_cast< const kernel::Formation_ABC& >( **safePtr ), *entityTarget );
        else if( mimeType == typeid( kernel::Ghost_ABC ).name() )
            Drop( static_cast< const kernel::Ghost_ABC& >( **safePtr ), *entityTarget );
        else if( mimeType == typeid( kernel::KnowledgeGroup_ABC ).name() )
            Drop( static_cast< const kernel::KnowledgeGroup_ABC& >( **safePtr ), *entityTarget );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::dragMoveEvent
// Created: JSR 2012-09-06
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::dragMoveEvent( QDragMoveEvent *pEvent )
{
    // TODO factoriser la méthode
    if( IsReadOnly()  )
    {
        pEvent->ignore();
        return;
    }
    //EntityTreeView_ABC::dragMoveEvent( pEvent );
    kernel::Entity_ABC* target = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( indexAt( pEvent->pos() ) );
    const QMimeData* mimeData = pEvent->mimeData();

    if( !target )
    {
        pEvent->ignore();
        return;
    }

    QStringList formats = mimeData->formats();
    foreach( QString format, formats )
    {
        QByteArray encodedData = mimeData->data( format );
        QDataStream stream( &encodedData, QIODevice::ReadOnly );
        while( !stream.atEnd() )
        {
            int ptr = 0;
            stream >> ptr;
            if( format == typeid( kernel::AgentType ).name() )
            {
                kernel::AgentType* type = reinterpret_cast< kernel::AgentType* >( ptr );
                if( type )
                {
                    if( const kernel::Ghost_ABC* ghost = dynamic_cast< const kernel::Ghost_ABC* >( target ) )
                        if( ghost->GetGhostType() == eGhostType_Agent )
                        {
                            pEvent->accept();
                            return;
                        }
                }
            }
            else if( format == typeid( kernel::AutomatType ).name() )
            {
                kernel::AutomatType* type = reinterpret_cast< kernel::AutomatType* >( ptr );
                if( type )
                {
                    if( const kernel::Ghost_ABC* ghost = dynamic_cast< const kernel::Ghost_ABC* >( target ) )
                        if( ghost->GetGhostType() == eGhostType_Automat )
                        {
                            pEvent->accept();
                            return;
                        }
                }
            }
            else
            {
                kernel::SafePointer< const kernel::Entity_ABC >* safePtr = reinterpret_cast< kernel::SafePointer< const kernel::Entity_ABC >* >( ptr );
                if( safePtr && *safePtr )
                {
                    const kernel::Entity_ABC* entity = *safePtr;
                    if( entity->GetId() != target->GetId() && CanChangeSuperior( *entity, *target ) && profile_.CanDoMagic( *entity ) && profile_.CanDoMagic( *target ))
                    {
                        pEvent->accept();
                        return;
                    }
                }
            }
        }
    }
    pEvent->ignore();
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::focusInEvent
// Created: LGY 2012-06-29
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::focusInEvent( QFocusEvent* event )
{
    EntityTreeView_ABC::focusInEvent( event );
    emit TreeViewFocusIn( this );
}
