// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_WeatherFireEffects_Table.h"
#include "ADN_WeatherFireEffects.h"
#include "ADN_Tr.h"

//-----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects_Table constructor
// Created: BCI 2010-12-02
//-----------------------------------------------------------------------------
ADN_WeatherFireEffects_Table::ADN_WeatherFireEffects_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    setShowGrid( false );
    dataModel_.setColumnCount( 2 );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Weather" ) << tr( "Heat decrease rate" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    delegate_.AddSpinBoxOnColumn( 1, std::numeric_limits< int >::min(), std::numeric_limits< int >::max() );
}

//-----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects_Table destructor
// Created: BCI 2010-12-02
//-----------------------------------------------------------------------------
ADN_WeatherFireEffects_Table::~ADN_WeatherFireEffects_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects_Table::AddRow
// Created: JSR 2012-11-06
// -----------------------------------------------------------------------------
void ADN_WeatherFireEffects_Table::AddRow( int row, void* data )
{
    ADN_WeatherFireEffects* pInfos = static_cast< ADN_WeatherFireEffects* >( data );
    if( !pInfos )
        return;
    AddItem( row, 0, data, ADN_Tr::ConvertFromSensorWeatherModifiers( pInfos->weatherType_, ENT_Tr::eToTr ).c_str() );
    AddItem( row, 1, data, &pInfos->heatDecreaseRate_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}
