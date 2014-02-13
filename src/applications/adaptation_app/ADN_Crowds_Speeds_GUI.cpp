// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Crowds_Speeds_GUI.h"
#include "moc_ADN_Crowds_Speeds_GUI.cpp"
#include "ADN_Crowds_Data.h"
#include "ENT/ENT_Tr.h"

//-----------------------------------------------------------------------------
// Name: ADN_Crowds_Speeds_GUI constructor
// Created: JSR 2014-02-13
//-----------------------------------------------------------------------------
ADN_Crowds_Speeds_GUI::ADN_Crowds_Speeds_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Ground type" )
                      << tr( "Speed (km/h)" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0, INT_MAX );
    setSortingEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_Crowds_Speeds_GUI destructor
// Created: JSR 2014-02-13
//-----------------------------------------------------------------------------
ADN_Crowds_Speeds_GUI::~ADN_Crowds_Speeds_GUI()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Speeds_GUI::AddRow
// Created: JSR 2014-02-13
// -----------------------------------------------------------------------------
void ADN_Crowds_Speeds_GUI::AddRow( int row, void* data )
{
    ADN_Crowds_Data::SpeedInfos* infos = static_cast< ADN_Crowds_Data::SpeedInfos* >( data );
    if( !infos )
        return;
    QString typeTerrain = ENT_Tr::ConvertFromLocation( infos->nTypeTerrain_, ENT_Tr::eToTr ).c_str();
    AddItem( row, 0, data, typeTerrain );
    AddItem( row, 1, data, &infos->rSpeed_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}
