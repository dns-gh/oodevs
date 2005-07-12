//*****************************************************************************
//
// $Created: JDY 03-07-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Composantes.cpp $
// $Author: Ape $
// $Modtime: 22/06/05 15:03 $
// $Revision: 20 $
// $Workfile: ADN_ListView_Composantes.cpp $
//
//*****************************************************************************
#include "ADN_Pch.h"
#include "ADN_ListView_Composantes.h"

#include "ADN_App.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Workspace.h"
#include "ADN_Units_Data.h"
#include "ADN_Composante_Wizard.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Composantes_GUI.h"

#include <qpopmenu.h>

typedef ADN_Composantes_Data::ComposanteInfos ComposanteInfos;


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Composantes::ADN_ListView_Composantes( QWidget* pParent, const char* szName, WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Composantes" ) );
    setResizeMode(QListView::AllColumns);

    setMinimumWidth( 200 );

    // Connector creation.
    pConnector_ = new ADN_Connector_ListView<ComposanteInfos>( *this );

    this->SetDeletionEnabled( true );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Composantes::~ADN_ListView_Composantes()
{
   delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Composantes::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ComposanteInfos* pInfos = (ComposanteInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Composantes_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Composantes_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eComments]->Connect( &pInfos->strAdditionalComments_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eMosId]->Connect( &pInfos->nMosId_, bConnect );  
    vItemConnectors_[ADN_Composantes_GUI::eArmor]->Connect( &pInfos->ptrArmor_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eSize]->Connect( &pInfos->ptrSize_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eWeight]->Connect( &pInfos->rWeight_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eMaxSpeed]->Connect( &pInfos->rMaxSpeed_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNNOCode]->Connect( &pInfos->strCodeNNO_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eEMAT6Code]->Connect( &pInfos->strCodeEMAT6_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eEMAT8Code]->Connect( &pInfos->strCodeEMAT8_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eLFRILCode]->Connect( &pInfos->strCodeLFRIL_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eTroopCapacity]->Connect( &pInfos->nTroopTransportCapacity_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasEmbarkTimes]->Connect( &pInfos->bTroopEmbarkingTimes_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eEmbarkingTimePerPerson]->Connect( &pInfos->rEmbarkingTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eDisembarkingTimePerPerson]->Connect( &pInfos->rDisembarkingTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eCanCarryCargo]->Connect( &pInfos->bCanCarryCargo_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eWeightCapacity]->Connect( &pInfos->rWeightTransportCapacity_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eEmbarkingTimePerTon]->Connect( &pInfos->rEmbarkingTimePerTon_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eDisembarkingTimePerTon]->Connect( &pInfos->rDisembarkingTimePerTon_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eSpeeds]->Connect( &pInfos->vSpeeds_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eSensors]->Connect( &pInfos->vSensors_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eRadars]->Connect( &pInfos->vRadars_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eWeapons]->Connect( &pInfos->vWeapons_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eDotations]->Connect( &pInfos->dotations_.categories_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eConsumptions]->Connect( &pInfos->consumptions_.vConsumptions_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eObjects]->Connect( &pInfos->vObjects_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eAttritionBreakdowns]->Connect( &pInfos->attritionBreakdowns_.vBreakdowns_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eRandomBreakdowns]->Connect( &pInfos->randomBreakdowns_.vBreakdowns_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eAmbulanceCapacity]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.rCapacity_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eAmbulanceLoadTime]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.rLoadTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eAmbulanceUnloadTime]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.rUnloadTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasAmbulance]->Connect( &pInfos->logInfos_.healthInfos_.bIsAmbulance_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eRAmbulanceCapacity]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.rCapacity_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eRAmbulanceLoadTime]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.rLoadTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eRAmbulanceUnloadTime]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.rUnloadTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasRAmbulance]->Connect( &pInfos->logInfos_.healthInfos_.bIsAmbulanceReleve_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eIsDoctor]->Connect( &pInfos->logInfos_.healthInfos_.bIsDoctor_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eIsSortingDoctor]->Connect( &pInfos->logInfos_.healthInfos_.bIsSortingDoctor_, bConnect );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        vItemConnectors_[ADN_Composantes_GUI::eDoctorSkills + n]->Connect( &pInfos->logInfos_.healthInfos_.doctorSkills_[n], bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eIsCuringDoctor]->Connect( &pInfos->logInfos_.healthInfos_.bIsCuringDoctor_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eTreatsNBC]->Connect( &pInfos->logInfos_.healthInfos_.bCuresNBC_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eTreatsShock]->Connect( &pInfos->logInfos_.healthInfos_.bCuresShock_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eLogCarryCapacity]->Connect( &pInfos->logInfos_.supplyInfos_.rCapacity_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eLogLoadTimePerTon]->Connect( &pInfos->logInfos_.supplyInfos_.rLoadTimePerTon_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eLogUnloadTimePerTon]->Connect( &pInfos->logInfos_.supplyInfos_.rUnloadTimePerTon_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eIsLogCarrier]->Connect( &pInfos->logInfos_.supplyInfos_.bIsCarrier_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eIsConvoyLeader]->Connect( &pInfos->logInfos_.supplyInfos_.bIsConvoyLeader_, bConnect );
    
    vItemConnectors_[ADN_Composantes_GUI::eMaintenanceLoadTimePerTon]->Connect( &pInfos->logInfos_.maintenanceInfos_.rLoadTimePerTon_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eMaintenanceUnloadTimePerTon]->Connect( &pInfos->logInfos_.maintenanceInfos_.rUnloadTimePerTon_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eTowerCapacity]->Connect( &pInfos->logInfos_.maintenanceInfos_.rCapacity_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eIsTower]->Connect( &pInfos->logInfos_.maintenanceInfos_.bIsTower_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eMaxSlope]->Connect( &pInfos->rMaxSlope_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eNTI1 + ADN_Composantes_GUI::eMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.rMaxRepairTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI1 + ADN_Composantes_GUI::eHasMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.bHasMaxRepairTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI1 + ADN_Composantes_GUI::eCanRepairEA]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.bCanRepairEA_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI1 + ADN_Composantes_GUI::eCanRepairM]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.bCanRepairM_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI1 + ADN_Composantes_GUI::eNTIIsPresent]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.bIsPresent_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eNTI2 + ADN_Composantes_GUI::eMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.rMaxRepairTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI2 + ADN_Composantes_GUI::eHasMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.bHasMaxRepairTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI2 + ADN_Composantes_GUI::eCanRepairEA]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.bCanRepairEA_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI2 + ADN_Composantes_GUI::eCanRepairM]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.bCanRepairM_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI2 + ADN_Composantes_GUI::eNTIIsPresent]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.bIsPresent_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eNTI3 + ADN_Composantes_GUI::eMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.rMaxRepairTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI3 + ADN_Composantes_GUI::eHasMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.bHasMaxRepairTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI3 + ADN_Composantes_GUI::eCanRepairEA]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.bCanRepairEA_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI3 + ADN_Composantes_GUI::eCanRepairM]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.bCanRepairM_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI3 + ADN_Composantes_GUI::eNTIIsPresent]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.bIsPresent_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eIsMaintenance]->Connect( &pInfos->logInfos_.bHasMaintenanceInfos_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eIsLogSupply]->Connect( &pInfos->logInfos_.bHasSupplyInfos_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasHealthLog]->Connect( &pInfos->logInfos_.bHasHealthInfos_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasMaxSlope]->Connect( &pInfos->bMaxSlope_, bConnect );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_ListView_Composantes::OnContextMenu( const QPoint& pt )
{
    QPopupMenu popupMenu( this );
    ADN_Composante_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );   
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes::GetToolTipFor
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
std::string ADN_ListView_Composantes::GetToolTipFor( QListViewItem& item )
{
    void* pData = static_cast<ADN_ListViewItem&>( item ).GetData();
    ComposanteInfos* pCastData = (ComposanteInfos*)pData;
    assert( pCastData != 0 );

    std::string strToolTip = tr( "<b>Used by:</b><br>" ).ascii();
    strToolTip += ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsThatUse( *pCastData );

    return strToolTip;
}
