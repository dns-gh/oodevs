// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FireResultListView.h"
#include "moc_FireResultListView.cpp"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/LinkItemDelegate.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/PopulationFireResult.h"
#include "gaming/AgentFireResult.h"
#include "gaming/Equipment.h"
#include "gaming/Casualties.h"
#include "gaming/Explosions.h"

// -----------------------------------------------------------------------------
// Name: FireResultListView constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireResultListView::FireResultListView( QWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor )
    : QTreeWidget( parent )
    , controllers_( controllers )
    , extractor_( extractor )
    , selected_( controllers )
{
    setFrameStyle( Q3Frame::Plain );
    setMouseTracking( true );
    setColumnCount( 4 );
    setHeaderHidden( true );
    setRootIsDecorated( false );
    gui::LinkItemDelegate* delegate = new gui::LinkItemDelegate( this );
    setItemDelegateForColumn( 1, delegate );
    setAllColumnsShowFocus( true );
    header()->setResizeMode( 0, QHeaderView::ResizeToContents );
    header()->setResizeMode( 1, QHeaderView::ResizeToContents );
    connect( delegate, SIGNAL( LinkClicked( const QString&, const QModelIndex& ) ), SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireResultListView::~FireResultListView()
{
    controllers_.Unregister( *this );
}

namespace
{
    template< typename T >
    int GetCount( const T& explosions )
    {
        int c = 0;
        for( T::const_iterator it = explosions.begin(); it != explosions.end(); ++it )
        {
            if( *it )
                ++c;
        }
        return c;
    }

    void SetNumberOfChildren( QTreeWidgetItem* item, int number )
    {
        if( item )
        {
            while( item->childCount() > number )
                delete item->takeChild( number );
            while( item->childCount() < number )
                item->addChild( new QTreeWidgetItem );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::OnLinkClicked
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
void FireResultListView::OnLinkClicked( const QString& url, const QModelIndex& index )
{
    selectionModel()->setCurrentIndex( index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
    extractor_.NotifyLinkClicked( url );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
void FireResultListView::Display( const AgentFireResult& result, QTreeWidgetItem* item )
{
    assert( item );
    if( !item )
        return;
    item->setText( 0, extractor_.GetDisplayName( result.time_ ) );
    item->setText( 1, extractor_.GetDisplayName( result.target_ ) );
    if( item->childCount() == 0 )
    {
        QStringList equipments;
        equipments << tools::translate( "FireResultListView", "Equipments" )
                   << tools::translate( "FireResultListView", "( avail, " )
                   << tools::translate( "FireResultListView", "unavail," )
                   << tools::translate( "FireResultListView", "repairable )" );
        item->addChild( new QTreeWidgetItem( equipments ) );
        QStringList troops;
        troops << tools::translate( "FireResultListView", "Troops" )
               << tools::translate( "FireResultListView", "( officer," )
               << tools::translate( "FireResultListView", "warrant-off.," )
               << tools::translate( "FireResultListView", "private )" );
        item->addChild( new QTreeWidgetItem( troops ) );
        item->child( 0 )->setTextAlignment( 1, Qt::AlignCenter );
        item->child( 0 )->setTextAlignment( 2, Qt::AlignCenter );
        item->child( 0 )->setTextAlignment( 3, Qt::AlignCenter );
        item->child( 1 )->setTextAlignment( 1, Qt::AlignCenter );
        item->child( 1 )->setTextAlignment( 2, Qt::AlignCenter );
        item->child( 1 )->setTextAlignment( 3, Qt::AlignCenter );
    }
    assert( item->childCount() == 2 );
    SetNumberOfChildren( item->child( 0 ), result.Count() );
    SetNumberOfChildren( item->child( 1 ), eNbrHumanWound );

    int row = 0;
    tools::Iterator< const Equipment& > it =  result.CreateIterator();
    while( it.HasMoreElements() )
        Display( it.NextElement(), item->child( 0 )->child( row++ ) );
    for( row = 0; row < eNbrHumanWound; ++row )
        Display( result.casualties_[ row ], item->child( 1 )->child( row ) );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
void FireResultListView::Display( const PopulationFireResult& result, QTreeWidgetItem* item )
{
    assert( item );
    if( !item )
        return;
    item->setText( 0, extractor_.GetDisplayName( result.time_ ) );
    item->setText( 1, extractor_.GetDisplayName( result.target_ ) );
    int lines = ( result.deadPeople_ == 0 ? 0 : 1 ) + ( result.woundedPeople_ == 0 ? 0 : 1 ) + ( result.scatteredPeople_ == 0 ? 0 : 1 );
    SetNumberOfChildren( item, lines );
    int row = 0;
    if( result.deadPeople_ != 0 )
    {
        item->child( row )->setText( 1, tools::translate( "FireResultListView", "Dead:" ) );
        item->child( row )->setText( 2, extractor_.GetDisplayName( result.deadPeople_ ) );
        ++row;
    }
    if( result.woundedPeople_ != 0 )
    {
        item->child( row )->setText( 1, tools::translate( "FireResultListView", "Wounded:" ) );
        item->child( row )->setText( 2, extractor_.GetDisplayName( result.woundedPeople_ ) );
        ++row;
    }
    if( result.scatteredPeople_ != 0 )
    {
        item->child( row )->setText( 1, tools::translate( "FireResultListView", "Scattered:" ) );
        item->child( row )->setText( 2, extractor_.GetDisplayName( result.scatteredPeople_ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
void FireResultListView::Display( const Equipment& equipment, QTreeWidgetItem* item )
{
    item->setText( 0, extractor_.GetDisplayName( equipment.type_ ) );
    item->setText( 1, extractor_.GetDisplayName( equipment.available_ ) );
    item->setTextAlignment( 1, Qt::AlignCenter );
    item->setText( 2, extractor_.GetDisplayName( equipment.unavailable_ ) );
    item->setTextAlignment( 2, Qt::AlignCenter );
    item->setText( 3, extractor_.GetDisplayName( equipment.repairable_ + equipment.onSiteFixable_ ) );
    item->setTextAlignment( 3, Qt::AlignCenter );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
void FireResultListView::Display( const Casualties& casualties, QTreeWidgetItem* item )
{
    item->setText( 0, extractor_.GetDisplayName( casualties.wound_ ) );
    item->setText( 1, extractor_.GetDisplayName( casualties.officers_ ) );
    item->setTextAlignment( 1, Qt::AlignCenter );
    item->setText( 2, extractor_.GetDisplayName( casualties.subOfficers_ ) );
    item->setTextAlignment( 2, Qt::AlignCenter );
    item->setText( 3, extractor_.GetDisplayName( casualties.troopers_ ) );
    item->setTextAlignment( 3, Qt::AlignCenter );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::NotifySelected
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void FireResultListView::NotifySelected( const kernel::Entity_ABC* element )
{
    if( element != selected_ )
    {
        selected_ = element;
        const Explosions* results = selected_ ? selected_->Retrieve< Explosions >() : 0;
        if( results )
            NotifyUpdated( *results );
        else
            clear();
    }
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::NotifyUpdated
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void FireResultListView::NotifyUpdated( const Explosions& results )
{
    if( selected_ && selected_->Retrieve< Explosions >() == &results )
    {
        const int count = GetCount( results.GetAgentExplosions() ) + GetCount( results.GetPopulationExplosions() );
        SetNumberOfChildren( invisibleRootItem(), count );
        int row = 0;
        for( Explosions::CIT_AgentFires it = results.GetAgentExplosions().begin(); it != results.GetAgentExplosions().end(); ++it )
            if( *it )
                Display( **it, topLevelItem( row++ ) );
        for( Explosions::CIT_PopulationFires it = results.GetPopulationExplosions().begin(); it != results.GetPopulationExplosions().end(); ++it )
            if( *it )
                Display( **it, topLevelItem( row++ ) );
    }
}
