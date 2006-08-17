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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Population_GUI.h $
// $Author: Ape $
// $Modtime: 4/04/05 11:32 $
// $Revision: 7 $
// $Workfile: ADN_Population_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_Population_GUI_h_
#define __ADN_Population_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Population_Data;
class ADN_Table;


// =============================================================================
/** @class  ADN_Population_GUI
    @brief  ADN_Population_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Population_GUI : public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_Population_GUI )

public:
    enum E_GuiElements
    {
        eName,
        eModel,
        eConcentrationDensity,
        eMoveDensity,
        eMoveSpeed,
        eSpeedEffectAttitude,
        eSpeedEffectVolume,
        eSpeedEffectDensity,
        eSpeedEffectMaxSpeed,
        eReloadingEffectDensity,
        eReloadingEffectModifier,
        eFireEffectAttitude,
        eFireEffectProtection,
        eFireEffectIntensityDensity,
        eFireEffectIntensityFactor,
        eFireEffectDestruction,
        eFireEffectFixableWithEvacuation,
        eFireEffectFixableWithoutEvacuation,
        eFireEffectRoe,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Population_GUI( ADN_Population_Data& data );
    ~ADN_Population_GUI();
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
