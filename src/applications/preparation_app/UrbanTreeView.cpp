// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UrbanTreeView.h"
#include "moc_UrbanTreeView.cpp"
#include "clients_gui/ItemPixmapDelegate.h"
#include "clients_gui/ModelObserver_ABC.h"
#include "clients_gui/StandardModelVisitor_ABC.h"
#include "clients_gui/SymbolIcon.h"
#include "clients_gui/SymbolIcons.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "preparation/UrbanHierarchies.h"
#include "ENT/ENT_Enums_Gen.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: UrbanTreeView constructor
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
UrbanTreeView::UrbanTreeView( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, gui::SymbolIcons& symbols, QWidget* parent /*= 0*/ )
    : gui::EntityTreeView_ABC( controllers, profile, modelObserver, parent )
    , symbols_( symbols )
{
    dataModel_.setColumnCount( 2 );
    setUniformRowHeights( true );
    header()->setStretchLastSection( false );
    header()->setResizeMode( 0, QHeaderView::Stretch );
    header()->setResizeMode( 1, QHeaderView::Fixed );
    header()->resizeSection( 1, 24 );

    setItemDelegateForColumn( 1, new gui::ItemPixmapDelegate( dataModel_, boost::bind( &UrbanTreeView::GetEntityPixmap, this, _1 ), this, 24 ) );

    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView destructor
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
UrbanTreeView::~UrbanTreeView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::NotifyCreated
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void UrbanTreeView::NotifyCreated( const kernel::UrbanObject_ABC& object )
{
    const kernel::Hierarchies* hierarchies = object.Retrieve< kernel::Hierarchies >();
    if( !hierarchies ) // $$$$ ABR 2012-05-22: Only urban block have hierarchies
        return;
    const kernel::Entity_ABC* superior = hierarchies->GetSuperior();
    QStandardItem* item = 0;
    if( superior )
    {
        if( QStandardItem* supItem = dataModel_.FindSafeItem( *superior ) )
        {
            const int row = supItem->rowCount();
            item = dataModel_.AddChildSafeItem( supItem, row, 0, object.GetName(), object.GetTooltip(), object, Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled );
            dataModel_.AddChildItem( supItem, row, 1 );
        }
    }
    else
    {
        const int row = dataModel_.rowCount();
        item = dataModel_.AddRootSafeItem( row, 0, object.GetName(), object.GetTooltip(), object, Qt::ItemIsDropEnabled );
        dataModel_.AddRootItem( row, 1 );
    }
    if( item )
        item->setEditable( GetCurrentMode() == ePreparationMode_Terrain );
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::NotifyUpdated
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void UrbanTreeView::NotifyUpdated( const kernel::UrbanObject_ABC& /*object*/ )
{
    doItemsLayout();
}

namespace
{
    class ModelVisitor : public gui::StandardModelVisitor_ABC
                       , private boost::noncopyable
    {
    public:
        ModelVisitor( bool enable ) : enable_( enable ) {}
        ~ModelVisitor() {}
        virtual void Visit( QStandardItem& item )
        {
            item.setEditable( enable_ );
        }
    private:
        bool enable_;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::NotifyCreated
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void UrbanTreeView::NotifyCreated( const kernel::Team_ABC& /*team*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::NotifyModeChanged
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void UrbanTreeView::NotifyModeChanged( int newMode )
{
    ModesObserver_ABC::NotifyModeChanged( newMode );
    bool isTerrainMode = newMode == ePreparationMode_Terrain;
    setSelectionMode( isTerrainMode ? QAbstractItemView::ExtendedSelection : QAbstractItemView::SingleSelection );
    EnableDragAndDrop( isTerrainMode );
    ModelVisitor visitor( isTerrainMode );
    dataModel_.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::OnCreateCity
// Created: JSR 2012-09-17
// -----------------------------------------------------------------------------
void UrbanTreeView::OnCreateCity()
{
    modelObserver_.CreateCityOrDistrict( 0 );
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::GetEntityPixmap
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
const QPixmap* UrbanTreeView::GetEntityPixmap( const kernel::Entity_ABC& object )
{
    if( const kernel::Infrastructure_ABC* infra = object.Retrieve< kernel::Infrastructure_ABC >() )
    {
        const kernel::InfrastructureType* infraType = infra->GetType();
        if( infraType )
        {
            gui::SymbolIcon icon( infraType->GetSymbol() );
            icon.SetSize( 24 );
            const QPixmap& pixmap = symbols_.GetSymbol( icon );
            if( !pixmap.isNull() )
                return &pixmap;
            doItemsLayout();
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::ContextMenuRequested
// Created: JSR 2012-09-17
// -----------------------------------------------------------------------------
void UrbanTreeView::ContextMenuRequested( const QPoint& where )
{
    if( !IsReadOnly() && GetCurrentMode() == ePreparationMode_Terrain )
    {
        kernel::ContextMenu* menu = new kernel::ContextMenu( this );
        menu->insertItem( tr( "Create city" ), this, SLOT( OnCreateCity() ) );
        menu->exec( where );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::IsTypeRejected
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
bool UrbanTreeView::IsTypeRejected( const kernel::Entity_ABC& entity ) const
{
    return entity.GetTypeName() != kernel::UrbanObject_ABC::typeName_;
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::LessThan
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
bool UrbanTreeView::LessThan( const QModelIndex& left, const QModelIndex& right, bool& valid ) const
{
    kernel::Entity_ABC* entity1 = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( left );
    kernel::Entity_ABC* entity2 = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( right );
    if( !entity1 || !entity2 )
        return false;
    valid = true;
    return entity2->GetId() < entity1->GetId();
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::MimeData
// Created: JSR 2012-09-17
// -----------------------------------------------------------------------------
QMimeData* UrbanTreeView::MimeData( const QModelIndexList& indexes, bool& overriden ) const
{
    overriden = true;
    QMimeData* mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream( &encodedData, QIODevice::WriteOnly );
    EUrbanLevel level = static_cast< EUrbanLevel >( -1 );
    foreach( QModelIndex index, indexes )
    {
        if( index.isValid() )
        {
            kernel::UrbanObject_ABC* entity = dataModel_.GetDataFromIndex< kernel::UrbanObject_ABC >( index );
            if( entity )
            {
                UrbanHierarchies& hierarchies = static_cast< UrbanHierarchies& >( entity->Get< kernel::Hierarchies >() );
                if( level != static_cast< EUrbanLevel >( -1 ) && level != hierarchies.GetLevel() )
                {
                    delete mimeData;
                    return 0;
                }
                level = hierarchies.GetLevel();
                QStandardItem* item = dataModel_.itemFromIndex( index.model() == &dataModel_ ? index : proxyModel_->mapToSource( index ) );
                if( item )
                    stream << reinterpret_cast< unsigned int >( item->data( gui::StandardModel::DataRole ).value< kernel::VariantPointer >().ptr_ );
            }
        }
    }
    mimeData->setData( gui::StandardModel::mimeTypeStr_, encodedData );
    return mimeData;
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::dragMoveEvent
// Created: JSR 2012-09-17
// -----------------------------------------------------------------------------
void UrbanTreeView::dragMoveEvent( QDragMoveEvent *pEvent )
{
    if( IsReadOnly() )
    {
        pEvent->ignore();
        return;
    }
    EntityTreeView_ABC::dragMoveEvent( pEvent );
    QPoint position = viewport()->mapFromParent( pEvent->pos() );
    kernel::UrbanObject_ABC* target = dataModel_.GetDataFromIndex< kernel::UrbanObject_ABC >( indexAt( pEvent->pos() ) );
    if( !target )
    {
        pEvent->ignore();
        return;
    }
    EUrbanLevel levelTarget = static_cast< const UrbanHierarchies& >( target->Get< kernel::Hierarchies >() ).GetLevel();
    const QMimeData* mimeData = pEvent->mimeData();
    QStringList formats = mimeData->formats();
    foreach( QString format, formats )
    {
        if( format != gui::StandardModel::mimeTypeStr_ )
            continue;
        QByteArray encodedData = mimeData->data( format );
        QDataStream stream( &encodedData, QIODevice::ReadOnly );
        while( !stream.atEnd() )
        {
            int ptr = 0;
            stream >> ptr;
            kernel::SafePointer< const kernel::UrbanObject_ABC >* safePtr = reinterpret_cast< kernel::SafePointer< const kernel::UrbanObject_ABC >* >( ptr );
            if( safePtr && *safePtr )
            {
                const kernel::UrbanObject_ABC* entity = *safePtr;
                EUrbanLevel level = static_cast< const UrbanHierarchies& >( entity->Get< kernel::Hierarchies >() ).GetLevel();
                if( ( level == eUrbanLevelBlock && levelTarget == eUrbanLevelDistrict ) || ( level == eUrbanLevelDistrict && levelTarget == eUrbanLevelCity ) )
                {
                    pEvent->accept();
                    return;
                }
            }
        }
    }
    pEvent->ignore();
}

// -----------------------------------------------------------------------------
// Name: UrbanTreeView::Drop
// Created: JSR 2012-09-17
// -----------------------------------------------------------------------------
void UrbanTreeView::Drop( const QString& mimeType, void* data, QStandardItem& target )
{
    if( IsReadOnly() )
        return;
    kernel::Entity_ABC* entityTarget = dataModel_.GetDataFromItem< kernel::UrbanObject_ABC >( target );
    if( !entityTarget )
        return;
    if( mimeType == gui::StandardModel::mimeTypeStr_ )
    {
        kernel::SafePointer< kernel::UrbanObject_ABC >* safePtr = reinterpret_cast< kernel::SafePointer< kernel::UrbanObject_ABC >* >( data );
        if( !safePtr || !*safePtr )
            return;
        kernel::UrbanObject_ABC* object = safePtr->ConstCast();
        UrbanHierarchies& hierarchies = static_cast< UrbanHierarchies& >( object->Get< kernel::Hierarchies >() );
        kernel::Entity_ABC* superior = const_cast< kernel::Entity_ABC* >( hierarchies.GetSuperior() );
        if( !superior || superior == entityTarget )
            return;
        superior->Get< kernel::UrbanPositions_ABC >().ResetConvexHull();
        QStandardItem* superiorItem = dataModel_.FindSafeItem( *superior );
        if( !superiorItem )
            return;
        QModelIndex index = dataModel_.indexFromItem( dataModel_.FindSafeItem( *object ) );
        QList< QStandardItem* > rowItems = superiorItem->takeRow( index.row() );
        for( QList< QStandardItem *>::iterator it = rowItems.begin(); it != rowItems.end(); ++it )
            delete *it;
        static_cast< UrbanHierarchies& >( object->Get< kernel::Hierarchies >() ).ChangeSuperior( *entityTarget );
        NotifyCreated( *object );
        tools::Iterator< const kernel::Entity_ABC& > subIt = hierarchies.CreateSubordinateIterator();
        while( subIt.HasMoreElements() )
        {
            const kernel::UrbanObject_ABC& child = static_cast< const kernel::UrbanObject_ABC& >( subIt.NextElement() );
            NotifyCreated( child );
        }
        entityTarget->Get< kernel::UrbanPositions_ABC >().ResetConvexHull();
        controllers_.actions_.SetSelected( *entityTarget, false );
    }
}
