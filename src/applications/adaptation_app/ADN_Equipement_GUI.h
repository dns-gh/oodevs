// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Equipement_GUI_h_
#define __ADN_Equipement_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Equipement_Data.h"

class ADN_CheckBox;
class ADN_ComboBox;
class ADN_Equipement_AttritionTable;
class ADN_Equipement_AttritionGraph;
class ADN_ListView;
class ADN_Table;
class ADN_ComboBox_Vector;

namespace helpers
{
    class ADN_UrbanAttritionInfos;
}

// =============================================================================
/** @class  ADN_Equipement_GUI
    @brief  ADN_Equipement_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Equipement_GUI : public ADN_Tabbed_GUI_ABC // $$$$ ABR 2012-01-20: Misnamed, should be ADN_Equipment_GUI ...
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
        eExplosivePresent,
        eSmokePresent,
        eFlarePresent,
        eMinePresent,
        eEffectPresent,
        eNeutralizationRatio,
        eDispersionFactor,
        eModifStances,
        eSmokeDeployTime,
        eSmokeLifetime,
        eFlareDeployTime,
        eFlareLifetime,
        eEffectType,
        eEffectLifetime,
        eMineNumber,
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
        eNbrGenericGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Equipement_GUI( ADN_Equipement_Data& data );
    virtual ~ADN_Equipement_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void RegisterTable( ADN_MainWindow& mainWindow );
    ADN_Table* CreatePKTable();
    void ExportPKs( ADN_HtmlBuilder& builder, ADN_Equipement_Data::AmmoCategoryInfo& infos );
    void UpdateGraph();
    void InitializeSimulationCombos();
    helpers::ArmorInfos* GetSelectedArmor() const;
    helpers::ADN_UrbanAttritionInfos* GetSelectedMaterial() const;
    //@}

private:
    //! @name Helpers
    //@{
    void BuildGeneric( E_DotationFamily nType );
    void BuildAmmunition();
    //@}

private slots:
    //! @name Slots
    //@{
    void IndirectTypeChanged();
    void SimulationCombosActivated();
    void NetworkUsableActivated( int state );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Equipement_Data&            data_;
    std::vector< ADN_CheckBox* >    vNetworkUsableCheckBoxs_;
    ADN_Equipement_AttritionTable*  pAttritionTable_;
    ADN_Equipement_AttritionGraph*  pAttritionGraph_;
    ADN_ComboBox_Vector*            pArmorCombo_;
    ADN_ComboBox_Vector*            pMaterialCombo_;
    QButtonGroup*                   buttonGroup_;
    Q3GroupBox*                     pExplosiveParametersGroup_;
    Q3GroupBox*                     pFlareParametersGroup_;
    Q3GroupBox*                     pEffectParametersGroup_;
    Q3GroupBox*                     pMineParametersGroup_;
    Q3GroupBox*                     pSmokeParametersGroup_;
    //@}
};

#endif // __ADN_Equipement_GUI_h_
