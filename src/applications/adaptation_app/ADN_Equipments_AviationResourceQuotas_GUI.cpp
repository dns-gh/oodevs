// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_AviationResourceQuotas_GUI.h"
#include "ADN_Equipments_Data.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Equipments_Data::AviationResourceQuotasInfos AviationResourceQuotasInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_AviationResourceQuotas_GUI constructor
// Created: JSR 2013-10-16
// -----------------------------------------------------------------------------
ADN_Equipments_AviationResourceQuotas_GUI::ADN_Equipments_AviationResourceQuotas_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /*= 0*/ )
    : ADN_Table( objectName, connector, pParent )
{
    setSelectionMode( QAbstractItemView::NoSelection );
    setShowGrid( false );

    dataModel_.setColumnCount( eNbrAmmunitionType + 1 );
    dataModel_.setRowCount( eNbrAviationRanges );
    QStringList horizontalHeaders;
    for( int i = 0; i < eNbrAmmunitionType + 1; ++i )
    {
        const QString strPercent = tools::translate( "ADN_Equipments_AviationResourceQuotas_GUI", " (%)" );
        if( i == eNbrAmmunitionType )
            horizontalHeaders.append( ENT_Tr::ConvertFromDotationFamily( eDotationFamily_Carburant ).c_str() + strPercent );
        else
            horizontalHeaders.append( ENT_Tr::ConvertFromAmmunitionType( static_cast< E_AmmunitionType >( i ), ENT_Tr_ABC::eToTr ).c_str() + strPercent );
        delegate_.AddColorOnColumn( i, 0., 100. );
        delegate_.AddSpinBoxOnColumn( i );
    }
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    QStringList verticalHeaders;
    for( int i = 0; i < eNbrAviationRanges; ++i )
        verticalHeaders.append( ENT_Tr::ConvertFromAviationRange( static_cast< E_AviationRange >( i ), ENT_Tr_ABC::eToTr ).c_str() );
    dataModel_.setVerticalHeaderLabels( verticalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setDefaultSectionSize( 24 );
    setMinimumHeight( ( eNbrAviationRanges + 1 ) * 24  + 6);
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_AviationResourceQuotas_GUI destructor
// Created: JSR 2013-10-16
// -----------------------------------------------------------------------------
ADN_Equipments_AviationResourceQuotas_GUI::~ADN_Equipments_AviationResourceQuotas_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_AviationResourceQuotas_GUI::AddRow
// Created: JSR 2013-10-16
// -----------------------------------------------------------------------------
void ADN_Equipments_AviationResourceQuotas_GUI::AddRow( int row, void* data )
{
    AviationResourceQuotasInfos* infos = static_cast< AviationResourceQuotasInfos* >( data );
    if( !infos )
        return;
    for( int i = 0; i < eNbrAmmunitionType + 1; ++i )
        AddItem( row, i, data, &infos->resourceQuotas_[ i ], ADN_StandardItem::eInt, Qt::ItemIsEditable );
    QStringList verticalHeaders;
    for( int i = 0; i < eNbrAviationRanges; ++i )
        verticalHeaders.append( ENT_Tr::ConvertFromAviationRange( static_cast< E_AviationRange >( i ), ENT_Tr_ABC::eToTr ).c_str() );
    dataModel_.setVerticalHeaderLabels( verticalHeaders );
}
