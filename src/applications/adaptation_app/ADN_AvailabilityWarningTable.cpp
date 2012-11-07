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

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable constructor
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
ADN_AvailabilityWarningTable::ADN_AvailabilityWarningTable( const QString& objectName, ADN_Ref_ABC& vector, QWidget* pParent /*= 0*/ )
    : ADN_Table( objectName, vector, pParent )
{
    setFixedHeight( 300 );
    dataModel_.setColumnCount( 1 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Percentage" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    dataModel_.setSortRole( Qt::Ascending );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddSpinBoxOnColumn( 0, 0, 100 );
    delegate_.AddColorOnColumn( 0, 0, 100. );
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
    Q3PopupMenu menu( this );
    menu.insertItem( tr( "New warning"), 0 );
    menu.insertItem( tr( "Delete warning"), 1 );
    menu.setItemEnabled( 1, GetSelectedData() != 0 );

    const int nMenu = menu.exec( pt );
    if( nMenu == 0 )
        CreateNewElement();
    else if( nMenu == 1 )
        DeleteCurrentElement();
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable::CreateNewElement
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarningTable::CreateNewElement()
{
    ADN_AvailabilityWarning* pNew = new ADN_AvailabilityWarning();
    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNew );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarningTable::DeleteCurrentElement
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarningTable::DeleteCurrentElement()
{
    ADN_AvailabilityWarning* pCurrent = (ADN_AvailabilityWarning*)GetSelectedData();
    if( pCurrent != 0 )
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrent );
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
