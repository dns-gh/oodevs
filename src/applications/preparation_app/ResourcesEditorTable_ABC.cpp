// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ResourcesEditorTable_ABC.h"
#include "moc_ResourcesEditorTable_ABC.cpp"
#include "clients_gui/CommonDelegate.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/SignalAdapter.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include <boost/bind.hpp>

Q_DECLARE_METATYPE( const kernel::DotationType* )

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC Constructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
ResourcesEditorTable_ABC::ResourcesEditorTable_ABC( const QStringList& headers, const QString& objectName, QWidget* parent, const kernel::Resolver2< kernel::DotationType >& dotationsType )
    : gui::RichWidget< QTableView >( objectName, parent )
    , headers_( headers )
    , dotations_( dotationsType )
{
    dataModel_  = new QStandardItemModel( this );
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel( this );
    proxyModel->setSourceModel( dataModel_ );
    proxyModel->setSortRole( Qt::UserRole + 1 );
    gui::CommonDelegate* delegate = new gui::CommonDelegate( this );

    setModel( proxyModel );
    setItemDelegate( delegate );
    setAlternatingRowColors( true );
    setSelectionMode( SingleSelection );
    setSelectionBehavior( SelectRows );
    setSortingEnabled( true );
    verticalHeader()->hide();

    Connect();

    InitHeader();
    delegate->AddSpinBoxOnColumn( dataModel_->columnCount() - 1, 0, std::numeric_limits< int >::max() );
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC Destructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
ResourcesEditorTable_ABC::~ResourcesEditorTable_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::Connect
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::Connect()
{
    connect( dataModel_ , SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ), SLOT( OnDataChanged( const QModelIndex&, const QModelIndex& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::Disconnect
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::Disconnect()
{
    disconnect( dataModel_ , SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( OnDataChanged( const QModelIndex&, const QModelIndex& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::contextMenuEvent
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::contextMenuEvent( QContextMenuEvent* event )
{
    kernel::ContextMenu* menu = new kernel::ContextMenu( this );
    connect( menu, SIGNAL( aboutToHide() ), menu, SLOT( deleteLater() ) );

    QMenu* resourcesMenu = menu->addMenu( tr( "Add supplies" ) );
    const QModelIndex indexClicked = indexAt( event->pos() );
    if( indexClicked.isValid() )
    {
        QAction* action = menu->addAction( tr( "Remove supplies" ) );
        gui::connect( action, SIGNAL( triggered() ), boost::bind( &ResourcesEditorTable_ABC::RemoveResource, this, indexClicked.row() ) );
    }
    menu->addAction( tr( "Clear list" ), this, SLOT( OnClearItems() ) );

    std::set< const kernel::DotationType* > currentDotations;
    for( int row = 0; row < dataModel_->rowCount(); ++row )
        currentDotations.insert( GetDotation( row ) );

    std::map< std::string, std::map< std::string, const kernel::DotationType* > > dotationsByType;
    auto it = dotations_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const auto& dotation = it.NextElement();
        if( currentDotations.find( &dotation ) == currentDotations.end() &&
            allowedSupplyClasses_.find( dotation.GetLogisticSupplyClass().GetName() ) != allowedSupplyClasses_.end() )
            dotationsByType[ dotation.GetCategoryDisplay() ][ dotation.GetName() ] = &dotation ;
    }

    for( auto it = dotationsByType.begin(); it != dotationsByType.end(); ++it )
    {
        kernel::ContextMenu* pTypeMenu = new kernel::ContextMenu( resourcesMenu );
        pTypeMenu->setTitle( QString::fromStdString( it->first ) );
        resourcesMenu->addMenu( pTypeMenu );
        for( auto itD = it->second.begin(); itD != it->second.end(); ++itD )
        {
            const kernel::DotationType* pDotation = itD->second;
            QAction* action = pTypeMenu->addAction( QString::fromStdString( pDotation->GetName() ) );
            gui::connect( action, SIGNAL( triggered() ), boost::bind( &ResourcesEditorTable_ABC::AddResource, this, boost::cref( *pDotation ), nullptr, 0 ) );
            CustomizeMenuAction( action, *pDotation );
        }
    }
    menu->popup( event->globalPos() );
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::RemoveResource
// Created: JSR 2014-03-03
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::RemoveResource( int row )
{
    dataModel_->removeRow( row );
    emit ResourceValueChanged();
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::OnClearItems
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::OnClearItems()
{
    dataModel_->clear();
    InitHeader();
    emit ResourceValueChanged();
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::OnDataChanged
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::OnDataChanged( const QModelIndex& index, const QModelIndex& )
{
    if( index.column() == dataModel_->columnCount() - 1 )
    {
        UpdateLine( index.row(), GetValue( index.row() ) );
        emit ResourceValueChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::ComputeValueByDotation
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::ComputeValueByDotation( std::map< const kernel::DotationType*, unsigned int >& result ) const
{
    if( dataModel_->columnCount() < 2 )
        return;
    for( int i = 0; i < dataModel_->rowCount(); ++i )
        result[ GetDotation( i ) ] += GetValue( i );
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::AddResource
// Created: JSR 2014-03-03
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::AddResource( const kernel::DotationType& resource, const kernel::Entity_ABC* /*entity*/, int value /*= 0*/ )
{
    const int newRowIndex = dataModel_->rowCount();
    dataModel_->setRowCount( newRowIndex + 1 );
    const int lastColumn = dataModel_->columnCount() - 1;
    const QString resourceName = resource.GetName().c_str();
    SetData( newRowIndex, 0, resourceName, Qt::DisplayRole, true );
    SetData( newRowIndex, 0, QVariant::fromValue( &resource ), Qt::UserRole );
    SetData( newRowIndex, lastColumn, value, Qt::DisplayRole, true, Qt::AlignRight | Qt::AlignVCenter );
    SetData( newRowIndex, lastColumn, value, Qt::UserRole, true, Qt::AlignRight | Qt::AlignVCenter );
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::InitHeader
// Created: JSR 2014-03-03
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::InitHeader()
{
    dataModel_->setHorizontalHeaderLabels( headers_ );
    horizontalHeader()->setResizeMode( QHeaderView::Interactive );
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::UpdateLine
// Created: JSR 2014-03-03
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::UpdateLine( int /*row*/, int /*value*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::CustomizeMenuAction
// Created: JSR 2014-03-04
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::CustomizeMenuAction( QAction* /*action*/, const kernel::DotationType& /*actionDotation*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::SetData
// Created: JSR 2014-03-03
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::SetData( int row, int col, const QVariant& value, int role /* = Qt::DisplayRole */, bool isSortType /* = false */, Qt::Alignment aligment /* = 0 */ )
{
    dataModel_->setData( dataModel_->index( row, col ), value, role );
    if( isSortType && role != Qt::UserRole + 1 )
        dataModel_->setData( dataModel_->index( row, col ), value, Qt::UserRole + 1 );
    if( aligment )
        dataModel_->item( row, col )->setTextAlignment( aligment );
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::GetDotation
// Created: JSR 2014-03-03
// -----------------------------------------------------------------------------
const kernel::DotationType* ResourcesEditorTable_ABC::GetDotation( int row ) const
{
    QStandardItem* item = dataModel_->item( row );
    return item ? item->data( Qt::UserRole ).value< const kernel::DotationType* >() : 0;
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::UpdateAllowedSupplyClasses
// Created: JSR 2014-03-12
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::UpdateAllowedSupplyClasses( const kernel::Entity_ABC& entity )
{
    allowedSupplyClasses_.clear();
    logistic_helpers::VisitAgentsWithLogisticSupply( entity, [&]( const kernel::Agent_ABC& agent )
        {
            agent.GetType().GetAllowedSupplyClasses( allowedSupplyClasses_ );
        } );
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::GetValue
// Created: JSR 2014-03-03
// -----------------------------------------------------------------------------
int ResourcesEditorTable_ABC::GetValue( int row ) const
{
    if( dataModel_->columnCount() < 2 )
        return 0;
    return dataModel_->item( row, dataModel_->columnCount() - 1 )->data( Qt::UserRole ).toInt();
}
