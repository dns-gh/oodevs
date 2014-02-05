// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_SurfaceFireInfos_Table.h"
#include "ADN_Fires_Data.h"

//-----------------------------------------------------------------------------
// Name: ADN_SurfaceFireInfos_Table constructor
// Created: BCI 2010-12-03
//-----------------------------------------------------------------------------
ADN_SurfaceFireInfos_Table::ADN_SurfaceFireInfos_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    setShowGrid( false );
    dataModel_.setColumnCount( 3 );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_SurfaceFireInfos_Table", "Ground type" )
                      << tools::translate( "ADN_SurfaceFireInfos_Table", "Ignition threshold" )
                      << tools::translate( "ADN_SurfaceFireInfos_Table", "Max combustion energy" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    delegate_.AddSpinBoxOnColumn( 1, std::numeric_limits< int >::min(), std::numeric_limits< int >::max() );
    delegate_.AddSpinBoxOnColumn( 2, std::numeric_limits< int >::min(), std::numeric_limits< int >::max() );
}

//-----------------------------------------------------------------------------
// Name: ADN_SurfaceFireInfos_Table destructor
// Created: JSR 2010-12-03
//-----------------------------------------------------------------------------
ADN_SurfaceFireInfos_Table::~ADN_SurfaceFireInfos_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_SurfaceFireInfos_Table::AddRow
// Created: JSR 2012-11-06
// -----------------------------------------------------------------------------
void ADN_SurfaceFireInfos_Table::AddRow( int row, void* data )
{
    ADN_Fires_Data::FireSurfaceInfos* pInfos = static_cast< ADN_Fires_Data::FireSurfaceInfos* >( data );
    if( !pInfos )
        return;
    AddItem( row, 0, data, &pInfos->strName_, ADN_StandardItem::eString );
    AddItem( row, 1, data, &pInfos->ignitionThreshold_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 2, data, &pInfos->maxCombustionEnergy_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}
