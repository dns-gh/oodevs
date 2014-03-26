// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_AvailabilityWarningTable.h"
#include "ADN_AvailabilityWarning.h"
#include "ADN_Gui_Tools.h"

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable constructor
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
ADN_AvailabilityWarningTable::ADN_AvailabilityWarningTable( const QString& objectName, ADN_Ref_ABC& vector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, vector, pParent )
{
    setFixedHeight( 300 );
    dataModel_.setColumnCount( 1 );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_AvailabilityWarningTable", "Percentage" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddSpinBoxOnColumn( 0, 0, 100 );
    delegate_.AddColorOnColumn( 0, 0, 100. );
    static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).AddItem( 0 );
    Sort();
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable destructor
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
ADN_AvailabilityWarningTable::~ADN_AvailabilityWarningTable()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable::OnContextMenu
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarningTable::OnContextMenu( const QPoint& pt )
{
    ADN_Gui_Tools::GenerateStandardContextMenu< ADN_AvailabilityWarning >( *this, pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable::AddRow
// Created: NPT 2012-11-05
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarningTable::AddRow( int row, void* data )
{
    ADN_AvailabilityWarning* warning = static_cast<ADN_AvailabilityWarning*>( data );
    if( !warning )
        return;
    AddItem( row, 0, data, &warning->percentage_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable::dataChanged
// Created: LDC 2014-03-26
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarningTable::dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
    ADN_Table::dataChanged( topLeft, bottomRight );
    Sort();
}
