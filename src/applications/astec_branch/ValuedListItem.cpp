// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ValuedListItem.h"

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( QListView * parent )
: QListViewItem( parent )
, container_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( QListView * parent, QListViewItem * after )
: QListViewItem( parent, after )
, container_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( QListView * parent, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
: QListViewItem( parent, label1, label2, label3, label4, label5, label6, label7, label8 )
, container_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( QListView * parent, QListViewItem * after, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
: QListViewItem( parent, after, label1, label2, label3, label4, label5, label6, label7, label8 )
, container_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( QListViewItem * parent )
: QListViewItem( parent )
, container_( 0 )
{

}
    
// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( QListViewItem * parent, QListViewItem * after )
: QListViewItem( parent, after )
, container_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( QListViewItem * parent, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
: QListViewItem( parent, label1, label2, label3, label4, label5, label6, label7, label8 )
, container_( 0 )
{

}
    
// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
ValuedListItem::ValuedListItem( QListViewItem * parent, QListViewItem * after, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
: QListViewItem( parent, after, label1, label2, label3, label4, label5, label6, label7, label8 )
, container_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: ValuedListItem destructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
ValuedListItem::~ValuedListItem()
{
    delete container_;
}
