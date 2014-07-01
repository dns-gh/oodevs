// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Resources_GUI_h_
#define __ADN_Resources_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Resources_Data.h"

class ADN_CheckBox;
class ADN_ComboBox;
class ADN_Resources_AttritionTable;
class ADN_Resources_AttritionGraph;
class ADN_ListView;
class ADN_Table;
class ADN_ComboBox_Vector;

namespace helpers
{
    class ADN_UrbanAttritionInfos;
}

namespace sword
{
    enum DotationType;
}

// =============================================================================
/** @class  ADN_Resources_GUI
    @brief  ADN_Resources_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Resources_GUI : public ADN_Tabbed_GUI_ABC
{
    Q_OBJECT

public:
    enum E_AmmoGuiElements
    {
        eAmmoName,
        eAmmoPackageNbr,
        eAmmoPackageWeight,
        eAmmoPackageVolume,
        eType,
        eNature,
        eEMAT6Code,
        eEMAT8Code,
        eLFRILCode,
        eNNOCode,
        eAttritions,
        eUrbanAttritions,
        eArmor,
        eMaterial,
        eAttritionGraph,
        eIntervention,
        eDispersionX,
        eDispersionY,
        eDetectionRange,
        eExplosivePresent,
        eSmokePresent,
        eFlarePresent,
        eEffectPresent,
        eNeutralizationRatio,
        eExplosiveDispersion,
        eModifStances,
        eSmokeDeployTime,
        eSmokeLifetime,
        eSmokeDispersion,
        eFlareDeployTime,
        eFlareLifetime,
        eFlareDispersion,
        eEffectType,
        eEffectLifetime,
        eDirect,
        eIndirect,
        eIlluminating,
        eMaintainIllumination,
        eRange,
        eGuided,
        eMaintainGuidance,
        eGuidanceRange,
        eLogisticSupplyClass,
        eIsIED,
        eNetworkUsable,
        eNbrAmmoGuiElements
    };

    enum E_IndirectFire
    {
        eExplosive,
        eSmoke,
        eIlluminationShell,
        eMine,
        eEffect,
        eNbrIndirectFire
    };

    enum E_GenericGuiElements
    {
        eName,
        ePackageNbr,
        ePackageWeight,
        ePackageVolume,
        eGenEMAT6Code,
        eGenEMAT8Code,
        eGenLFRILCode,
        eGenNNOCode,
        eGenNature,
        eGenLogisticSupplyClass,
        eGenNetworkUsable,
        eGenType,
        eGenAttritions,
        eGenUrbanAttritions,
        eGenArmor,
        eGenMaterial,
        eGenAttritionGraph,
        eNbrGenericGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Resources_GUI( ADN_Resources_Data& data );
    virtual ~ADN_Resources_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void RegisterTable( ADN_MainWindow& mainWindow );
    QWidget* CreatePKTable();
    void ExportPKs( ADN_HtmlBuilder& builder, ADN_Resources_Data::AmmoCategoryInfo& infos );
    void UpdateGraph();
    void InitializeSimulationCombos();
    ADN_Armors_Data::ArmorInfos* GetSelectedArmor() const;
    helpers::ADN_UrbanAttritionInfos* GetSelectedMaterial() const;
    //@}

private:
    //! @name Helpers
    //@{
    void BuildGeneric( sword::DotationType nType );
    void BuildAmmunition();
    //@}

private slots:
    //! @name Slots
    //@{
    void IndirectTypeChanged();
    void SimulationCombosActivated();
    void NetworkUsableActivated( int state );
    void OnCurrentTabChanged( int tab );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Resources_Data&                                                data_;
    std::vector< ADN_CheckBox* >                                       vNetworkUsablecheckboxes_;
    sword::DotationType                                            currentTab_;
    std::map< sword::DotationType, ADN_Resources_AttritionTable* > pAttritionTables_;
    std::map< sword::DotationType, ADN_Resources_AttritionGraph* > pAttritionGraphs_;
    std::map< sword::DotationType, ADN_ComboBox_Vector* >          pArmorCombos_;
    std::map< sword::DotationType, ADN_ComboBox_Vector* >          pMaterialCombos_;
    QButtonGroup*                                                      buttonGroup_;
    Q3GroupBox*                                                        pExplosiveParametersGroup_;
    Q3GroupBox*                                                        pFlareParametersGroup_;
    Q3GroupBox*                                                        pEffectParametersGroup_;
    Q3GroupBox*                                                        pSmokeParametersGroup_;
    //@}
};

#endif // __ADN_Resources_GUI_h_
