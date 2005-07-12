//*****************************************************************************
//
// $Created: JDY 03-07-24 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Units_GUI.h $
// $Author: Ape $
// $Modtime: 22/06/05 17:44 $
// $Revision: 11 $
// $Workfile: ADN_Units_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Units_GUI_h_
#define __ADN_Units_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_EditLine.h"
class ADN_Units_Data;
class QWidget;
class ADN_GroupBox;
class QComboBox;
class ADN_ListView_Units;


// =============================================================================
/** @class  ADN_Units_GUI
@brief  ADN_Units_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Units_GUI 
    : public ADN_GUI_ABC
{
    Q_OBJECT
        friend class ADN_Units;

public:
    enum E_GuiElements
    {
        eName,
        eTypeId,
        eModel,
        eMosId,
        eDecontaminationDelay,
        eStrengthRatioFeedbackTime,
        eProbeWidth,
        eProbeLength,
        eNatureLevel,
        eNatureWeapon,
        eNatureSpec,
        eNatureQualifer,
        eNatureCategory,
        eNatureMobility,
        eNatureAtlas,
        eMissionCapacity,
        eNbOfficer,
        eNbNCOfficer,
        ePostures,
        ePointInfos,
        eComposantes,
        eContenancesTC1,
        eStock,
        eCanProbe,
        eHasStock,
        eHasTC1,
        eHasStrengthRatioFeedbackTime,
        eCanFly,
        eWeaponsReach,
        eSensorsReach,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Units_GUI( ADN_Units_Data& data );
    ~ADN_Units_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private slots:
    void OnTypeChanged();
    void OnNbrOfOfficersChanged();
    void OnNbrOfNCOfficersChanged();

private:
    //! @name Member data
    //@{
    ADN_Units_Data& data_;

    ADN_ListView_Units* pListUnits_;
    ADN_EditLine_Int* pOfficersEditLine_;
    ADN_EditLine_Int* pNCOfficersEditLine_;

    QComboBox* pTypeCombo_;
    ADN_GroupBox* pStockGroup_;
    //@}
};


#endif // __ADN_Units_GUI_h_