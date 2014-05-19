// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipments_GUI_h_
#define __ADN_Equipments_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Connector_ABC.h"

class QWidget;
class ADN_ComboBox;
class ADN_Equipments_Data;
class ADN_Equipments_Speeds_GUI;
class ADN_Equipments_RadarsListView;
class ADN_Equipments_WeaponsListView;
class ADN_Equipments_ActiveProtectionsListView;
class ADN_Equipments_Resources_Tables;
class ADN_Equipments_AviationResourceQuotas_GUI;
class ADN_GuiBuilder;
class ADN_ListView_Equipments;
class ADN_Table;

// =============================================================================
/** @class  ADN_Equipments_GUI
@brief  ADN_Equipments_GUI
*/
// Created: APE 2004-12-07
// =============================================================================
class ADN_Equipments_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    enum E_NTIGuiElements
    {
        eNTIIsPresent,
        eHasMaxRepairTime,
        eMaxRepairTime,
        eCanRepairEA,
        eCanRepairM,
        eNbrNTIGuiElements
    };

    enum E_GuiElements
    {
        eName,
        eComments,
        eArmor,
        eSize,
        eWeight,
        eMaxSpeed,
        eNNOCode,
        eEMAT6Code,
        eEMAT8Code,
        eLFRILCode,
        eNativeCountry,
        eStartingCountry,
        eStartingDate,
        eInformationOrigin,
        eEquipmentCategory,
        eHasEmbarkTimes,
        eEmbarkingTimePerPerson,
        eDisembarkingTimePerPerson,
        eCanCarryCargo,
        eWeightCapacity,
        eEmbarkingTimePerTon,
        eDisembarkingTimePerTon,
        eCanTransportDestroyed,
        eCanCarryCrowd,
        eCrowdCapacity,
        eCrowdEmbarkingTimePerPerson,
        eCrowdDisembarkingTimePerPerson,
        eSpeeds,
        eSensors,
        eRadars,
        eWeapons,
        eActiveProtections,
        eDotations,
        eDotationsAllocation,
        eConsumptions,
        eHasAviationResourceQuotas,
        eAviationResourceQuotas,
        eObjects,
        eDisasterImpacts,
        eAttritionBreakdowns,
        eRandomBreakdowns,
        eHasHealthLog,
        eHasAmbulance,
        eAmbulanceCapacity,
        eAmbulanceLoadTime,
        eAmbulanceUnloadTime,
        eAmbulanceTransportNBC,
        eAmbulanceTransportShock,
        eAmbulanceTransportSkills,
        eHasRAmbulance = eAmbulanceTransportSkills + eNbrDoctorSkills,
        eRAmbulanceCapacity,
        eRAmbulanceLoadTime,
        eRAmbulanceUnloadTime,
        eRAmbulanceTransportNBC,
        eRAmbulanceTransportShock,
        eRAmbulanceTransportSkills,
        eIsDoctor = eRAmbulanceTransportSkills + eNbrDoctorSkills,
        eIsSortingDoctor,
        eIsCuringDoctor,
        eDoctorSkills,
        eTreatsNBC = eDoctorSkills + eNbrDoctorSkills,
        eTreatsShock,
        eIsLogSupply,
        eLogCarryMinWeightCapacity,
        eLogCarryMaxWeightCapacity,
        eLogCarryMinVolumeCapacity,
        eLogCarryMaxVolumeCapacity,
        eLogCarryNature,
        eIsMaintenance,
        eIsTower,
        eTowerCapacity,
        eMaintenanceLoadTime,
        eMaintenanceUnloadTime,
        eMaxSlope,
        eSlopeDeceleration,
        ePowerDirectFire,
        ePowerIndirectFire,
        ePowerCloseCombat,
        ePowerEngineering,
        eLength,
        eWidth,
        eFrontSeparationDistance,
        eSafetyDistance,
        eSpeedSafetyDistance,
        eNTI1,
        eNTI2 = eNTI1 + eNbrNTIGuiElements,
        eNTI3 = eNTI2 + eNbrNTIGuiElements,
        eNbrGuiElements = eNTI3 + eNbrNTIGuiElements
    };

    enum E_ObjGuiElements
    {
        eHasInitialBuildTime,
        eInitialBuildTime,
        eHasInitialDestructionTime,
        eInitialDestructionTime,
        eHasCoeffBuildTime,
        eCoeffBuildTime,
        eHasCoeffDestructionTime,
        eCoeffDestructionTime,
        eHasValorizationTime,
        eValorizationTime,
        eHasDevalorizationTime,
        eHasExtinguishingTime,
        eDevalorizationTime,
        eExtinguishingTime,
        eHasCoeffCircTime,
        eCoeffCircTime,
        eHasSpeedCirc,
        eSpeedCirc,
        eHasSpeedNotCirc,
        eSpeedNotCirc,
        eNbrObjGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Equipments_GUI( ADN_Equipments_Data& data );
    virtual ~ADN_Equipments_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void RegisterTable( ADN_MainWindow& mainWindow );

    void ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const tools::Path& strPath );
    //@}

private:
    //! @name Helpers
    //@{
    QWidget* BuildHealth( QWidget* pParent, T_ConnectorVector& vInfosConnectors, ADN_GuiBuilder& builder );
    QWidget* BuildAmbulance( QWidget* pParent, const char* objectName, const char* szName, T_ConnectorVector& vInfosConnectors, int nIndex, ADN_GuiBuilder& builder );

    QWidget* BuildSupply( QWidget* pParent, T_ConnectorVector& vInfosConnectors, ADN_GuiBuilder& builder );

    QWidget* BuildMaintenance( QWidget* pParent, T_ConnectorVector& vInfosConnectors, ADN_GuiBuilder& builder );
    QWidget* BuildNTI( QWidget* pParent, const char* objectName, const char* szName, T_ConnectorVector& vInfosConnectors, int nIndex, ADN_GuiBuilder& builder );

    QWidget* BuildPowerIndicators( QWidget* pParent, T_ConnectorVector& vInfosConnectors, ADN_GuiBuilder& builder );

    QWidget* CreateComposanteSpeedsTable();
    //@}

private slots:
    void OnProtectionTypeChanged();

private:
    //! @name Member data
    //@{
    ADN_Equipments_Data& data_;

    Q3GroupBox* pBreakdownsGroup_;
    ADN_Equipments_Speeds_GUI* pSpeeds_;
    ADN_Table* pSensors_;
    ADN_Table* pRadars_;
    ADN_Equipments_WeaponsListView* pWeapons_;
    ADN_Equipments_ActiveProtectionsListView* pActiveProtections_;
    ADN_Equipments_Resources_Tables* pResources_;
    ADN_Equipments_AviationResourceQuotas_GUI* pAviationResourceQuotas_;
    ADN_ComboBox* pCombo_;
    //@}
};

#endif // __ADN_Equipments_GUI_h_
