// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Radars_GUI_h_
#define __ADN_Radars_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_Enums.h"

class ADN_Radars_Data;

// =============================================================================
/** @class  ADN_Radars_GUI
    @brief  ADN_Radars_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Radars_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    enum E_GuiElements
    {
        eName,
        eType,
        eRange,
        eHasMaxHeight,
        eMaxHeight,
        eHasMinHeight,
        eMinHeight,
        eHasDetectableActivities,
        eHasDetectionTimes = eHasDetectableActivities + eNbrConsumptionType + 1,
        eHasDetectionTime,
        eDetectionTime,
        eHasRecoTime,
        eRecoTime,
        eHasIdentificationTime,
        eIdentificationTime,
        eHasHQDetectionTimes,
        eHasHQDetectionTime,
        eHQDetectionTime,
        eHasHQRecoTime,
        eHQRecoTime,
        eHasHQIdentificationTime,
        eHQIdentificationTime,
        eModifSizes,
        eModifWeather,
        eModifIllumination,
        eModifEnvironment,
        eModifUrbanBlockMaterial,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Radars_GUI( ADN_Radars_Data& data );
    virtual ~ADN_Radars_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Radars_Data& data_;
    //@}
};

#endif // __ADN_Radars_GUI_h_
