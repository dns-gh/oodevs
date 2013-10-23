// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Rich_ListViewItem.h"
#include "ADN_ListView.h"
#include "ADN_StandardItem.h"

QColor ADN_Rich_ListViewItem::warningColor_( 255, 185, 125 );
QColor ADN_Rich_ListViewItem::errorColor_  ( 255, 125, 125 );

namespace
{
    void Initialize( QStandardItem* item )
    {
        item->setFlags( Qt::ItemIsEnabled );
        item->setData( QVariant( gui::StandardModel::showValue_ ), gui::Roles::FilterRole );
        item->setData( QVariant(), gui::Roles::DataRole );
        item->setData( QVariant(), gui::Roles::SafeRole );
        item->setData( QVariant(), gui::Roles::MimeTypeRole );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem constructor
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
ADN_Rich_ListViewItem::ADN_Rich_ListViewItem( ADN_ListView* parent, Qt::Alignment alignment )
    : eSortingConstraint_ ( eSortingConstraint_Default )
{
    Initialize( this );
    QList< QStandardItem* > list;
    list << this;
    const int col = parent->header()->count();
    for( int i = 1; i < col; ++i )
    {
        QStandardItem* item = new QStandardItem();
        item->setTextAlignment( alignment );
        Initialize( item );
        list << item;
    }
    parent->InsertItems( list );
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem constructor
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
ADN_Rich_ListViewItem::ADN_Rich_ListViewItem( QStandardItem* parent, Qt::Alignment alignment )
    : eSortingConstraint_ ( eSortingConstraint_Default )
{
    Initialize( this );
    const int row = parent->rowCount();
    parent->setChild( row, 0, this );
    const int col = parent->model()->columnCount();
    for( int i = 1; i < col; ++i )
    {
        QStandardItem* item = new QStandardItem();
        item->setTextAlignment( alignment );
        Initialize( item );
        parent->setChild( row, i, item );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem destructor
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
ADN_Rich_ListViewItem::~ADN_Rich_ListViewItem()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::setText
// Created: JSR 2012-10-02
// -----------------------------------------------------------------------------
void ADN_Rich_ListViewItem::setText( int column, const QString& string )
{
    if( column == 0 )
        QStandardItem::setText( string );
    else
    {
        QStandardItem* p = parent();
        if( !p )
            p = model()->invisibleRootItem();
        if( p )
            p->child( row(), column )->setText( string );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::SetBackgroundColor
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
void ADN_Rich_ListViewItem::SetBackgroundColor( uint nColumn, const QColor& color )
{
    if( nColumn == 0 )
        setData( color, Qt::BackgroundColorRole );
    else
    {
        QStandardItem* p = parent();
        if( !p )
            p = model()->invisibleRootItem();
        if( p )
            p->child( row(), nColumn )->setData( color, Qt::BackgroundColorRole );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::SetWarning
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
void ADN_Rich_ListViewItem::SetWarning( uint nColumn )
{
    SetBackgroundColor( nColumn, warningColor_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::SetError
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
void ADN_Rich_ListViewItem::SetError( uint nColumn )
{
    SetBackgroundColor( nColumn, errorColor_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::GetSorting
// Created: JSR 2012-10-03
// -----------------------------------------------------------------------------
ADN_Rich_ListViewItem::E_SortingConstraint ADN_Rich_ListViewItem::GetSorting() const
{
    return eSortingConstraint_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::OverrideSorting
// Created: SBO 2006-01-13
// -----------------------------------------------------------------------------
void ADN_Rich_ListViewItem::OverrideSorting( E_SortingConstraint eConstraint )
{
    eSortingConstraint_ = eConstraint;
}
