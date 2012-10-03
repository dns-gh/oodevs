// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Population_SpeedEffect_Attitude_ListView.h"
#include "ADN_Population_Data.h"
#include "ADN_Population_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tr.h"

typedef ADN_Population_Data::SpeedEffectInfos SpeedEffectInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Population_SpeedEffect_Attitude_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Population_SpeedEffect_Attitude_ListView::ADN_Population_SpeedEffect_Attitude_ListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Population_SpeedEffect_Attitude_ListView", tools::translate( "ADN_Population_SpeedEffect_Attitude_ListView", "Attitudes" ) )
{
    // Connector creation
    pConnector_ = new ADN_Connector_ListView< SpeedEffectInfos >( *this );
    this->SetDeletionEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_SpeedEffect_Attitude_ListView destructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Population_SpeedEffect_Attitude_ListView::~ADN_Population_SpeedEffect_Attitude_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_SpeedEffect_Attitude_ListView::ConnectItem
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Population_SpeedEffect_Attitude_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    SpeedEffectInfos* pInfos = (SpeedEffectInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Population_GUI::eNbrGuiElements );
    vItemConnectors_[ADN_Population_GUI::eSpeedEffectVolume]->Connect( &pInfos->vVolumeInfos_, bConnect );
}
