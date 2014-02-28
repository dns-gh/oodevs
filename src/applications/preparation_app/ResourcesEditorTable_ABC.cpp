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
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/DotationType.h"

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC Constructor
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
ResourcesEditorTable_ABC::ResourcesEditorTable_ABC( const QString& objectName, QWidget* parent, const kernel::Resolver2< kernel::DotationType >& dotationsType ) 
    : gui::RichWidget< QTableView >( objectName, parent )
    , dotations_( dotationsType )
    , popupMenu_( new kernel::ContextMenu( this ) )
{
    dataModel_  = new QStandardItemModel( parent );
    delegate_   = new gui::CommonDelegate( parent );

    setModel( dataModel_ );
    setItemDelegate( delegate_ );
    setAlternatingRowColors( true );
    setSelectionMode( SingleSelection );
    setSelectionBehavior( SelectRows );
    verticalHeader()->hide();

    resourcesMenu_ = popupMenu_->addMenu( "Add resource" );
    popupMenu_->insertItem( tr( "Remove resource" ), this, SLOT( OnRemoveCurrentItem() ) );
    popupMenu_->insertItem( tr( "Clear list" ), this, SLOT( OnClearItems() ) );

    Connect();
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
    popupMenu_->popup( event->globalPos() );
    possiblyToRemove_ = indexAt( event->pos() );
    resourcesMenu_->clear();

    std::set< int > currentDotations;
    for( int row = 0; row < dataModel_->rowCount(); ++row )
        currentDotations.insert( dataModel_->item( row )->data( Qt::UserRole ).toInt() );

    std::map< std::string, std::map< std::string, const kernel::DotationType* > > dotationsByType;
    auto it = dotations_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const auto& dotation = it.NextElement();
        if( currentDotations.find( dotation.GetId() ) == currentDotations.end() )
            dotationsByType[ dotation.GetCategoryDisplay() ][ dotation.GetName() ] = &dotation ;
    }

    for( auto it = dotationsByType.begin(); it != dotationsByType.end(); ++it )
    {
        auto pTypeMenu = new kernel::ContextMenu( resourcesMenu_ );
        resourcesMenu_->insertItem( QString::fromStdString( it->first ), pTypeMenu );
        for( auto itD = it->second.begin(); itD != it->second.end(); ++itD )
        {
            auto pDotation = itD->second;
            int itemId = pTypeMenu->insertItem( QString::fromStdString( pDotation->GetName() ), this, SLOT( AddLine( int ) ) );
            pTypeMenu->setItemParameter( itemId, pDotation->GetId() );

            if( !allowedNatures_.empty() && allowedNatures_.find( pDotation->GetNature() ) == allowedNatures_.end() )
                if( QMenuItem* pItem = pTypeMenu->findItem( itemId ) )
                    pItem->setFont( QFont( "Arial", 8, 3, true ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::AddLine
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::AddLine( int dotationId )
{
    if( auto pDotation = dotations_.Find( dotationId ) )
        AddResource( *pDotation );
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::OnRemoveCurrentItem
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::OnRemoveCurrentItem()
{
    if( possiblyToRemove_.isValid() )
    {
        dataModel_->removeRow( possiblyToRemove_.row() );
        emit ResourceValueChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::OnClearItems
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::OnClearItems()
{
    possiblyToRemove_ = QModelIndex();
    dataModel_->clear();
    allowedNatures_.clear();
    InitHeader();
    emit ResourceValueChanged();
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::OnDataChanged
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void  ResourcesEditorTable_ABC::OnDataChanged( const QModelIndex& index, const QModelIndex& )
{
    if( dataModel_->columnCount() < 2 )
        return;
    int column = dataModel_->columnCount() - 1;
    if( index.column() != column )
        return;
    double value = dataModel_->item( index.row(), column )->data( Qt::UserRole ).toDouble();    
    OnValueChanged( index.row(), value );
    emit ResourceValueChanged();
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::ComputeValueByDotation
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::ComputeValueByDotation( std::map< const kernel::DotationType*, double >& result ) const
{
    if( dataModel_->columnCount() < 2 )
        return;
    for( int i = 0; i < dataModel_->rowCount(); ++i )
    {
        int dotationId = dataModel_->item( i, 0 )->data( Qt::UserRole ).toInt();
        double value = dataModel_->item( i, dataModel_->columnCount() - 1 )->data( Qt::UserRole ).toDouble();
        if( auto pDotation = dotations_.Find( dotationId ) )
            result[ pDotation ] += value;
    }
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::SetAllowedNatures
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
void ResourcesEditorTable_ABC::SetAllowedNatures( const std::set< std::string >& allowedNatures )
{
    allowedNatures_ = allowedNatures;
}

// -----------------------------------------------------------------------------
// Name: ResourcesEditorTable_ABC::GetDataModel
// Created: MMC 2013-10-24
// -----------------------------------------------------------------------------
QStandardItemModel* ResourcesEditorTable_ABC::GetDataModel() const
{
    return dataModel_;
}
