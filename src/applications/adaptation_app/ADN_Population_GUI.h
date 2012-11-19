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

class ADN_Population_Data;

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
    //@}

private:
    //! @name Member data
    //@{
    ADN_Population_Data& data_;
    //@}
};

#endif // __ADN_Population_GUI_h_
