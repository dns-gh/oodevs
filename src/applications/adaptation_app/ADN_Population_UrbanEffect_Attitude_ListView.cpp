// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Population_UrbanEffect_Attitude_ListView.h"
#include "ADN_Population_Data.h"
#include "ADN_Population_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tr.h"

typedef ADN_Population_Data::UrbanEffectInfos UrbanEffectInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Population_UrbanEffect_Attitude_ListView constructor
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
ADN_Population_UrbanEffect_Attitude_ListView::ADN_Population_UrbanEffect_Attitude_ListView( QWidget* pParent, const char* szName, Qt::WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tools::translate( "ADN_Population_UrbanEffect_Attitude_ListView", "Attitudes" ) );
    setSortColumn( -1 );
    setResizeMode( Q3ListView::AllColumns );
    // Connector creation
    pConnector_ = new ADN_Connector_ListView< UrbanEffectInfos >( *this );
    this->SetDeletionEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_UrbanEffect_Attitude_ListView destructor
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
ADN_Population_UrbanEffect_Attitude_ListView::~ADN_Population_UrbanEffect_Attitude_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_UrbanEffect_Attitude_ListView::ConnectItem
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
void ADN_Population_UrbanEffect_Attitude_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    UrbanEffectInfos* pInfos = (UrbanEffectInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Population_GUI::eNbrGuiElements );
    vItemConnectors_[ADN_Population_GUI::eUrbanBlocDestructionDensity]->Connect( &pInfos->rDensity_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eUrbanBlocDestructionTime]->Connect( &pInfos->rTime_, bConnect );
}
