// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InhabitantPanel.h"
#include "moc_InhabitantPanel.cpp"
#include "gaming/Model.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/LivingArea_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/VariantPointer.h"
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: InhabitantPanel constructor
// Created: LGY 2011-12-29
// -----------------------------------------------------------------------------
InhabitantPanel::InhabitantPanel( QMainWindow* mainWindow, kernel::Controllers& controllers, Model& model )
    : QDockWidget( "Inhabitant", mainWindow )
    , controllers_( controllers )
    , model_      ( model )
    , current_    ( 0 )
{
    setObjectName( "ProfilesPanel" );
    QWidget* main = new QWidget( this );
    QVBoxLayout* mainLayout = new QVBoxLayout( main );
    setCaption( tools::translate( "InhabitantPanel", "Living area" ) );
    horizontalHeaders_ << tools::translate( "InhabitantPanel", "Name" ) << tools::translate( "InhabitantPanel", "Resident" )
                       << tools::translate( "InhabitantPanel", "Angriness" ) << tools::translate( "InhabitantPanel", "Alerted" )
                       << tools::translate( "InhabitantPanel", "Confined" ) << tools::translate( "InhabitantPanel", "Evacuated" );

    dataModel_ = new QStandardItemModel( this );
    dataModel_->setColumnCount( 6 );
    proxyModel_ = new QSortFilterProxyModel( this );
    proxyModel_->setDynamicSortFilter( true );
    proxyModel_->setSourceModel( dataModel_ );
    proxyModel_->setSortRole( Qt::UserRole );

    table_ = new QTableView( this );
    table_->setModel( proxyModel_ );
    table_->setSortingEnabled( true );
    table_->setSelectionBehavior( QAbstractItemView::SelectRows );
    table_->setSelectionMode( QAbstractItemView::SingleSelection );
    table_->setAlternatingRowColors( true );
    table_->horizontalHeader()->setVisible( false );
    table_->verticalHeader()->setVisible( false );
    table_->setEnabled( false );
    connect( table_, SIGNAL( doubleClicked( const QModelIndex & ) ), this, SLOT( OnSelectionChanged( const QModelIndex& ) ) );
    mainLayout->addWidget( table_ );
    setWidget( main );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel destructor
// Created: LGY 2011-12-29
// -----------------------------------------------------------------------------
InhabitantPanel::~InhabitantPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel::NotifySelected
// Created: LGY 2011-12-29
// -----------------------------------------------------------------------------
void InhabitantPanel::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( !entity || entity->GetTypeName() != kernel::Inhabitant_ABC::typeName_ )
    {
        dataModel_->clear();
        table_->horizontalHeader()->setVisible( false );
        table_->setEnabled( false );
    }
    else if( const kernel::LivingArea_ABC* area = entity->Retrieve< kernel::LivingArea_ABC >() )
         {
             int sort = proxyModel_->sortColumn();
             Qt::SortOrder order = proxyModel_->sortOrder();
             dataModel_->clear();
             dataModel_->setHorizontalHeaderLabels( horizontalHeaders_ );
             table_->horizontalHeader()->setVisible( true );
             area->Accept( *this );
             table_->setEnabled( true );
             current_ = entity;
             proxyModel_->sort( sort, order );
         }
}

namespace
{
    QString Convert( bool value )
    {
        return value ?  tools::translate( "InhabitantPanel", "Yes" ) : tools::translate( "InhabitantPanel", "No" );
    }

    template< typename T >
    QStandardItem* Create( T data, const QString& text, const kernel::Object_ABC* object )
    {
        QStandardItem* item = new QStandardItem( text );
        item->setTextAlignment( Qt::AlignVCenter | Qt::AlignHCenter );
        item->setFlags( item->flags() & ~Qt::ItemIsEditable );
        item->setData( data, Qt::UserRole );
        QVariant* variant = new QVariant();
        variant->setValue( kernel::VariantPointer( object ) );
        item->setData( *variant, Qt::UserRole + 1 );
        return item;
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel::Visit
// Created: LGY 2011-12-29
// -----------------------------------------------------------------------------
void InhabitantPanel::Visit( unsigned int id, const std::string& name, unsigned int resident, bool alerted,
                             float angriness, bool confined, bool evacuated )
{
    if( const kernel::Object_ABC* object = model_.FindUrbanObject( id ) )
    {
        QList< QStandardItem* > items;
        items.push_back( Create( name.c_str(), name.c_str(), object ) );
        items.push_back( Create( resident, QString::number( resident ), object ) );
        items.push_back( Create( angriness, QString::number( angriness ), object ) );
        items.push_back( Create( alerted, Convert( alerted ), object ) );
        items.push_back( Create( confined, Convert( confined ), object ) );
        items.push_back( Create( evacuated, Convert( evacuated ), object ) );
        dataModel_->appendRow( items );
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel::OnSelectionChanged
// Created: LGY 2011-12-30
// -----------------------------------------------------------------------------
void InhabitantPanel::OnSelectionChanged( const QModelIndex& index )
{
    const kernel::Object_ABC* object = static_cast< const kernel::Object_ABC* >( index.data( Qt::UserRole + 1 ).value< kernel::VariantPointer >().ptr_ );
    if( object )
        object->Activate( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPanel::NotifyUpdated
// Created: LGY 2011-12-30
// -----------------------------------------------------------------------------
void InhabitantPanel::NotifyUpdated( const kernel::LivingArea_ABC& area )
{
    if( current_ && current_->GetId() == area.GetId() )
    {
        int sort = proxyModel_->sortColumn();
        Qt::SortOrder order = proxyModel_->sortOrder();
        dataModel_->clear();
        dataModel_->setHorizontalHeaderLabels( horizontalHeaders_ );
        table_->horizontalHeader()->setVisible( true );
        area.Accept( *this );
        proxyModel_->sort( sort, order );
    }
}
