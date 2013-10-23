// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "KeyModel.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: KeyModel constructor
// Created: LGY 2012-10-01
// -----------------------------------------------------------------------------
KeyModel::KeyModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KeyModel destructor
// Created: LGY 2012-10-01
// -----------------------------------------------------------------------------
KeyModel::~KeyModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KeyModel::FindItem
// Created: LGY 2012-10-01
// -----------------------------------------------------------------------------
QStandardItem* KeyModel::FindItem( QStandardItem* item, const QString& text ) const
{
    for( int row = 0; row < item->rowCount(); ++row )
    {
        QStandardItem* childItem = item->child( row, 0 );
        if( childItem && childItem->text() == text )
            return childItem;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: KeyModel::FindItem
// Created: LGY 2012-10-01
// -----------------------------------------------------------------------------
QStandardItem* KeyModel::FindItem( const QString& category, QStandardItem* root ) const
{
    QStandardItem* parent = root;
    QStringList path = QStringList::split( '/', category );
    for( int i = 0; i < path.size(); ++i )
    {
        const QString name = path.at( i );
        QStandardItem* item = FindItem( parent, name );
        if( !item )
            return 0;
        if( i == path.size() - 1 )
            return item;
        parent = item;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: KeyModel::FindItem
// Created: LGY 2012-10-01
// -----------------------------------------------------------------------------
QStandardItem* KeyModel::FindItem( const QString& category ) const
{
    return FindItem( category, invisibleRootItem() );
}

// -----------------------------------------------------------------------------
// Name: KeyModel::FindParent
// Created: LGY 2012-10-01
// -----------------------------------------------------------------------------
QStandardItem* KeyModel::FindParent( QStandardItem* item ) const
{
    return item->parent() ? item->parent() : invisibleRootItem();
}

// -----------------------------------------------------------------------------
// Name: KeyModel::CreateItem
// Created: LGY 2012-10-01
// -----------------------------------------------------------------------------
QStandardItem* KeyModel::CreateItem( QStandardItem& parent, const QString& name, const QString& path )
{
    QStandardItem* category = new QStandardItem( name );
    category->setEditable( false );
    category->setData( QVariant( path ), Qt::UserRole );
    parent.appendRow( category );
    return category;
}

// -----------------------------------------------------------------------------
// Name: KeyModel::RemoveIfEmpty
// Created: MMC 2013-03-06
// -----------------------------------------------------------------------------
void KeyModel::RemoveIfEmpty( QStandardItem& item )
{
    if( &item == invisibleRootItem() )
        return;
    if( item.hasChildren() )
        return;
    QStandardItem* pParent = FindParent( &item );
    if( !pParent )
        return;
    QStandardItem* pValueItem = pParent->child( item.row(), 1 );
    if( !pValueItem || ( pValueItem && pValueItem->text().isEmpty() ) )
    {
        pParent->removeRow( item.row() );
        RemoveIfEmpty( *pParent );
    }
}

namespace
{
    void RemoveChildren( QStandardItem& item )
    {
        while( item.rowCount() > 0 )
        {
            RemoveChildren( *item.child( 0 ) );
            item.removeRow( 0 );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: KeyModel::Delete
// Created: LGY 2012-10-01
// -----------------------------------------------------------------------------
void KeyModel::Delete( const QString& category )
{
    QStandardItem* item = FindItem( category );
    if( !item )
        return;
    QStandardItem* pParent = FindParent( item );
    if( !pParent )
        return;
    int curRow = item->row();
    QStandardItem* pRowItem = pParent->child( curRow );
    if( !pRowItem )
        return;
    RemoveChildren( *pRowItem );
    pParent->removeRow( curRow );
    RemoveIfEmpty( *pParent );
}
