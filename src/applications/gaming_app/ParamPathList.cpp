// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPathList.h"
#include "moc_ParamPathList.cpp"
#include "clients_gui/ParametersLayer.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "ParamPath.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_kernel/ActionController.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamPathList constructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamPathList::ParamPathList( QObject* parent, const QString& name, ParametersLayer& layer, const CoordinateConverter_ABC& converter, const Entity_ABC& agent, ActionController& controller )
    : QObject( parent )
    , Param_ABC( name )
    , layer_( layer )
    , converter_( converter )
    , controller_( controller )
    , agent_( agent )
    , box_( 0 )
    , list_( new ParamListView( this, GetName() ) )
    , selected_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPathList destructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamPathList::~ParamPathList()
{
    ClearList();
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamPathList::BuildInterface( QWidget* parent )
{
    box_ = new QVBox( parent );
    list_->BuildInterface( box_ );
    popup_ = new QPopupMenu( list_->ListView() );
    connect( list_->ListView(), SIGNAL( selectionChanged( QListViewItem* ) ), SLOT( OnSelectionChanged( QListViewItem* ) ) );
    disconnect( list_->ListView(), SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), list_, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
    connect( list_->ListView(), SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::CheckValidity
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
bool ParamPathList::CheckValidity()
{
    if( list_->ListView()->childCount() == 0 )
        return list_->Invalid();

    if( selected_  )
    {
        ParamPath* path = selected_->GetValue< ParamPath >();
//        path->CommitTo();
        return path->CheckValidity();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPathList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    if( ! box_ )
        InterfaceNotInitialized();
//    if( ! ChangeSelection() )
//        return;
    asn.value.t = T_MissionParameter_value_listItineraire;
    asn.value.u.listItineraire = new ASN1T_ListItineraire();
    asn.value.u.listItineraire->n = list_->ListView()->childCount();
    asn.null_value = asn.value.u.listItineraire->n ? 0 : 1;
    if( asn.null_value )
        return;

    ASN1T_ListItineraire*& list = asn.value.u.listItineraire;
    list->elem = new ASN1T_Itineraire[ list->n ];
    QListViewItemIterator it( list_->ListView() );
    for( unsigned int i = 0; it.current(); ++it, ++i )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( it.current() );
        item->GetValue< ParamPath >()->CommitTo( list->elem[i] );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPathList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listItineraire )
    {
        for( unsigned int i = 0; i < asn.value.u.listItineraire->n; ++i )
            delete[] asn.value.u.listItineraire->elem[i].vecteur_point.elem;
        delete[] asn.value.u.listItineraire->elem;
    }
    delete asn.value.u.listItineraire;
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::ChangeSelection
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
bool ParamPathList::ChangeSelection()
{
    if( !selected_ )
        return true;

    ParamPath* path = selected_->GetValue< ParamPath >();
//    path->Commit();
    if( !path->CheckValidity() )
    {
        list_->ListView()->setSelected( selected_, true );
        return false;
    }
    path->RemoveFromController();
    path->Hide();
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
    ParamPath* path = current->GetValue< ParamPath >();
    path->Show();
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
    popup_->insertItem( tr( "New route" ), this, SLOT( NewPath() ) );
    if( item )
        popup_->insertItem( tr( "Remove" ), this, SLOT( DeleteSelected() ) );
    popup_->insertItem( tr( "Clear list" ), this, SLOT( ClearList() ) );
    popup_->exec( pos );
}


// -----------------------------------------------------------------------------
// Name: ParamPathList::NewPath
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPathList::NewPath()
{
    if( ! ChangeSelection() )
        return;

    ValuedListItem* item = new ValuedListItem( list_->ListView() );
    item->setText( 0, tr( "Route" ) );
    ParamPath* param = new ParamPath( box_, tr( "Route" ), layer_, converter_, agent_ );
    param->BuildInterface( box_ );
    item->SetValue( param );
    param->Show();
    list_->ListView()->setSelected( item, true );
    selected_ = item;
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::DeleteSelected
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPathList::DeleteSelected()
{
    DeleteItem( *list_->ListView()->selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::DeleteItem
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPathList::DeleteItem( QListViewItem& item )
{
    if( &item == selected_ )
    {
        ParamPath* path = selected_->GetValue< ParamPath >();
        path->RemoveFromController();
        path->Hide();
        selected_ = 0;
    }
    item.setSelected( false );
    ValuedListItem* valuedItem = static_cast< ValuedListItem* >( &item );
    ParamPath* path = valuedItem->GetValue< ParamPath >();
    delete path;
    delete &item;
}


// -----------------------------------------------------------------------------
// Name: ParamPathList::ClearList
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
void ParamPathList::ClearList()
{
    while( list_->ListView()->childCount() )
        DeleteItem( *list_->ListView()->firstChild() );
}
