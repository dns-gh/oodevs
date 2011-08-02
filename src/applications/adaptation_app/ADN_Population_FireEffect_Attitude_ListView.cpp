// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Population_FireEffect_Attitude_ListView.h"
#include "ADN_Population_Data.h"
#include "ADN_Population_GUI.h"
#include "ADN_Connector_ListView.h"

typedef ADN_Population_Data::FireEffectInfos FireEffectInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Population_FireEffect_Attitude_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Population_FireEffect_Attitude_ListView::ADN_Population_FireEffect_Attitude_ListView( QWidget* pParent, const char* szName, Qt::WFlags f )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Attitudes" ) );
    setSortColumn( -1 );
    setResizeMode( Q3ListView::AllColumns );
    // Connector creation
    pConnector_ = new ADN_Connector_ListView< FireEffectInfos >( *this );
    this->SetDeletionEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_FireEffect_Attitude_ListView destructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Population_FireEffect_Attitude_ListView::~ADN_Population_FireEffect_Attitude_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_FireEffect_Attitude_ListView::ConnectItem
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Population_FireEffect_Attitude_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    FireEffectInfos* pInfos = (FireEffectInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Population_GUI::eNbrGuiElements );
    vItemConnectors_[ADN_Population_GUI::eFireEffectProtection]->Connect( &pInfos->vProtectionInfos_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eFireEffectIntensityDensity]->Connect( &pInfos->rIntensityDensity_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eFireEffectIntensityFactor] ->Connect( &pInfos->rIntensityFactor_, bConnect );
}
