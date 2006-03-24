//*****************************************************************************
//
// $Created: JDY 03-07-17 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composantes_GUI.h $
// $Author: Nld $
// $Modtime: 20/07/05 13:50 $
// $Revision: 18 $
// $Workfile: ADN_Composantes_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Composantes_GUI_h_
#define __ADN_Composantes_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Connector_ABC.h"

class QWidget;
class ADN_Table;
class ADN_Composantes_Data;

class ADN_ListView_Composantes;
class ADN_Composantes_Speeds_GUI;
class ADN_Composantes_Sensors_GUI;
class ADN_Composantes_RadarsListView;
class ADN_Composantes_Dotations_GUI;
class ADN_Composantes_ConsumptionsTable;
class ADN_Composantes_WeaponsListView;

// =============================================================================
/** @class  ADN_Composantes_GUI
@brief  ADN_Composantes_GUI
*/
// Created: APE 2004-12-07
// =============================================================================
class ADN_Composantes_GUI : public ADN_GUI_ABC
{
    Q_OBJECT
        MT_COPYNOTALLOWED( ADN_Composantes_GUI )

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
        eDeviceCategory,
        eTroopCapacity,
        eHasEmbarkTimes,
        eEmbarkingTimePerPerson,
        eDisembarkingTimePerPerson,
        eCanCarryCargo,
        eWeightCapacity,
        eEmbarkingTimePerTon,
        eDisembarkingTimePerTon,
        eSpeeds,
        eSensors,
        eRadars,
        eWeapons,
        eDotations,
        eConsumptions,
        eObjects,
        eAttritionBreakdowns,
        eRandomBreakdowns,
        eHasHealthLog,
        eHasAmbulance,
        eAmbulanceCapacity,
        eAmbulanceLoadTime,
        eAmbulanceUnloadTime,
        eHasRAmbulance,
        eRAmbulanceCapacity,
        eRAmbulanceLoadTime,
        eRAmbulanceUnloadTime,
        eIsDoctor,
        eIsSortingDoctor,
        eIsCuringDoctor,
        eDoctorSkills,
        eTreatsNBC = eDoctorSkills + eNbrDoctorSkills,
        eTreatsShock,
        eIsLogSupply,
        eIsLogCarrier,
        eLogCarryWeightCapacity,
        eLogCarryVolumeCapacity,
        eLogLoadTime,
        eLogUnloadTime,
        eLogCarryNature,
        eIsConvoyLeader,
        eIsConveyor,
        eIsMaintenance,
        eIsTower,
        eTowerCapacity,
        eMaintenanceLoadTimePerTon,
        eMaintenanceUnloadTimePerTon,
        eHasMaxSlope,
        eMaxSlope,
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
        eDevalorizationTime,
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
    ADN_Composantes_GUI( ADN_Composantes_Data& data );
    ~ADN_Composantes_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void RegisterTable( ADN_MainWindow& mainWindow );

    ADN_Table* CreateComposanteSpeedsTable();

    void ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const QString& strPath );
    //@}

private:
    //! @name Helpers
    //@{
    QWidget* BuildHealth( QWidget* pParent, T_ConnectorVector& vInfosConnectors );
    QWidget* BuildAmbulance( QWidget* pParent, const char* szName, T_ConnectorVector& vInfosConnectors, int nIndex );

    QWidget* BuildSupply( QWidget* pParent, T_ConnectorVector& vInfosConnectors );

    QWidget* BuildMaintenance( QWidget* pParent, T_ConnectorVector& vInfosConnectors );
    QWidget* BuildNTI( QWidget* pParent, const char* szName, T_ConnectorVector& vInfosConnectors, int nIndex );
    //@}

private slots:
    void OnProtectionTypeChanged();

private:
    //! @name Member data
    //@{
    ADN_Composantes_Data& data_;

    ADN_ListView_Composantes* pComposanteList_;
    QGroupBox* pBreakdownsGroup_;
    ADN_Composantes_Speeds_GUI* pSpeeds_;
    ADN_Composantes_Sensors_GUI* pSensors_;
    ADN_Composantes_RadarsListView* pRadars_;
    ADN_Composantes_Dotations_GUI* pDotations_;
    ADN_Composantes_ConsumptionsTable* pConsumptions_;
    ADN_Composantes_WeaponsListView* pWeapons_;
    //@}
};


#endif // __ADN_Composantes_GUI_h_