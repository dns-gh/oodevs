// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Crowds_GUI_h_
#define __ADN_Crowds_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Crowds_Data;

// =============================================================================
/** @class  ADN_Crowds_GUI
    @brief  ADN_Crowds_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Crowds_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    enum E_GuiElements
    {
        eName,
        eModel,
        eCrowdCollision,
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
    explicit ADN_Crowds_GUI( ADN_Crowds_Data& data );
    virtual ~ADN_Crowds_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Crowds_Data& data_;
    //@}
};

#endif // __ADN_Crowds_GUI_h_
