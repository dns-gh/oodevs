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
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/Options.h"
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
        QStandardItem* superiorItem = dataModel_.FindSafeItem( *superior );
        if( !superiorItem )
            superiorItem = AddItem( 0, *superior );
        entityItem = AddItem( superiorItem, entity );
    }
    else                // Root item
        entityItem = AddItem( 0, entity );

    if( entityItem )
        UpdateItem( *entityItem, entity );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::AddItem
// Created: JSR 2012-09-12
// -----------------------------------------------------------------------------
QStandardItem* HierarchyTreeView_ABC::AddItem( QStandardItem* parent, const kernel::Entity_ABC& entity )
{
    QStandardItem* ret;
    if( parent )
    {
        int row = parent->rowCount();
        ret = dataModel_.AddChildSafeItem( parent, row, 0, entity.GetName(), entity.GetTooltip(), entity, ItemSpecificFlags( entity ) );
        for( int col = 1; col < dataModel_.columnCount(); ++col )
            dataModel_.AddChildItem( parent, row, col );
    }
    else
    {
        int row = dataModel_.rowCount();
        ret = dataModel_.AddRootSafeItem( row, 0, entity.GetName(), entity.GetTooltip(), entity, ItemSpecificFlags( entity ) );
        for( int col = 1; col < dataModel_.columnCount(); ++col )
            dataModel_.AddRootItem( row, col );
    }
    return ret;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::InternalNotifyUpdated
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::InternalNotifyUpdated( const kernel::Hierarchies& hierarchy )
{
    const kernel::Entity_ABC& entity = hierarchy.GetEntity();
    QStandardItem* entityItem = dataModel_.FindSafeItem( entity );
    if( entityItem )
    {
        UpdateItem( *entityItem, entity );

        // Update superior if needed
        const kernel::Entity_ABC* newSuperior = hierarchy.GetSuperior();
        if( newSuperior )
        {
            QStandardItem* newSuperiorItem = dataModel_.FindSafeItem( *newSuperior );
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
// Name: HierarchyTreeView_ABC::UpdateItem
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::UpdateItem( QStandardItem& entityItem, const kernel::Entity_ABC& entity )
{
    UpdateBackgroundColor( entityItem, entity );
    AdditionalUpdateItem( entityItem, entity );
}

void HierarchyTreeView_ABC::UpdateBackgroundColor( QStandardItem& entityItem, const kernel::Entity_ABC& entity )
{
    // TODO Move following to preparation or gui reimplementation, no ghost on gaming ... or not, all color could also be handle here (log missing, tacticaly destroyed, etc.), options are permissive for that, and it anticipate a gaming/prepa merge
    QColor color = Qt::transparent;
    if( dynamic_cast< const kernel::Ghost_ABC* >( &entity ) != 0 )
        color = QColor( controllers_.options_.GetOption( "Color/Phantom", QString( "" ) ).To< QString >() );
    entityItem.setBackground( QBrush( color ) );
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
                static const QPixmap emptyPixmap( 32, 32 );
                return &emptyPixmap;
            }
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::ItemSpecificFlags
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
Qt::ItemFlags HierarchyTreeView_ABC::ItemSpecificFlags( const kernel::Entity_ABC& /*entity*/ ) const
{
    return Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
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
// Name: HierarchyTreeView_ABC::AdditionalMimeTypes
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
QStringList HierarchyTreeView_ABC::AdditionalMimeTypes() const
{
    QStringList l;
    l << typeid( kernel::AgentType ).name() << typeid( kernel::AutomatType ).name();
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

    if( mimeType == StandardModel::mimeTypeStr_ )
    {
        kernel::SafePointer< const kernel::Entity_ABC >* safePtr = reinterpret_cast< kernel::SafePointer< const kernel::Entity_ABC >* >( data );
        if( !safePtr || !*safePtr )
            return;
        const kernel::Entity_ABC* entity = *safePtr;
        if( const kernel::Agent_ABC* agent = dynamic_cast< const kernel::Agent_ABC* >( entity ) )
            Drop( *agent, *entityTarget );
        else if( const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( entity ) )
            Drop( *automat, *entityTarget );
        else if( const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( entity ) )
            Drop( *formation, *entityTarget );
        else if( const kernel::Ghost_ABC* ghost = dynamic_cast< const kernel::Ghost_ABC* >( entity ) )
            Drop( *ghost, *entityTarget );
        else if( const kernel::KnowledgeGroup_ABC* knowledgeGroup = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( entity ) )
            Drop( *knowledgeGroup, *entityTarget );
    }
    else if( mimeType == typeid( kernel::AgentType ).name() )
    {

        const kernel::AgentType* type = *reinterpret_cast< const kernel::AgentType** >( data );
        if( type )
            Drop( *type, *entityTarget );
    }
    else if( mimeType == typeid( kernel::AutomatType ).name() )
    {
        const kernel::AutomatType* type = *reinterpret_cast< const kernel::AutomatType** >( data );
        if( type )
            Drop( *type, *entityTarget );
    }
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView_ABC::dragMoveEvent
// Created: JSR 2012-09-06
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::dragMoveEvent( QDragMoveEvent *pEvent )
{
    // TODO factoriser la méthode
    if( IsReadOnly() )
    {
        pEvent->ignore();
        return;
    }
    EntityTreeView_ABC::dragMoveEvent( pEvent );
    QPoint position = viewport()->mapFromParent( pEvent->pos() );
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
            if( format == StandardModel::mimeTypeStr_ )
            {
                kernel::SafePointer< const kernel::Entity_ABC >* safePtr = reinterpret_cast< kernel::SafePointer< const kernel::Entity_ABC >* >( ptr );
                if( safePtr && *safePtr )
                {
                    const kernel::Entity_ABC* entity = *safePtr;
                    if( entity->GetId() != target->GetId() && CanChangeSuperior( *entity, *target ) )
                    {
                        pEvent->accept();
                        return;
                    }
                }
            }
            else if( format == typeid( kernel::AgentType ).name() )
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
        }
    }
    pEvent->ignore();
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView_ABC::focusInEvent
// Created: LGY 2012-06-29
// -----------------------------------------------------------------------------
void HierarchyTreeView_ABC::focusInEvent( QFocusEvent* event )
{
    EntityTreeView_ABC::focusInEvent( event );
    emit TreeViewFocusIn( this );
}
