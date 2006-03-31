// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "EntityListParameter.h"
#include "moc_EntityListParameter.cpp"

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
EntityListParameterBase::EntityListParameterBase( QWidget* pParent, unsigned int& n, ASN1T_OID*& ids, const std::string& label, const std::string& menu )
    : QListView( pParent )
    , n_( n )
    , ids_( ids )
    , menu_( menu )
    , pPopupMenu_( new QPopupMenu( this ) )
{
    ids_ = 0;
    addColumn( label.c_str() );
    setResizeMode( QListView::LastColumn );
    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
EntityListParameterBase::~EntityListParameterBase()
{
    delete[] ids_;
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::OnRequestPopup
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityListParameterBase::OnRequestPopup( QListViewItem* pItem, const QPoint& pos )
{
    pPopupMenu_->clear();
    if( pItem != 0 )
        pPopupMenu_->insertItem( tr( "Effacer" ), this, SLOT( OnDeleteSelectedItem() ) );
    pPopupMenu_->insertItem( tr( "Effacer la liste" ), this, SLOT( OnClearList() ) );
    pPopupMenu_->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::OnDeleteSelectedItem
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityListParameterBase::OnDeleteSelectedItem()
{
     delete currentItem();
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::OnClearList
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityListParameterBase::OnClearList()
{
    clear();
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::TurnHeaderBlack
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityListParameterBase::TurnHeaderBlack()
{
    header()->setPaletteForegroundColor( Qt::black );
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::Invalid
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
bool EntityListParameterBase::Invalid()
{
    header()->setPaletteForegroundColor( Qt::red );
    QTimer::singleShot( 3000, this, SLOT( TurnHeaderBlack() ) );
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::AddToMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityListParameterBase::AddToMenu( QPopupMenu& menu )
{
    menu.insertItem( menu_.c_str(), this, SLOT( MenuItemValidated() ) );
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::CheckValidity
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
bool EntityListParameterBase::CheckValidity()
{
    if( !IsOptional() && childCount() == 0 ) // $$$$ AGE 2006-03-14: min number ?
        return Invalid();
    return true;
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::Commit
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void EntityListParameterBase::Commit()
{
    n_ = childCount();
    if( ! childCount() )
        return;
    
    delete ids_;
    ids_ = new ASN1T_OID[ n_ ];

    unsigned int i = 0;
    ValuedListItem* item = (ValuedListItem*)( firstChild() );
    while( item )
    {
        ids_[ i ] = GetId( item );
        item = (ValuedListItem*)( item->nextSibling() );
        ++i;
    }
}
