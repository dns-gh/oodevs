//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ParamObstacleList.h"
#include "moc_ParamObstacleList.cpp"

#include "ParamListView.h"
#include "ParamObstacle.h"
#include "ValuedListItem.h"
#include "astec_gaming/ActionController.h"

// -----------------------------------------------------------------------------
// Name: ParamObstacleList constructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamObstacleList::ParamObstacleList( QWidget* parent, ASN1T_ListMissionGenObject& asnObjectList, const std::string& label, const ObjectTypes& objectTypes, ParametersLayer& layer, const CoordinateConverter_ABC& converter, ActionController& controller )
    : QVBox( parent )
    , controller_( controller )
    , objectTypes_( objectTypes )
    , layer_( layer )
    , converter_( converter )
    , asn_( asnObjectList )
    , objects_( 0 )
    , selected_( 0 )
    , popup_( new QPopupMenu( this ) )
{
    listView_ = new ParamListView( this, label.c_str() );
    connect( listView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    disconnect( listView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), listView_, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
    connect( listView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList destructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamObstacleList::~ParamObstacleList()
{
    delete objects_;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::CheckValidity
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
bool ParamObstacleList::CheckValidity()
{
    if( listView_->childCount() == 0 )
        return listView_->Invalid();

    if( selected_  )
    {
        ParamObstacle* obstacle = selected_->GetValue< ParamObstacle* >();
        obstacle->Commit();
        return obstacle->CheckValidity();
    }
    return true;
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::Commit
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacleList::Commit()
{
    if( ! ChangeSelection() )
        return;
    
    const unsigned int childs = listView_->childCount();
    asn_.n = childs;

    if( !childs && IsOptional() )
        return;

    objects_ = new ASN1T_MissionGenObject[ childs ];
    asn_.elem = objects_;

    QListViewItemIterator it( listView_ );
    for( unsigned int i = 0; it.current(); ++it, ++i )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( it.current() );
        item->GetValue< ParamObstacle* >()->CommitTo( asn_.elem[i] );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::ChangeSelection
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
bool ParamObstacleList::ChangeSelection()
{
    if( !selected_ )
        return true;

    ParamObstacle* obstacle = selected_->GetValue< ParamObstacle* >();
    obstacle->Commit();
    if( !obstacle->CheckValidity() )
    {
        listView_->setSelected( selected_, true );
        return false;
    }
    obstacle->RemoveFromController();
    obstacle->hide();
    selected_ = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::OnSelectionChanged
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacleList::OnSelectionChanged( QListViewItem* item )
{
    if( item == selected_ || ! ChangeSelection() || !item )
        return;
    ValuedListItem* current = static_cast< ValuedListItem* >( item );
    ParamObstacle* obstacle = current->GetValue< ParamObstacle* >();
    obstacle->show();
    obstacle->RegisterIn( controller_ );
    selected_ = current;
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::OnRequestPopup
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacleList::OnRequestPopup( QListViewItem* item, const QPoint& pos, int )
{
    popup_->clear();
    popup_->insertItem( tr( "Nouvel obstacle" ), this, SLOT( NewObstacle() ) );
    if( item )
        popup_->insertItem( tr( "Effacer" ), this, SLOT( DeleteSelected() ) );
    popup_->insertItem( tr( "Effacer la liste" ), this, SLOT( ClearList() ) );
    popup_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::NewObstacle
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacleList::NewObstacle()
{
    if( ! ChangeSelection() )
        return;

    ASN1T_MissionGenObject* object = new ASN1T_MissionGenObject();
    ValuedListItem* item = new ValuedListItem( listView_ );
    item->setText( 0, tr( "Obstacle" ) );
    ParamObstacle* param = new ParamObstacle( this, *object, tr( "Obstacle" ).ascii(), objectTypes_, layer_, converter_ );
    item->SetValue( param );
    param->show();
    listView_->setSelected( item, true );
    selected_ = item;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::DeleteSelected
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacleList::DeleteSelected()
{
    DeleteItem( *listView_->selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::DeleteItem
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacleList::DeleteItem( QListViewItem& item )
{
    if( &item == selected_ )
    {
        selected_->GetValue< ParamObstacle* >()->RemoveFromController();
        selected_ = 0;
    }
    item.setSelected( false );
    ValuedListItem* valuedItem = static_cast< ValuedListItem* >( &item );
    ParamObstacle* obstacle = valuedItem->GetValue< ParamObstacle* >();
    delete obstacle;
    delete &item;
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::ClearList
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacleList::ClearList()
{
    while( listView_->childCount() )
        DeleteItem( *listView_->firstChild() );
}
