// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ParamPathList.h"
#include "moc_ParamPathList.cpp"
#include "ParametersLayer.h"
#include "astec_kernel/CoordinateConverter_ABC.h"
#include "astec_kernel/Entity_ABC.h"
#include "ParamPath.h"
#include "ValuedListItem.h"
#include "astec_kernel/ActionController.h"

// -----------------------------------------------------------------------------
// Name: ParamPathList constructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamPathList::ParamPathList( QWidget* parent, ASN1T_ListItineraire& asnPathList, const std::string& label, ParametersLayer& layer, const CoordinateConverter_ABC& converter, const Entity_ABC& agent, ActionController& controller )
    : QVBox( parent )
    , layer_( layer )
    , converter_( converter )
    , controller_( controller )
    , agent_( agent )
    , asn_( asnPathList )
    , paths_( 0 )
    , listView_( new ParamListView( this, label.c_str() ) )
    , selected_( 0 )
    , popup_( new QPopupMenu( this ) )
{
    connect( listView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    disconnect( listView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), listView_, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
    connect( listView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
    NewPath();
}

// -----------------------------------------------------------------------------
// Name: ParamPathList destructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamPathList::~ParamPathList()
{
    delete[] paths_;
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::CheckValidity
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
bool ParamPathList::CheckValidity()
{
    if( listView_->childCount() == 0 )
        return listView_->Invalid();

    if( selected_  )
    {
        ParamPath* path = selected_->GetValue< ParamPath* >();
        path->Commit();
        return path->CheckValidity();
    }
    return true;
}


// -----------------------------------------------------------------------------
// Name: ParamPathList::Commit
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPathList::Commit()
{
    if( ! ChangeSelection() )
        return;
    
    const unsigned int childs = listView_->childCount();
    asn_.n = childs;

    if( !childs && IsOptional() )
        return;

    paths_ = new ASN1T_Itineraire[ childs ];
    asn_.elem = paths_;

    QListViewItemIterator it( listView_ );
    for( unsigned int i = 0; it.current(); ++it, ++i )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( it.current() );
        item->GetValue< ParamPath* >()->CommitTo( asn_.elem[i] );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::ChangeSelection
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
bool ParamPathList::ChangeSelection()
{
    if( !selected_ )
        return true;

    ParamPath* path = selected_->GetValue< ParamPath* >();
    path->Commit();
    if( !path->CheckValidity() )
    {
        listView_->setSelected( selected_, true );
        return false;
    }
    path->RemoveFromController();
    path->hide();
    selected_ = 0;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::OnSelectionChanged
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPathList::OnSelectionChanged( QListViewItem* item )
{
    if( item == selected_ || ! ChangeSelection() || ! item )
        return;
    ValuedListItem* current = static_cast< ValuedListItem* >( item );
    ParamPath* path = current->GetValue< ParamPath* >();
    path->show();
    path->RegisterIn( controller_ );
    selected_ = current;
}


// -----------------------------------------------------------------------------
// Name: ParamPathList::OnRequestPopup
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPathList::OnRequestPopup( QListViewItem* item, const QPoint& pos, int )
{
    popup_->clear();
    popup_->insertItem( tr( "Nouvel itinéraire" ), this, SLOT( NewPath() ) );
    if( item )
        popup_->insertItem( tr( "Effacer" ), this, SLOT( DeleteSelected() ) );
    popup_->insertItem( tr( "Effacer la liste" ), this, SLOT( ClearList() ) );
    popup_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: ParamPathList::NewPath
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPathList::NewPath()
{
    if( ! ChangeSelection() )
        return;

    ASN1T_Itineraire* asn = new ASN1T_Itineraire();
    ValuedListItem* item = new ValuedListItem( listView_ );
    item->setText( 0, tr( "Itinéraire" ) );
    ParamPath* param = new ParamPath( this, *asn, tr( "Itinéraire" ).ascii(), tr( "Itinéraire" ).ascii(), layer_, converter_, agent_ );
    item->SetValue( param );
    param->show();
    listView_->setSelected( item, true );
    selected_ = item;
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::DeleteSelected
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPathList::DeleteSelected()
{
    DeleteItem( *listView_->selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::DeleteItem
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPathList::DeleteItem( QListViewItem& item )
{
    if( &item == selected_ )
    {
        selected_->GetValue< ParamPath* >()->RemoveFromController();
        selected_ = 0;
    }
    item.setSelected( false );
    ValuedListItem* valuedItem = static_cast< ValuedListItem* >( &item );
    ParamPath* path = valuedItem->GetValue< ParamPath* >();
    delete path;
    delete &item;
}


// -----------------------------------------------------------------------------
// Name: ParamPathList::ClearList
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPathList::ClearList()
{
    while( listView_->childCount() )
        DeleteItem( *listView_->firstChild() );
}
