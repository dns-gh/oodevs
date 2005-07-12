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
// $Author: Ape $
// $Modtime: 21/04/05 16:17 $
// $Revision: 10 $
// $Workfile: ADN_Equipement_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_Equipement_GUI_h_
#define __ADN_Equipement_GUI_h_

#include <qobject.h>

#include "ADN_GUI_ABC.h"
#include "ADN_Enums.h"
#include "ADN_EditLine.h"
#include "ADN_Equipement_Data.h"

class ADN_Table;
class ADN_Equipement_AttritionTable;
class ADN_Equipement_AmmoListView;
class ADN_ComboBox;
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
        eAmmoNbrInUnitVolume,
        eType,
        eTrancheD,
        eEMAT6Code,
        eEMAT8Code,
        eLFRILCode,
        eNNOCode,
        eAttritions,
        eIndirectType,
        eIntervention,
        eDispersionX,
        eDispersionY,
        eNeutralizationRatio,
        eDeployTime,
        eLifetime,
        eMineNumber,
        eDirect,
        eIndirect,
        eNbrAmmoGuiElements
    };

    enum E_GenericGuiElements
    {
        eName,
        eNbrInUnitVolume,
        eGenEMAT6Code,
        eGenEMAT8Code,
        eGenLFRILCode,
        eGenNNOCode,
        eNbrGenericGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Equipement_GUI( ADN_Equipement_Data& data );
    ~ADN_Equipement_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void RegisterTable( ADN_MainWindow& mainWindow );

    ADN_Table* CreatePKTable();

    void ExportPKs( ADN_HtmlBuilder& builder, ADN_Equipement_Data::AmmoCategoryInfo& infos );
    //@}

private:
    //! @name Helpers
    //@{
    void BuildGeneric( E_TypeDotation nType, QTabWidget* pParent );
    void BuildAmmunition( QTabWidget* pParent );
    //@}

private slots:
    void IndirectTypeComboActivated( int nIndex );

private:
    //! @name Member data
    //@{
    ADN_Equipement_Data& data_;

    ADN_Equipement_AmmoListView* pAmmoListView_;
    ADN_Equipement_AttritionTable* pAttritionTable_;

    ADN_ComboBox* pIndirectTypeCombo_;
    
    QGroupBox* pExplosiveParametersGroup_;
    QGroupBox* pFlareParametersGroup_;
    QGroupBox* pMineParametersGroup_;
    //@}
};


#endif // __ADN_Equipement_GUI_h_
