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
#include "clients_gui/InternalLinks.h"
#include "clients_gui/LinkItemDelegate.h"
#include "clients_gui/Roles.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/KnowledgeConverter_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "gaming/PopulationFireResult.h"
#include "gaming/AgentFireResult.h"
#include "gaming/Equipment.h"
#include "gaming/Casualties.h"
#include "gaming/Explosions.h"

Q_DECLARE_METATYPE( const kernel::Entity_ABC* );

// -----------------------------------------------------------------------------
// Name: FireResultListView constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
FireResultListView::FireResultListView( QWidget* parent, kernel::Controllers& controllers,
    gui::DisplayExtractor& extractor, const kernel::KnowledgeConverter_ABC& converter )
    : QTreeView( parent )
    , controllers_( controllers )
    , extractor_( extractor )
    , converter_( converter )
    , selected_( controllers )
{
    model_.setHorizontalHeaderLabels( QStringList() << tr( "Date" )
                                                    << tr( "Firer" )
                                                    << tr( "Target" )
                                                    << "" );
    header()->setResizeMode( QHeaderView::ResizeToContents );

    proxy_.setSourceModel( &model_ );
    proxy_.setDynamicSortFilter( true );
    proxy_.setSortRole( gui::Roles::SortRole );
    proxy_.sort( 0, Qt::DescendingOrder );
    setModel( &proxy_ );
    setSortingEnabled( true );
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setSelectionMode( QAbstractItemView::SingleSelection );
    setSortingEnabled( false );

    gui::LinkItemDelegate* delegate = new gui::LinkItemDelegate( this );
    setItemDelegateForColumn( 1, delegate );
    setItemDelegateForColumn( 2, delegate );

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
// Name: FireResultListView::GetFirerName
// Created: ABR 2014-08-25
// -----------------------------------------------------------------------------
QString FireResultListView::GetFirerName( const kernel::Entity_ABC* firer ) const
{
    if( !firer )
        return "";
    const std::string& typeName = firer->GetTypeName();
    if( typeName == kernel::Agent_ABC::typeName_ )
        return GetDisplayName( *static_cast< const kernel::Agent_ABC* >( firer ) );
    else if( typeName == kernel::Population_ABC::typeName_ )
        return extractor_.GetDisplayName( *static_cast< const kernel::Population_ABC* >( firer ) );
    else if( typeName == kernel::Object_ABC::typeName_ )
        return extractor_.GetDisplayName( *static_cast< const kernel::Object_ABC* >( firer ) );
    return "";
}

QString FireResultListView::GetDisplayName( const kernel::Agent_ABC& agent ) const
{
    if( selected_ )
        if( auto k = converter_.FindAgent( agent.GetId(), *selected_ ) )
            return gui::InternalLinks::CreateLink( *k, k->GetName() );
    return extractor_.GetDisplayName( agent );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::AddRoot
// Created: ABR 2014-08-25
// -----------------------------------------------------------------------------
QStandardItem* FireResultListView::AddRoot( int row,
                                            int col,
                                            const QString& text,
                                            const kernel::Entity_ABC* entity /* = 0 */ )
{
    if( auto root = model_.invisibleRootItem() )
    {
        auto item = AddChild( *root, row, col, text );
        if( entity )
            item->setData( QVariant::fromValue( entity ), Qt::UserRole );
        return item;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::AddChild
// Created: ABR 2014-08-25
// -----------------------------------------------------------------------------
QStandardItem* FireResultListView::AddChild( QStandardItem& root,
                                             int row,
                                             int col,
                                             const QString& text,
                                             bool bold /* = false */ )
{
    auto* item = new QStandardItem( text );
    if( bold )
    {
        QFont font = item->font();
        font.setBold( true );
        item->setFont( font );
    }
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    root.setChild( row, col, item );
    return item;
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
void FireResultListView::Display( const AgentFireResult& result )
{
    int row = model_.rowCount();
    auto root = AddRoot( row, 0, extractor_.GetDisplayName( result.time_ ) );
    root->setData( result.time_, gui::Roles::SortRole );
    AddRoot( row, 1, GetFirerName( result.firer_ ), result.firer_ );
    AddRoot( row, 2, GetDisplayName( result.target_ ), &result.target_ );

    if( result.IsMiss() )
    {
        AddChild( *root, 0, 0, tr( "No damage" ), true );
        return;
    }

    auto equipment = AddChild( *root, 0, 0, tr( "Equipments" ), true );
    AddChild( *root, 0, 1, tr( "avail" ), true );
    AddChild( *root, 0, 2, tr( "unavail" ), true );
    AddChild( *root, 0, 3, tr( "repairable" ), true );

    auto troops = AddChild( *root, 1, 0, tr( "Troops" ), true );
    AddChild( *root, 1, 1, tr( "officer" ), true );
    AddChild( *root, 1, 2, tr( "warrant-off." ), true );
    AddChild( *root, 1, 3, tr( "private" ), true );

    auto it =  result.CreateIterator();
    while( it.HasMoreElements() )
        Display( it.NextElement(), *equipment );
    for( int row = 0; row < eNbrHumanWound; ++row )
        Display( result.casualties_[ row ], *troops );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
void FireResultListView::Display( const PopulationFireResult& result )
{
    int row = model_.rowCount();
    auto root = AddRoot( row, 0, extractor_.GetDisplayName( result.time_ ) );
    root->setData( result.time_, gui::Roles::SortRole );
    AddRoot( row, 1, GetFirerName( result.firer_ ), result.firer_ );
    AddRoot( row, 2, extractor_.GetDisplayName( result.target_ ), &result.target_ );
    int subRow = 0;
    if( result.deadPeople_ != 0 )
    {
        AddChild( *root, subRow, 1, tr( "Dead:" ) );
        AddChild( *root, subRow, 2, extractor_.GetDisplayName( result.deadPeople_ ) );
        ++subRow;
    }
    if( result.woundedPeople_ != 0 )
    {
        AddChild( *root, subRow, 1, tr( "Wounded:" ) );
        AddChild( *root, subRow, 2, extractor_.GetDisplayName( result.woundedPeople_ ) );
        ++subRow;
    }
    if( result.scatteredPeople_ != 0 )
    {
        AddChild( *root, subRow, 1, tr( "Scattered:" ) );
        AddChild( *root, subRow, 2, extractor_.GetDisplayName( result.scatteredPeople_ ) );
        ++subRow;
    }
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
void FireResultListView::Display( const Equipment& equipment, QStandardItem& root )
{
    int row = root.rowCount();
    AddChild( root, row, 0, extractor_.GetDisplayName( equipment.type_ ) );
    AddChild( root, row, 1, extractor_.GetDisplayName( equipment.available_ ) );
    AddChild( root, row, 2, extractor_.GetDisplayName( equipment.unavailable_ ) );
    AddChild( root, row, 3, extractor_.GetDisplayName( equipment.repairable_ + equipment.onSiteFixable_ ) );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::Display
// Created: JSR 2012-10-24
// -----------------------------------------------------------------------------
void FireResultListView::Display( const Casualties& casualties, QStandardItem& root )
{
    int row = root.rowCount();
    AddChild( root, row, 0, extractor_.GetDisplayName( casualties.wound_ ) );
    AddChild( root, row, 1, extractor_.GetDisplayName( casualties.officers_ ) );
    AddChild( root, row, 2, extractor_.GetDisplayName( casualties.subOfficers_ ) );
    AddChild( root, row, 3, extractor_.GetDisplayName( casualties.troopers_ ) );
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::UpdateDisplay
// Created: ABR 2014-08-25
// -----------------------------------------------------------------------------
void FireResultListView::UpdateDisplay()
{
    model()->removeRows( 0, model()->rowCount() );
    std::for_each( explosions_.begin(), explosions_.end(), [&]( const Explosions* explosion ){
        if( !explosion )
            return;
        const auto& agentFires = explosion->GetAgentExplosions();
        const auto& popFires = explosion->GetPopulationExplosions();
        for( auto it = agentFires.rbegin(); it != agentFires.rend(); ++it )
            Display( *it );
        for( auto it = popFires.rbegin(); it != popFires.rend(); ++it )
            Display( *it );
    } );
}

namespace
{
    void ExtractExtensions( const kernel::Entity_ABC& entity, std::vector< const Explosions* >& explosions )
    {
        if( const auto* explosion = entity.Retrieve< Explosions >() )
            explosions.push_back( explosion );
        if( auto* hierarchy = entity.Retrieve< kernel::TacticalHierarchies >() )
        {
            auto childIterator = hierarchy->CreateSubordinateIterator();
            while( childIterator.HasMoreElements() )
                ExtractExtensions( childIterator.NextElement(), explosions );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::DisplaySelection
// Created: ABR 2014-08-25
// -----------------------------------------------------------------------------
void FireResultListView::DisplaySelection()
{
    explosions_.clear();
    if( selected_ )
        ExtractExtensions( *selected_, explosions_ );
    UpdateDisplay();
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::NotifySelected
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void FireResultListView::NotifySelected( const kernel::Entity_ABC* element )
{
    if( element == selected_ )
        return;
    selected_ = element;
    DisplaySelection();
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::NotifyUpdated
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void FireResultListView::NotifyUpdated( const Explosions& results )
{
    if( !selected_ || std::find( explosions_.begin(), explosions_.end(), &results ) == explosions_.end() )
        return;
    UpdateDisplay();
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::NotifyDeleted
// Created: ABR 2014-08-25
// -----------------------------------------------------------------------------
void FireResultListView::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    auto* hierarchy = entity.Retrieve< kernel::TacticalHierarchies >();
    if( selected_ == &entity ||
        selected_ && hierarchy && hierarchy->IsSubordinateOf( *selected_ ) )
        DisplaySelection();
}

// -----------------------------------------------------------------------------
// Name: FireResultListView::NotifyUpdated
// Created: ABR 2014-09-11
// -----------------------------------------------------------------------------
void FireResultListView::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    for( int i = 0; i < model_.rowCount(); ++i )
    {
        if( auto* firerItem = model_.item( i, 1 ) )
            if( const auto* firer = firerItem->data( Qt::UserRole ).value< const kernel::Entity_ABC* >() )
                if( firer == &entity )
                    firerItem->setData( GetFirerName( &entity ), Qt::DisplayRole );
        if( auto* targetItem = model_.item( i, 2 ) )
            if( const auto* target = targetItem->data( Qt::UserRole ).value< const kernel::Entity_ABC* >() )
                if( target == &entity )
                    targetItem->setData( extractor_.GetDisplayName( entity ), Qt::DisplayRole );
    }
}
