// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Equipments.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Workspace.h"
#include "ADN_Units_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Equipments_GUI.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"
#include <Qt3Support/q3popupmenu.h>

typedef ADN_Equipments_Data::EquipmentInfos EquipmentInfos;

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Equipments constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Equipments::ADN_ListView_Equipments( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_ListView_Equipments", ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str() )
{
    pConnector_.reset( new ADN_Connector_ListView< EquipmentInfos >( *this ) );
    SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Equipments destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Equipments::~ADN_ListView_Equipments()
{
   // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Equipments::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Equipments::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    EquipmentInfos* pInfos = static_cast< EquipmentInfos* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Equipments_GUI::eNbrGuiElements );

    vItemConnectors_[ADN_Equipments_GUI::eName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eComments]->Connect( &pInfos->strAdditionalComments_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eArmor]->Connect( &pInfos->ptrArmor_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eSize]->Connect( &pInfos->ptrSize_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eWeight]->Connect( &pInfos->rWeight_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eMaxSpeed]->Connect( &pInfos->rMaxSpeed_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNNOCode]->Connect( &pInfos->strCodeNNO_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eEMAT6Code]->Connect( &pInfos->strCodeEMAT6_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eEMAT8Code]->Connect( &pInfos->strCodeEMAT8_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eLFRILCode]->Connect( &pInfos->strCodeLFRIL_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNativeCountry]->Connect( &pInfos->strNativeCountry_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eStartingCountry]->Connect( &pInfos->strStartingCountry_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eStartingDate]->Connect( &pInfos->strStartingDate_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eInformationOrigin]->Connect( &pInfos->strInformationOrigin_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eEquipmentCategory]->Connect( &pInfos->equipmentCategory_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eEmbarkingTimePerPerson]->Connect( &pInfos->embarkingTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eDisembarkingTimePerPerson]->Connect( &pInfos->disembarkingTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eHasEmbarkTimes]->Connect( &pInfos->bTroopEmbarkingTimes_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eWeightCapacity]->Connect( &pInfos->rWeightTransportCapacity_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eEmbarkingTimePerTon]->Connect( &pInfos->embarkingTimePerTon_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eDisembarkingTimePerTon]->Connect( &pInfos->disembarkingTimePerTon_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eCanTransportDestroyed]->Connect( &pInfos->bCanTransportDestroyed_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eCanCarryCargo]->Connect( &pInfos->bCanCarryCargo_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eCrowdCapacity]->Connect( &pInfos->nCrowdTransportCapacity_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eCrowdEmbarkingTimePerPerson]->Connect( &pInfos->crowdEmbarkingTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eCrowdDisembarkingTimePerPerson]->Connect( &pInfos->crowdDisembarkingTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eCanCarryCrowd]->Connect( &pInfos->bCanCarryCrowd_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eSpeeds]->Connect( &pInfos->vSpeeds_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eSensors]->Connect( &pInfos->vSensors_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eRadars]->Connect( &pInfos->vRadars_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eWeapons]->Connect( &pInfos->vWeapons_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eActiveProtections]->Connect( &pInfos->vActiveProtections_, bConnect );

    vItemConnectors_[ADN_Equipments_GUI::eDotationsAllocation]->Connect( &pInfos->resources_.categories_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eDotations]->Connect( &pInfos->resources_.categories_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eConsumptions]->Connect( &pInfos->consumptions_.vConsumptions_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eObjects]->Connect( &pInfos->vObjects_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eDisasterImpacts]->Connect( &pInfos->vDisasterImpacts_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eAttritionBreakdowns]->Connect( &pInfos->attritionBreakdowns_.vBreakdowns_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eRandomBreakdowns]->Connect( &pInfos->randomBreakdowns_.vBreakdowns_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eAviationResourceQuotas]->Connect( &pInfos->vAviationResourceQuotas_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eHasAviationResourceQuotas]->Connect( &pInfos->bAviationResourcesQuotas_, bConnect );

    vItemConnectors_[ADN_Equipments_GUI::eHasAmbulance]->Connect( &pInfos->logInfos_.healthInfos_.bIsAmbulance_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eAmbulanceCapacity]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.nCapacity_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eAmbulanceLoadTime]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.loadTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eAmbulanceUnloadTime]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.unloadTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eAmbulanceTransportNBC]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.bTransportNBC_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eAmbulanceTransportShock]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.bTransportShock_, bConnect );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        vItemConnectors_[ADN_Equipments_GUI::eAmbulanceTransportSkills + n]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceInfos_.transportSkills_[n], bConnect );

    vItemConnectors_[ADN_Equipments_GUI::eHasRAmbulance]->Connect( &pInfos->logInfos_.healthInfos_.bIsAmbulanceReleve_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eRAmbulanceCapacity]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.nCapacity_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eRAmbulanceLoadTime]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.loadTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eRAmbulanceUnloadTime]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.unloadTimePerPerson_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eRAmbulanceTransportNBC]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.bTransportNBC_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eRAmbulanceTransportShock]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.bTransportShock_, bConnect );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        vItemConnectors_[ADN_Equipments_GUI::eRAmbulanceTransportSkills + n]->Connect( &pInfos->logInfos_.healthInfos_.ambulanceReleveInfos_.transportSkills_[n], bConnect );

    vItemConnectors_[ADN_Equipments_GUI::eIsDoctor]->Connect( &pInfos->logInfos_.healthInfos_.bIsDoctor_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eIsSortingDoctor]->Connect( &pInfos->logInfos_.healthInfos_.bIsSortingDoctor_, bConnect );
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        vItemConnectors_[ADN_Equipments_GUI::eDoctorSkills + n]->Connect( &pInfos->logInfos_.healthInfos_.doctorSkills_[n], bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eIsCuringDoctor]->Connect( &pInfos->logInfos_.healthInfos_.bIsCuringDoctor_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eTreatsNBC]->Connect( &pInfos->logInfos_.healthInfos_.bCuresNBC_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eTreatsShock]->Connect( &pInfos->logInfos_.healthInfos_.bCuresShock_, bConnect );

    vItemConnectors_[ADN_Equipments_GUI::eLogCarryMinVolumeCapacity]->Connect( &pInfos->logInfos_.supplyInfos_.rMinVolume_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eLogCarryMaxVolumeCapacity]->Connect( &pInfos->logInfos_.supplyInfos_.rMaxVolume_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eLogCarryMinWeightCapacity]->Connect( &pInfos->logInfos_.supplyInfos_.rMinWeight_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eLogCarryMaxWeightCapacity]->Connect( &pInfos->logInfos_.supplyInfos_.rMaxWeight_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eLogCarryNature]->Connect( &pInfos->logInfos_.supplyInfos_.ptr_, bConnect );

    vItemConnectors_[ADN_Equipments_GUI::eMaintenanceLoadTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.loadTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eMaintenanceUnloadTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.unloadTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eTowerCapacity]->Connect( &pInfos->logInfos_.maintenanceInfos_.rCapacity_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eIsTower]->Connect( &pInfos->logInfos_.maintenanceInfos_.bIsTower_, bConnect );
    
    vItemConnectors_[ADN_Equipments_GUI::eMaxSlope]->Connect( &pInfos->nMaxSlope_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eSlopeDeceleration]->Connect( &pInfos->nSlopeDeceleration_, bConnect );

    vItemConnectors_[ADN_Equipments_GUI::eLength]->Connect( &pInfos->length_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eWidth]->Connect( &pInfos->width_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eFrontSeparationDistance]->Connect( &pInfos->frontSeparationDistance_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eSafetyDistance]->Connect( &pInfos->safetyDistance_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eSpeedSafetyDistance]->Connect( &pInfos->speedSafetyDistance_, bConnect );

    vItemConnectors_[ADN_Equipments_GUI::eNTI1 + ADN_Equipments_GUI::eMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.maxRepairTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNTI1 + ADN_Equipments_GUI::eHasMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.bHasMaxRepairTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNTI1 + ADN_Equipments_GUI::eCanRepairEA]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.bCanRepairEA_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNTI1 + ADN_Equipments_GUI::eCanRepairM]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.bCanRepairM_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNTI1 + ADN_Equipments_GUI::eNTIIsPresent]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI1Infos_.bIsPresent_, bConnect );

    vItemConnectors_[ADN_Equipments_GUI::eNTI2 + ADN_Equipments_GUI::eMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.maxRepairTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNTI2 + ADN_Equipments_GUI::eHasMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.bHasMaxRepairTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNTI2 + ADN_Equipments_GUI::eCanRepairEA]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.bCanRepairEA_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNTI2 + ADN_Equipments_GUI::eCanRepairM]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.bCanRepairM_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNTI2 + ADN_Equipments_GUI::eNTIIsPresent]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI2Infos_.bIsPresent_, bConnect );

    vItemConnectors_[ADN_Equipments_GUI::eNTI3 + ADN_Equipments_GUI::eMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.maxRepairTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNTI3 + ADN_Equipments_GUI::eHasMaxRepairTime]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.bHasMaxRepairTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNTI3 + ADN_Equipments_GUI::eCanRepairEA]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.bCanRepairEA_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNTI3 + ADN_Equipments_GUI::eCanRepairM]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.bCanRepairM_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eNTI3 + ADN_Equipments_GUI::eNTIIsPresent]->Connect( &pInfos->logInfos_.maintenanceInfos_.NTI3Infos_.bIsPresent_, bConnect );

    vItemConnectors_[ADN_Equipments_GUI::eIsMaintenance]->Connect( &pInfos->logInfos_.bHasMaintenanceInfos_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eIsLogSupply]->Connect( &pInfos->logInfos_.bHasSupplyInfos_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eHasHealthLog]->Connect( &pInfos->logInfos_.bHasHealthInfos_, bConnect );

    vItemConnectors_[ ADN_Equipments_GUI::ePowerDirectFire ]->Connect( &pInfos->nPowerDirectFire_, bConnect );
    vItemConnectors_[ ADN_Equipments_GUI::ePowerIndirectFire ]->Connect( &pInfos->nPowerIndirectFire_, bConnect );
    vItemConnectors_[ ADN_Equipments_GUI::ePowerCloseCombat ]->Connect( &pInfos->nPowerCloseCombat_, bConnect );
    vItemConnectors_[ ADN_Equipments_GUI::ePowerEngineering ]->Connect( &pInfos->nPowerEngineering_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Equipments::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_ListView_Equipments::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< EquipmentInfos > wizard( ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(), ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipments(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        EquipmentInfos* pCastData = static_cast< EquipmentInfos* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                      ADN_Tr::ConvertFromWorkspaceElement( eUnits ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsThatUse( *pCastData ), eUnits );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Equipments::GetToolTipFor
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
std::string ADN_ListView_Equipments::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    EquipmentInfos* pCastData = static_cast< EquipmentInfos* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eUnits ).c_str(),
                            ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsThatUse( *pCastData ) );
}
