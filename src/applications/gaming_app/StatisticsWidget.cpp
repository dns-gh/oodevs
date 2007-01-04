// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "StatisticsWidget.h"

// -----------------------------------------------------------------------------
// Name: StatisticsWidget constructor
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
StatisticsWidget::StatisticsWidget( QWidget* parent )
    : QListView( parent )
{
    addColumn( "value" );
    setSorting( -1 );
}

// -----------------------------------------------------------------------------
// Name: StatisticsWidget destructor
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
StatisticsWidget::~StatisticsWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StatisticsWidget::AddValue
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
void StatisticsWidget::AddValue( unsigned long value )
{
    QListViewItem* item = new QListViewItem( this );
    item->setText( 0, QString::number( value ) );
    if( childCount() > 5 )
        removeItem( lastItem() );
}
