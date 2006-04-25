//*****************************************************************************
//
// $Created: JDY 03-06-25 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Objects_GUI.h $
// $Author: Nld $
// $Modtime: 29/04/05 16:37 $
// $Revision: 9 $
// $Workfile: ADN_Objects_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Objects_GUI_h_
#define __ADN_Objects_GUI_h_

#include "ADN_GUI_ABC.h"

class QWidget;
class ADN_Objects;
class ADN_Objects_Data;


// =============================================================================
/** @class  ADN_Objects_GUI
    @brief  ADN_Objects_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Objects_GUI 
: public ADN_GUI_ABC
{
    Q_OBJECT

public:
    enum E_GuiElements
    {
        eName,
        eCanBeBypassed,
        eAvoidDistance,
        eDefaultSpeed,
        eDefaultBypassSpeed,
        eMaxInteractionHeight,
        eDefaultConsumption,
        eSpeedImpact,
        eMaxAgentSpeedPercentage,
        eHasOutgoingPopulationDensity,
        eOutgoingPopulationDensity,
        eMaxNbrUsers,
        eToBuild,
        eToReinforce,
        eLocationScores,
        eAttritions,
        eHasToBuild,
        eHasToReinforce,
        eHasAttritions,
        eHasPopulationAttritions,
        ePopulationAttritionSurface,
        ePopulationAttritionPh,
        eCanBePrepared,
        eCanBeValorized,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Objects_GUI( ADN_Objects_Data& data );
    ~ADN_Objects_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private slots:
    void OnSpeedImpactComboChanged();

private:
    //! @name Member data
    //@{
    ADN_Objects_Data& data_;

    QComboBox* pSpeedImpactCombo_;
    QWidget* pMaxAgentSpeed_;
    //@}
};


#endif // __ADN_Objects_GUI_h_