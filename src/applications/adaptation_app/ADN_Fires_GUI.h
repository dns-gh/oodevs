// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Fires_GUI_h_
#define __ADN_Fires_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Fires_Data;

// =============================================================================
/** @class  ADN_Fires_GUI
    @brief  ADN_Fires_GUI
*/
// Created: JSR 2010-12-01
// =============================================================================
class ADN_Fires_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    enum E_GuiElements
    {
        eName,
        eInitialHeat,
        eMaxHeat,
        eIncreaseRate,
        eDecreaseRate,
        eNbrHurtU1,
        eNbrHurtU2,
        eNbrHurtU3,
        eNbrHurtUE,
        eNbrDead,
        eUrbanAttrition,
        eExtinguisherAgents,
        eWeatherFireEffects,
        eSurface,
        eSurfaceEffects,
        eCellSize,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Fires_GUI( ADN_Fires_Data& );
    virtual ~ADN_Fires_GUI();
    //@}

    //! @name Operations
    //@{
    virtual void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Fires_Data& data_;
    //@}
};

#endif // __ADN_Fires_GUI_h_
