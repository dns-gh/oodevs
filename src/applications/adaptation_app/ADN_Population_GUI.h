// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Population_GUI_h_
#define __ADN_Population_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_EditLine.h"

class ADN_Population_Data;
class ADN_Population_ListView;
class ADN_Table;

// =============================================================================
/** @class  ADN_Population_GUI
    @brief  ADN_Population_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Population_GUI : public ADN_GUI_ABC
{
public:
    enum E_GuiElements
    {
        eName,
        eModel,
        eConcentrationDensity,
        eMoveDensity,
        eMoveSpeed,
        eMale,
        eFemale,
        eChildren,
        eArmedIndividuals,
        eDecontaminationDelay,
        eSpeedEffectAttitude,
        eSpeedEffectVolume,
        eSpeedEffectDensity,
        eSpeedEffectMaxSpeed,
        eTimeBetweenNBCApplication,
        eReloadingEffectDensity,
        eReloadingEffectModifier,
        eFireEffectAttitude,
        eFireEffectProtection,
        eFireEffectIntensityDensity,
        eFireEffectIntensityFactor,
        eFireEffectUnarmedDestruction,
        eFireEffectUnarmedFixableWithEvacuation,
        eFireEffectUnarmedFixableWithoutEvacuation,
        eFireEffectArmedDestruction,
        eFireEffectArmedFixableWithEvacuation,
        eFireEffectArmedFixableWithoutEvacuation,
        eUrbanBlocDestructionAttitude,
        eUrbanBlocDestructionDensity,
        eUrbanBlocDestructionTime,
        eFireEffectRoe,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Population_GUI( ADN_Population_Data& data );
    virtual ~ADN_Population_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void UpdatePopulationFireEffectValidator( ADN_Type_Double& destruUnarmed, ADN_Type_Double& evacUnarmed, ADN_Type_Double& withoutEvacUnarmed, ADN_Type_Double& destruArmed, ADN_Type_Double& evacArmed, ADN_Type_Double& withoutEvacArmed );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Population_Data& data_;

    ADN_PercentageValidator* pValidatorDestruUnarmed_;
    ADN_PercentageValidator* pValidatorEvacUnarmed_;
    ADN_PercentageValidator* pValidatorWithoutUnarmed_;
    ADN_PercentageValidator* pValidatorDestruArmed_;
    ADN_PercentageValidator* pValidatorEvacArmed_;
    ADN_PercentageValidator* pValidatorWithoutArmed_;

    //@}
};


#endif // __ADN_Population_GUI_h_
