// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_UrbanModifiersTable.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_Connector_Table_ABC.h"

using namespace helpers;

//-----------------------------------------------------------------------------
// Name: ADN_UrbanModifiersTable constructor
// Created: SLG 2010-04-13
//-----------------------------------------------------------------------------
ADN_UrbanModifiersTable::ADN_UrbanModifiersTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /*= 0*/ )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 2 );
    setShowGrid( false );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Material" )
        << tr( "Modifiers" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );

    delegate_.AddDoubleSpinBoxOnColumn( 1, 0, 1, 0.01 );
}

//-----------------------------------------------------------------------------
// Name: ADN_UrbanModifiersTable destructor
// Created: SLG 2010-04-13
//-----------------------------------------------------------------------------
ADN_UrbanModifiersTable::~ADN_UrbanModifiersTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_UrbanModifiersTable::AddRow
// Created: JSR 2012-10-31
// -----------------------------------------------------------------------------
void ADN_UrbanModifiersTable::AddRow( int row, void* data )
{
    helpers::ADN_UrbanAttritionInfos* pAttritions = static_cast< helpers::ADN_UrbanAttritionInfos* >( data );
    if( !pAttritions )
        return;

    AddItem( row, 0, data, &pAttritions->ptrMaterial_.GetData()->strName_, ADN_StandardItem::eString );
    AddItem( row, 1, data, &pAttritions->rCoeff_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}
