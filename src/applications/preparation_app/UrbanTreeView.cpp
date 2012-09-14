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
#include "clients_gui/StandardModelVisitor_ABC.h"
#include "clients_gui/ItemPixmapDelegate.h"
#include "clients_gui/SymbolIcon.h"
#include "clients_gui/SymbolIcons.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/UrbanObject_ABC.h"
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
            item = dataModel_.AddChildSafeItem( supItem, row, 0, object.GetName(), object.GetTooltip(), object );
            dataModel_.AddChildItem( supItem, row, 1 );
        }
    }
    else
        item = dataModel_.AddRootSafeItem( dataModel_.rowCount(), 0, object.GetName(), object.GetTooltip(), object );
    if( item )
    {
        item->setEditable( GetCurrentMode() == ePreparationMode_Terrain );
        /* todo*/
        /*if( superior )
            item->setDragEnabled( true );*/
    }
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
            if( pixmap.isNull() )
            {
                doItemsLayout();
                return 0;
            }
            else
                return &pixmap;
        }
    }
    return 0;
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
