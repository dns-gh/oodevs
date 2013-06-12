// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticConsignsWidget_ABC.h"
#include "moc_LogisticConsignsWidget_ABC.cpp"
#include "clients_gui/LinkItemDelegate.h"

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC constructor
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
LogisticConsignsWidget_ABC::LogisticConsignsWidget_ABC( QWidget* parent, kernel::DisplayExtractor_ABC& extractor )
    : QWidget( parent )
    , extractor_( extractor )
{
    QVBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->setSizeConstraint( QLayout::SetMaximumSize );

    pConsignTreeView_ = CreateTreeWidget( tools::translate( "LogisticConsignsWidget_ABC", "Logistic requests" ) );
    gui::LinkItemDelegate* consignsDelegate = new gui::LinkItemDelegate( pConsignTreeView_ );
    pConsignTreeView_->setItemDelegateForColumn( 1, consignsDelegate );
    connect( consignsDelegate, SIGNAL( LinkClicked( const QString&, const QModelIndex& ) )
                             , SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );
    connect( pConsignTreeView_, SIGNAL( itemExpanded( QTreeWidgetItem* ) ), SLOT( OnItemExpanded( QTreeWidgetItem* ) ) );
    pLayout->addWidget( pConsignTreeView_ );

    pConsignHandledTreeView_ = CreateTreeWidget( tools::translate( "LogisticConsignsWidget_ABC", "Processing instructions" ) );
    gui::LinkItemDelegate* handledDelegate = new gui::LinkItemDelegate( pConsignHandledTreeView_ );
    pConsignHandledTreeView_->setItemDelegateForColumn( 1, handledDelegate );
    connect( handledDelegate, SIGNAL( LinkClicked( const QString&, const QModelIndex& ) )
                            , SLOT( OnHandledLinkClicked( const QString&, const QModelIndex& ) ) );
    connect( pConsignHandledTreeView_, SIGNAL( itemExpanded( QTreeWidgetItem* ) ), SLOT( OnItemExpanded( QTreeWidgetItem* ) ) );
    pLayout->addWidget( pConsignHandledTreeView_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC destructor
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
LogisticConsignsWidget_ABC::~LogisticConsignsWidget_ABC()
{
    // NOTHING
}

namespace
{
    void NotifyLink( const QString& url, const QModelIndex& index, QTreeWidget& widget, kernel::DisplayExtractor_ABC& extractor )
    {
        widget.selectionModel()->setCurrentIndex( index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
        extractor.NotifyLinkClicked( url + "#select" );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::OnLinkClicked
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::OnLinkClicked( const QString& url, const QModelIndex& index )
{
    if( pConsignTreeView_ )
        NotifyLink( url, index, *pConsignTreeView_, extractor_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::OnLinkClicked
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::OnHandledLinkClicked( const QString& url, const QModelIndex& index )
{
    if( pConsignHandledTreeView_ )
        NotifyLink( url, index, *pConsignHandledTreeView_, extractor_ );
}

namespace
{
    void ExpandAll( QTreeWidgetItem& item )
    {
        int count = item.childCount();
        for( int i = 0; i < count; ++i )
        {
            QTreeWidgetItem* pChild = item.child( i );
            if( pChild )
            {
                pChild->setExpanded( true );
                ExpandAll( *pChild );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::OnLinkClicked
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::OnItemExpanded( QTreeWidgetItem* item )
{
    if( item && item->parent() )
        ExpandAll( *item );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::CreateTreeWidget
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
QTreeWidget* LogisticConsignsWidget_ABC::CreateTreeWidget( const QString& title )
{
    QTreeWidget* pTreeView = new QTreeWidget( this );
    pTreeView->setEnabled( true );
    pTreeView->setColumnCount( 2 );
    QStringList headers; headers << title << "";
    pTreeView->setHeaderLabels( headers );
    pTreeView->setRootIsDecorated( false );
    pTreeView->header()->setResizeMode( 0, QHeaderView::Stretch );
    pTreeView->setMouseTracking( true );
    pTreeView->setAllColumnsShowFocus( true );
    return pTreeView;
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::SetTreeWidgetItemData
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
void LogisticConsignsWidget_ABC::SetTreeWidgetItemData( QTreeWidgetItem& item, const QString& key, const QString& value )
{
    item.setText( 0, key );
    item.setText( 1, value );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::AddTreeWidgetItem
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
QTreeWidgetItem* LogisticConsignsWidget_ABC::AddTreeWidgetItem( QTreeWidgetItem& parent, const QString& key, const QString& value )
{
    int count = parent.childCount();
    for( int i = 0; i < count; ++i )
    {
        QTreeWidgetItem* pItem = parent.child( i );
        if( pItem && pItem->text( 0 ) == key )
        {
            pItem->setText( 1, value );
            return pItem;
        }
    }
    QTreeWidgetItem* pChild = new QTreeWidgetItem( &parent );
    pChild->setTextColor( 0, QColor().black() );
    pChild->setText( 0, key );
    pChild->setText( 1, value );
    return pChild;
}
