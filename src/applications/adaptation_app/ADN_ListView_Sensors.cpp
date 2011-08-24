//*****************************************************************************
//
// $Created: JDY 03-07-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Sensors.cpp $
// $Author: Ape $
// $Modtime: 25/04/05 11:14 $
// $Revision: 9 $
// $Workfile: ADN_ListView_Sensors.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView_Sensors.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Sensors_GUI.h"
#include "ADN_Sensor_Wizard.h"
#include "ADN_Tools.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Sensors_DetectionAlgorithmPrevision.h"

typedef ADN_Sensors_Data::SensorInfos SensorInfos;

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Sensors constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Sensors::ADN_ListView_Sensors( QWidget* pParent, const char* szName, Qt::WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Sensors" ) );
    setResizeMode( Q3ListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<SensorInfos>( *this );

    this->SetDeletionEnabled( true );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Sensors destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Sensors::~ADN_ListView_Sensors()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Sensors::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Sensors::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    SensorInfos *pInfos = (SensorInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Sensors_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Sensors_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eAngle]->Connect( &pInfos->rAngle_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eCanScan]->Connect( &pInfos->bCanScan_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eDistFirerReco]->Connect( &pInfos->rFirerDetectionRange_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eDistProximity]->Connect( &pInfos->rDistProximity_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eDistDetection]->Connect( &pInfos->rDistDetection_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eDistReco]->Connect( &pInfos->rDistReco_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eDistIdent]->Connect( &pInfos->rDistIdent_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eModifSizes]->Connect( &pInfos->vModifSizes_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eModifWeather]->Connect( &pInfos->vModifWeather_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eModifIllumination]->Connect( &pInfos->vModifIlluminations_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eModifEnvironement]->Connect( &pInfos->vModifEnvironments_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eModifUrbanBlockMaterial]->Connect( &pInfos->vModifUrbanBlocks_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eModifStances]->Connect( &pInfos->vModifStance_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eModifTargetStances]->Connect( &pInfos->vModifTargetStance_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eTargets]->Connect( &pInfos->vTargets_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::ePopulationDensity]->Connect( &pInfos->populationInfos_.rDensity_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::ePopulationModifier]->Connect( &pInfos->populationInfos_.rModifier_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eLimitedSensorsList]->Connect( &pInfos->vLimitedToSensorsInfos_, bConnect ); // LTO

    // Order is important.
    vItemConnectors_[ADN_Sensors_GUI::eLimitedToSensors]->Connect( &pInfos->bLimitedToSensors_, bConnect ); // LTO
    vItemConnectors_[ADN_Sensors_GUI::eDetectionDelay]->Connect( &pInfos->detectionDelay_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eCanDetectAgents]->Connect( &pInfos->bCanDetectAgents_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eCanDetectObjects]->Connect( &pInfos->bCanDetectObjects_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::ePreviewModifSizes]->Connect( &pInfos->vModifSizes_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::ePreviewModifWeather]->Connect( &pInfos->vModifWeather_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::ePreviewModifIllumination]->Connect( &pInfos->vModifIlluminations_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::ePreviewModifEnvironement]->Connect( &pInfos->vModifEnvironments_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::ePreviewModifUrbanBlockMaterial]->Connect( &pInfos->vModifUrbanBlocks_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::ePreviewModifStances]->Connect( &pInfos->vModifStance_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::ePreviewModifTargetStances]->Connect( &pInfos->vModifTargetStance_, bConnect );

}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Sensors::OnContextMenu
// Created: JDY 03-07-28
//-----------------------------------------------------------------------------
void ADN_ListView_Sensors::OnContextMenu( const QPoint& pt)
{
    Q3PopupMenu popupMenu( this );
    ADN_Sensor_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Sensors::GetToolTipFor
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
std::string ADN_ListView_Sensors::GetToolTipFor( Q3ListViewItem& item )
{
    void* pData = static_cast<ADN_ListViewItem&>( item ).GetData();
    SensorInfos* pCastData = (SensorInfos*)pData;
    assert( pCastData != 0 );

    std::string strToolTip = tr( "<b>Used by:</b><br>" ).ascii();
    strToolTip += ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantesThatUse( *pCastData );

    return strToolTip;
}
