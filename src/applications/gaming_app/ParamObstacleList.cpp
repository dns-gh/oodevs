//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamObstacleList.h"
#include "moc_ParamObstacleList.cpp"

#include "ParamListView.h"
#include "ParamObstacle.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_kernel/ActionController.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamObstacleList constructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamObstacleList::ParamObstacleList( QObject* parent, const QString& name, const ObjectTypes& objectTypes, ParametersLayer& layer, const CoordinateConverter_ABC& converter, ActionController& controller )
    : QObject( parent )
    , Param_ABC( name )
    , converter_( converter )
    , controller_( controller )
    , objectTypes_( objectTypes )
    , layer_( layer )
    , box_( 0 )
    , list_( new ParamListView( this, GetName() ) )
    , selected_( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList destructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamObstacleList::~ParamObstacleList()
{
    ClearList();
    delete list_;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamObstacleList::BuildInterface( QWidget* parent )
{
    box_ = new QVBox( parent );
    popup_ = new QPopupMenu( box_ );
    list_->BuildInterface( box_ );
    connect( list_->ListView(), SIGNAL( selectionChanged( QListViewItem* ) ), SLOT( OnSelectionChanged( QListViewItem* ) ) );
    disconnect( list_->ListView(), SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), list_, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
    connect( list_->ListView(), SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::CheckValidity
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
bool ParamObstacleList::CheckValidity()
{
    if( list_->ListView()->childCount() == 0 )
        return list_->Invalid();

    if( selected_  )
    {
        ParamObstacle* obstacle = selected_->GetValue< ParamObstacle >();
//        obstacle->Commit();
        return obstacle->CheckValidity();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamObstacleList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    ASN1T_ListMissionGenObject*& list = asn.value.u.listMissionGenObject;
    list = 0;
    if( ! box_ )
        InterfaceNotInitialized();
//    if( ! ChangeSelection() )
//        return;
    asn.value.t = T_MissionParameter_value_listMissionGenObject;
    list = new ASN1T_ListMissionGenObject();
    list->n = list_->ListView()->childCount();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_MissionGenObject[ list->n ];
    QListViewItemIterator it( list_->ListView() );
    for( unsigned int i = 0; it.current(); ++it, ++i )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( it.current() );
        item->GetValue< ParamObstacle >()->CommitTo( list->elem[i] );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamObstacleList::Clean( ASN1T_MissionParameter& asn ) const
{
    delete[] asn.value.u.listMissionGenObject->elem;
    delete asn.value.u.listMissionGenObject;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::ChangeSelection
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
bool ParamObstacleList::ChangeSelection()
{
    if( !selected_ )
        return true;

    ParamObstacle* obstacle = selected_->GetValue< ParamObstacle >();
//    obstacle->Commit();
    if( !obstacle->CheckValidity() )
    {
        list_->ListView()->setSelected( selected_, true );
        return false;
    }
    obstacle->RemoveFromController();
    obstacle->Hide();
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
    ParamObstacle* obstacle = current->GetValue< ParamObstacle >();
    obstacle->Show();
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
    popup_->insertItem( tr( "New obstacle" ), this, SLOT( NewObstacle() ) );
    if( item )
        popup_->insertItem( tr( "Remove" ), this, SLOT( DeleteSelected() ) );
    popup_->insertItem( tr( "Clear list" ), this, SLOT( ClearList() ) );
    popup_->exec( pos );
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::NewObstacle
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacleList::NewObstacle()
{
    if( ! ChangeSelection() )
        return;

    ValuedListItem* item = new ValuedListItem( list_->ListView() );
    item->setText( 0, tr( "Obstacle" ) );
    ParamObstacle* param = new ParamObstacle( box_, tr( "Obstacle" ), objectTypes_, layer_, converter_ );
    param->BuildInterface( box_ ); // $$$$ SBO 2007-03-14: 
    item->SetValue( param );
    param->Show();
    list_->ListView()->setSelected( item, true );
    selected_ = item;
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::DeleteSelected
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacleList::DeleteSelected()
{
    DeleteItem( *list_->ListView()->selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::DeleteItem
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacleList::DeleteItem( QListViewItem& item )
{
    if( &item == selected_ )
    {
        ParamObstacle* obstacle = selected_->GetValue< ParamObstacle >();
        obstacle->RemoveFromController();
        obstacle->Hide();
        selected_ = 0;
    }
    item.setSelected( false );
    ValuedListItem* valuedItem = static_cast< ValuedListItem* >( &item );
    ParamObstacle* obstacle = valuedItem->GetValue< ParamObstacle >();
    delete obstacle;
    delete &item;
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::ClearList
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamObstacleList::ClearList()
{
    while( list_->ListView()->childCount() )
        DeleteItem( *list_->ListView()->firstChild() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::Draw
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void ParamObstacleList::Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !list_ )
        return;
    QListViewItemIterator it( list_->ListView() );
    for( unsigned int i = 0; it.current(); ++it, ++i )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( it.current() );
        item->GetValue< ParamObstacle >()->Draw( point, viewport, tools );
    }
}
