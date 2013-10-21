// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Crowds_SpeedEffect_Attitude_ListView.h"
#include "ADN_Crowds_Data.h"
#include "ADN_Crowds_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tr.h"

typedef ADN_Crowds_Data::SpeedEffectInfos SpeedEffectInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_SpeedEffect_Attitude_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Crowds_SpeedEffect_Attitude_ListView::ADN_Crowds_SpeedEffect_Attitude_ListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Crowds_SpeedEffect_Attitude_ListView", tools::translate( "ADN_Crowds_SpeedEffect_Attitude_ListView", "Attitudes" ) )
{
    pConnector_.reset( new ADN_Connector_ListView< SpeedEffectInfos >( *this ) );
    SetDeletionEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_SpeedEffect_Attitude_ListView destructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Crowds_SpeedEffect_Attitude_ListView::~ADN_Crowds_SpeedEffect_Attitude_ListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_SpeedEffect_Attitude_ListView::ConnectItem
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Crowds_SpeedEffect_Attitude_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    SpeedEffectInfos* pInfos = (SpeedEffectInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Crowds_GUI::eNbrGuiElements );
    vItemConnectors_[ADN_Crowds_GUI::eSpeedEffectVolume]->Connect( &pInfos->vVolumeInfos_, bConnect );
}
