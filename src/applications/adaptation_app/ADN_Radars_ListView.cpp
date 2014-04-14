// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Radars_ListView.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Radars_Data.h"
#include "ADN_Radars_GUI.h"
#include "ADN_Tr.h"
#include "ADN_Tools.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Radars_Data::RadarInfos RadarInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Radars_ListView constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Radars_ListView::ADN_Radars_ListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Radars_ListView", tools::translate( "ADN_Radars_ListView", "Special sensors" ) )
{
    pConnector_.reset( new ADN_Connector_ListView< RadarInfos >( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_ListView destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Radars_ListView::~ADN_Radars_ListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_ListView::ConnectItem
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Radars_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    RadarInfos* pInfos = (RadarInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Radars_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Radars_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eType]->Connect( &pInfos->nType_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eRange]->Connect( &pInfos->rRange_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eMinHeight]->Connect( &pInfos->rMinHeight_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eMaxHeight]->Connect( &pInfos->rMaxHeight_, bConnect );

    for( int n = 0; n < eNbrConsumptionType; ++n )
        vItemConnectors_[ADN_Radars_GUI::eHasDetectableActivities + 1 + n]->Connect( &pInfos->detectableActivities_[n], bConnect );

    vItemConnectors_[ADN_Radars_GUI::eDetectionTime]->Connect( &pInfos->detectTimes_.detectTime_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eRecoTime]->Connect( &pInfos->detectTimes_.recoTime_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eIdentificationTime]->Connect( &pInfos->detectTimes_.identTime_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHQDetectionTime]->Connect( &pInfos->hqDetectTimes_.detectTime_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHQRecoTime]->Connect( &pInfos->hqDetectTimes_.recoTime_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHQIdentificationTime]->Connect( &pInfos->hqDetectTimes_.identTime_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHasMinHeight]->Connect( &pInfos->bHasMinHeight_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHasMaxHeight]->Connect( &pInfos->bHasMaxHeight_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHasDetectableActivities]->Connect( &pInfos->bHasDetectableActivities_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHasHQDetectionTime]->Connect( &pInfos->hqDetectTimes_.bDetectTime_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHasHQIdentificationTime]->Connect( &pInfos->hqDetectTimes_.bIdentTime_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHasHQRecoTime]->Connect( &pInfos->hqDetectTimes_.bRecoTime_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHasHQDetectionTimes]->Connect( &pInfos->bHasHQDetectTimes_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHasDetectionTime]->Connect( &pInfos->detectTimes_.bDetectTime_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHasIdentificationTime]->Connect( &pInfos->detectTimes_.bIdentTime_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHasRecoTime]->Connect( &pInfos->detectTimes_.bRecoTime_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eHasDetectionTimes]->Connect( &pInfos->bHasDetectTimes_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eModifSizes]->Connect( &pInfos->modificators_->vModifSizes_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eModifWeather]->Connect( &pInfos->modificators_->vModifWeather_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eModifIllumination]->Connect( &pInfos->modificators_->vModifIlluminations_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eModifEnvironment]->Connect( &pInfos->modificators_->vModifEnvironments_, bConnect );
    vItemConnectors_[ADN_Radars_GUI::eModifUrbanBlockMaterial]->Connect( &pInfos->modificators_->vModifUrbanBlocks_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_ListView::OnContextMenu
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Radars_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< RadarInfos > wizard( tools::translate( "ADN_Radars_ListView", "Special sensors" ), ADN_Workspace::GetWorkspace().GetSensors().GetData().radarData_->GetRadars(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        RadarInfos* pCastData = static_cast< RadarInfos* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(), ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ), eEquipments );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_ListView::GetToolTipFor
// Created: LDC 2012-05-03
// -----------------------------------------------------------------------------
std::string ADN_Radars_ListView::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    RadarInfos* pCastData = static_cast< RadarInfos* >( pData );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                            ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ) );
}