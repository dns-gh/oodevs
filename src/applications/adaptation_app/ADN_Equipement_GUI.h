// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_GUI.h $
// $Author: Nld $
// $Modtime: 20/07/05 14:10 $
// $Revision: 11 $
// $Workfile: ADN_Equipement_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_Equipement_GUI_h_
#define __ADN_Equipement_GUI_h_

#include <qobject.h>

#include "ADN_GUI_ABC.h"
#include "ADN_Enums.h"
#include "ADN_EditLine.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Equipement_Data.h"

class ADN_Table;
class ADN_Equipement_AttritionTable;
class ADN_Equipement_AttritionGraph;
class ADN_Equipement_UrbanAttritionTable;
class ADN_Equipement_AmmoListView;
class ADN_ComboBox;
template < class T > class ADN_ComboBox_Vector;
class QLabel;
class QWidget;
class QTabWidget;
class QGroupBox;


// =============================================================================
/** @class  ADN_Equipement_GUI
    @brief  ADN_Equipement_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Equipement_GUI
: public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_Equipement_GUI )
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
        eTrancheD,
        eEMAT6Code,
        eEMAT8Code,
        eLFRILCode,
        eNNOCode,
        eAttritions,
        eUrbanAttritions,
        eArmor,
        eMaterial,
        eAttritionGraph,
        eIndirectType,
        eIntervention,
        eDispersionX,
        eDispersionY,
        eNeutralizationRatio,
        eModifStances,
        eDeployTime,
        eLifetime,
        eEffectType,
        eMineNumber,
        eDirect,
        eIndirect,
        eIlluminating,
        eMaintainIllumination,
        eRange,
        eGuided,
        eMaintainGuidance,
        eGuidanceRange,
        eNbrAmmoGuiElements
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
        eNbrGenericGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Equipement_GUI( ADN_Equipement_Data& data );
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
    ADN_Categories_Data::ArmorInfos* GetSelectedArmor() const;
    ADN_Equipement_Data::UrbanAttritionInfos* GetSelectedMaterial() const;
    //@}

private:
    //! @name Helpers
    //@{
    void BuildGeneric( E_DotationFamily nType, QTabWidget* pParent );
    void BuildAmmunition( QTabWidget* pParent );
    //@}

private slots:
    void IndirectTypeComboActivated( int nIndex );
    void SimulationCombosActivated( int nIndex );

private:
    //! @name Member data
    //@{
    ADN_Equipement_Data& data_;

    ADN_Equipement_AmmoListView*        pAmmoListView_;
    ADN_Equipement_AttritionTable*      pAttritionTable_;
    ADN_Equipement_UrbanAttritionTable* pUrbanAttritionTable_;

    ADN_ComboBox* pIndirectTypeCombo_;
    ADN_Equipement_AttritionGraph* pAttritionGraph_;
    ADN_ComboBox_Vector< ADN_Categories_Data::ArmorInfos >* pArmorCombo_;
    ADN_ComboBox_Vector< ADN_Equipement_Data::UrbanAttritionInfos >* pMaterialCombo_;

    QGroupBox* pExplosiveParametersGroup_;
    QGroupBox* pFlareParametersGroup_;
    QGroupBox* pEffectParametersGroup_;
    QGroupBox* pMineParametersGroup_;
    //@}
};


#endif // __ADN_Equipement_GUI_h_
