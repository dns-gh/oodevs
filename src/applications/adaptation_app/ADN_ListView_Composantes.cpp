//*****************************************************************************
//
// $Created: JDY 03-07-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Composantes.cpp $
// $Author: Nld $
// $Modtime: 20/07/05 13:54 $
// $Revision: 21 $
// $Workfile: ADN_ListView_Composantes.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView_Composantes.h"

#include "ADN_App.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Workspace.h"
#include "ADN_Units_Data.h"
#include "ADN_Composante_Wizard.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Composantes_GUI.h"

#include <Qt3Support/q3popupmenu.h>

typedef ADN_Composantes_Data::ComposanteInfos ComposanteInfos;


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Composantes::ADN_ListView_Composantes( QWidget* pParent, const char* szName, Qt::WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tools::translate( "ADN_ListView_Composantes", "Equipments" ) );
    setResizeMode(Q3ListView::AllColumns);

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
    vItemConnectors_[ADN_Composantes_GUI::eArmor]->Connect( &pInfos->ptrArmor_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eSize]->Connect( &pInfos->ptrSize_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eWeight]->Connect( &pInfos->rWeight_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eMaxSpeed]->Connect( &pInfos->rMaxSpeed_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNNOCode]->Connect( &pInfos->strCodeNNO_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eEMAT6Code]->Connect( &pInfos->strCodeEMAT6_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eEMAT8Code]->Connect( &pInfos->strCodeEMAT8_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eLFRILCode]->Connect( &pInfos->strCodeLFRIL_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNativeCountry]->Connect( &pInfos->strNativeCountry_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eStartingCountry]->Connect( &pInfos->strStartingCountry_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eStartingDate]->Connect( &pInfos->strStartingDate_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eInformationOrigin]->Connect( &pInfos->strInformationOrigin_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eEmbarkingTimePerPerson]->Connect( &pInfos->embarkingTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eDisembarkingTimePerPerson]->Connect( &pInfos->disembarkingTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasEmbarkTimes]->Connect( &pInfos->bTroopEmbarkingTimes_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eWeightCapacity]->Connect( &pInfos->rWeightTransportCapacity_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eEmbarkingTimePerTon]->Connect( &pInfos->embarkingTimePerTon_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eDisembarkingTimePerTon]->Connect( &pInfos->disembarkingTimePerTon_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eCanCarryCargo]->Connect( &pInfos->bCanCarryCargo_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eCrowdCapacity]->Connect( &pInfos->nCrowdTransportCapacity_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eCrowdEmbarkingTimePerPerson]->Connect( &pInfos->crowdEmbarkingTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eCrowdDisembarkingTimePerPerson]->Connect( &pInfos->crowdDisembarkingTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eCanCarryCrowd]->Connect( &pInfos->bCanCarryCrowd_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eSpeeds]->Connect( &pInfos->vSpeeds_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eSensors]->Connect( &pInfos->vSensors_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eRadars]->Connect( &pInfos->vRadars_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eWeapons]->Connect( &pInfos->vWeapons_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eActiveProtections]->Connect( &pInfos->vActiveProtections_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eDotations]->Connect( &pInfos->resources_.categories_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eConsumptions]->Connect( &pInfos->consumptions_.vConsumptions_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eObjects]->Connect( &pInfos->vObjects_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eAttritionBreakdowns]->Connect( &pInfos->attritionBreakdowns_.vBreakdowns_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eRandomBreakdowns]->Connect( &pInfos->randomBreakdowns_.vBreakdowns_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eHasAmbulance]->Connect( &pInfos->logInfos_.healthInfos_.bIsAmbulance_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eAmbulanceCapacity]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.rCapacity_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eAmbulanceLoadTime]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.loadTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eAmbulanceUnloadTime]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.unloadTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eAmbulanceTransportNBC]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.bTransportNBC_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eAmbulanceTransportShock]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.bTransportShock_, bConnect );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        vItemConnectors_[ADN_Composantes_GUI::eAmbulanceTransportSkills + n]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.transportSkills_[n], bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eHasRAmbulance]->Connect( &pInfos->logInfos_.healthInfos_.bIsAmbulanceReleve_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eRAmbulanceCapacity]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.rCapacity_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eRAmbulanceLoadTime]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.loadTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eRAmbulanceUnloadTime]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.unloadTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eRAmbulanceTransportNBC]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.bTransportNBC_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eRAmbulanceTransportShock]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.bTransportShock_, bConnect );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        vItemConnectors_[ADN_Composantes_GUI::eRAmbulanceTransportSkills + n]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.transportSkills_[n], bConnect );


    vItemConnectors_[ADN_Composantes_GUI::eIsDoctor]->Connect( &pInfos->logInfos_.healthInfos_.bIsDoctor_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eIsSortingDoctor]->Connect( &pInfos->logInfos_.healthInfos_.bIsSortingDoctor_, bConnect );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        vItemConnectors_[ADN_Composantes_GUI::eDoctorSkills + n]->Connect( &pInfos->logInfos_.healthInfos_.doctorSkills_[n], bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eIsCuringDoctor]->Connect( &pInfos->logInfos_.healthInfos_.bIsCuringDoctor_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eTreatsNBC]->Connect( &pInfos->logInfos_.healthInfos_.bCuresNBC_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eTreatsShock]->Connect( &pInfos->logInfos_.healthInfos_.bCuresShock_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eLogCarryVolumeCapacity]->Connect( &pInfos->logInfos_.supplyInfos_.rVolume_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eLogCarryWeightCapacity]->Connect( &pInfos->logInfos_.supplyInfos_.rWeight_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eLogCarryNature]->Connect( &pInfos->logInfos_.supplyInfos_.ptrResourceNature_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eIsLogCarrier]->Connect( &pInfos->logInfos_.supplyInfos_.bIsCarrier_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eMaintenanceLoadTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.loadTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eMaintenanceUnloadTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.unloadTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eTowerCapacity]->Connect( &pInfos->logInfos_.maintenanceInfos_.rCapacity_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eIsTower]->Connect( &pInfos->logInfos_.maintenanceInfos_.bIsTower_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eMaxSlope]->Connect( &pInfos->rMaxSlope_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eNTI1 + ADN_Composantes_GUI::eMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.maxRepairTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI1 + ADN_Composantes_GUI::eHasMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.bHasMaxRepairTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI1 + ADN_Composantes_GUI::eCanRepairEA]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.bCanRepairEA_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI1 + ADN_Composantes_GUI::eCanRepairM]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.bCanRepairM_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI1 + ADN_Composantes_GUI::eNTIIsPresent]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.bIsPresent_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eNTI2 + ADN_Composantes_GUI::eMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.maxRepairTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI2 + ADN_Composantes_GUI::eHasMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.bHasMaxRepairTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI2 + ADN_Composantes_GUI::eCanRepairEA]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.bCanRepairEA_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI2 + ADN_Composantes_GUI::eCanRepairM]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.bCanRepairM_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI2 + ADN_Composantes_GUI::eNTIIsPresent]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.bIsPresent_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eNTI3 + ADN_Composantes_GUI::eMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.maxRepairTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI3 + ADN_Composantes_GUI::eHasMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.bHasMaxRepairTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI3 + ADN_Composantes_GUI::eCanRepairEA]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.bCanRepairEA_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI3 + ADN_Composantes_GUI::eCanRepairM]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.bCanRepairM_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eNTI3 + ADN_Composantes_GUI::eNTIIsPresent]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.bIsPresent_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eIsMaintenance]->Connect( &pInfos->logInfos_.bHasMaintenanceInfos_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eIsLogSupply]->Connect( &pInfos->logInfos_.bHasSupplyInfos_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasHealthLog]->Connect( &pInfos->logInfos_.bHasHealthInfos_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasMaxSlope]->Connect( &pInfos->bMaxSlope_, bConnect );

    vItemConnectors_[ ADN_Composantes_GUI::ePowerDirectFire ]->Connect( &pInfos->nPowerDirectFire_, bConnect );
    vItemConnectors_[ ADN_Composantes_GUI::ePowerIndirectFire ]->Connect( &pInfos->nPowerIndirectFire_, bConnect );
    vItemConnectors_[ ADN_Composantes_GUI::ePowerCloseCombat ]->Connect( &pInfos->nPowerCloseCombat_, bConnect );
    vItemConnectors_[ ADN_Composantes_GUI::ePowerEngineering ]->Connect( &pInfos->nPowerEngineering_, bConnect );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_ListView_Composantes::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Composante_Wizard wizard( this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes::GetToolTipFor
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
std::string ADN_ListView_Composantes::GetToolTipFor( Q3ListViewItem& item )
{
    void* pData = static_cast<ADN_ListViewItem&>( item ).GetData();
    ComposanteInfos* pCastData = (ComposanteInfos*)pData;
    assert( pCastData != 0 );

    std::string strToolTip = tools::translate( "ADN_ListView_Composantes", "<b>Used by:</b><br>" ).ascii();
    strToolTip += ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsThatUse( *pCastData );

    return strToolTip;
}
