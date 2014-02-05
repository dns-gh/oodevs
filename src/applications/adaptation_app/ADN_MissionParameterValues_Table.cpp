// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MissionParameterValues_Table.h"
#include "ADN_Missions_Data.h"
#include "ADN_CommonGfx.h"

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterValues_Table constructor
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
ADN_MissionParameterValues_Table::ADN_MissionParameterValues_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    setShowGrid( false );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    dataModel_.setHorizontalHeaderLabels( QStringList( tools::translate( "ADN_MissionParameterValues_Table", "Name" ) ) );
    delegate_.AddLineEditOnColumn( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterValues_Table destructor
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
ADN_MissionParameterValues_Table::~ADN_MissionParameterValues_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterValues_Table::AddNewElement
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
void ADN_MissionParameterValues_Table::AddNewElement()
{
    ADN_Missions_ParameterValue* newElement = new ADN_Missions_ParameterValue();
    newElement->name_ = tools::translate( "ADN_MissionParameterValues_Table", "New value" ).toStdString();

    ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
    pCTable.AddItem( newElement );
    pCTable.AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterValues_Table::OnContextMenu
// Created: SBO 2006-12-05
// -----------------------------------------------------------------------------
void ADN_MissionParameterValues_Table::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popup( this );

    popup.insertItem( tools::translate( "ADN_MissionParameterValues_Table", "Add value"), 0 );
    if( GetData( pt ) )
        popup.insertItem( tools::translate( "ADN_MissionParameterValues_Table", "Remove value"), 1 );

    int result = popup.exec( pt );
    if( result == 1 )
        RemoveCurrentElement();
    else if( result == 0 )
        AddNewElement();
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameterValues_Table::AddRow
// Created: JSR 2012-11-06
// -----------------------------------------------------------------------------
void ADN_MissionParameterValues_Table::AddRow( int row, void* data )
{
    ADN_Missions_ParameterValue* pInfos = static_cast< ADN_Missions_ParameterValue* >( data );
    if( !pInfos )
        return;
    AddItem( row, 0, data, &pInfos->name_, ADN_StandardItem::eString, Qt::ItemIsEditable );
}
