// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Population_ListView.h"
#include "ADN_Population_Data.h"
#include "ADN_Population_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Population_Wizard.h"
#include "ADN_Tr.h"

typedef ADN_Population_Data::PopulationInfos PopulationInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Population_ListView constructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Population_ListView::ADN_Population_ListView( QWidget* pParent, const char* szName, Qt::WFlags f )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tools::translate( "ADN_Population_ListView", "Crowds" ) );
    setResizeMode( Q3ListView::AllColumns );
    // Connector creation
    pConnector_ = new ADN_Connector_ListView<PopulationInfos>(*this);
    this->SetDeletionEnabled( true );
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_ListView destructor
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
ADN_Population_ListView::~ADN_Population_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_ListView::ConnectItem
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Population_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    PopulationInfos* pInfos = (PopulationInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Population_GUI::eNbrGuiElements );
    vItemConnectors_[ADN_Population_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eModel]->Connect( &pInfos->ptrModel_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eConcentrationDensity]->Connect( &pInfos->rConcentrationDensity_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eMoveDensity]->Connect( &pInfos->rMoveDensity_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eMoveSpeed]->Connect( &pInfos->rMoveSpeed_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eMale ]->Connect( &pInfos->repartition_.male_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eFemale ]->Connect( &pInfos->repartition_.female_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eChildren ]->Connect( &pInfos->repartition_.children_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eArmedIndividuals ]->Connect( &pInfos->armedIndividuals_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eDecontaminationDelay ]->Connect( &pInfos->decontaminationDelay_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eSpeedEffectAttitude]->Connect( &pInfos->vSpeedEffectInfos_  , bConnect );
    vItemConnectors_[ADN_Population_GUI::eFireEffectAttitude] ->Connect( &pInfos->vFireEffectInfos_   , bConnect );
    vItemConnectors_[ADN_Population_GUI::eFireEffectRoe]      ->Connect( &pInfos->vFireEffectRoeInfos_, bConnect );
    vItemConnectors_[ADN_Population_GUI::eUrbanBlocDestructionAttitude] ->Connect( &pInfos->vUrbanEffectInfos_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_ListView::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Population_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Population_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}
