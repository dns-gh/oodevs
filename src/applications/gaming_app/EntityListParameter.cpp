// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EntityListParameter.h"
#include "moc_EntityListParameter.cpp"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
EntityListParameterBase::EntityListParameterBase( QObject* parent, const QString& name )
    : QObject( parent )
    , Param_ABC( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
EntityListParameterBase::~EntityListParameterBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void EntityListParameterBase::BuildInterface( QWidget* parent )
{
    listView_ = new QListView( parent );
    pPopupMenu_ = new QPopupMenu( listView_ );
    listView_->addColumn( GetName() );
    listView_->setResizeMode( QListView::LastColumn );
    connect( listView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::OnRequestPopup
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityListParameterBase::OnRequestPopup( QListViewItem* pItem, const QPoint& pos )
{
    pPopupMenu_->clear();
    if( pItem != 0 )
        pPopupMenu_->insertItem( tr( "Remove" ), this, SLOT( OnDeleteSelectedItem() ) );
    pPopupMenu_->insertItem( tr( "Clear list" ), this, SLOT( OnClearList() ) );
    pPopupMenu_->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::OnDeleteSelectedItem
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityListParameterBase::OnDeleteSelectedItem()
{
     delete listView_->currentItem();
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::OnClearList
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityListParameterBase::OnClearList()
{
    listView_->clear();
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::TurnHeaderBlack
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityListParameterBase::TurnHeaderBlack()
{
    listView_->header()->setPaletteForegroundColor( Qt::black );
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::Invalid
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
bool EntityListParameterBase::Invalid()
{
    listView_->header()->setPaletteForegroundColor( Qt::red );
    QTimer::singleShot( 3000, this, SLOT( TurnHeaderBlack() ) );
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::AddToMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityListParameterBase::AddToMenu( ContextMenu& menu )
{
    menu.InsertItem( "Parameter", GetName(), this, SLOT( MenuItemValidated() ) );
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::CheckValidity
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
bool EntityListParameterBase::CheckValidity()
{
    // $$$$ AGE 2006-03-14: min number ?
//    if( !IsOptional() && childCount() == 0 ) 
//        return Invalid();
    return true;
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void EntityListParameterBase::CommitTo( ASN1T_ListOID*& asn ) const
{
    asn = new ASN1T_ListOID();
    if( !( asn->n = listView_->childCount() ) )
    {
        asn->elem = 0;
        return;
    }
    asn->elem = new ASN1T_OID[ asn->n ];
    unsigned int i = 0;
    ValuedListItem* item = (ValuedListItem*)( listView_->firstChild() );
    while( item )
    {
        asn->elem[ i ] = GetId( item );
        item = (ValuedListItem*)( item->nextSibling() );
        ++i;
    }
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void EntityListParameterBase::Clean( ASN1T_ListOID*& asn ) const
{
    delete[] asn->elem;
    delete asn;
}
