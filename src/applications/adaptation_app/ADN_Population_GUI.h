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
    Q_OBJECT

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
        eFireEffectDestruction,
        eFireEffectFixableWithEvacuation,
        eFireEffectFixableWithoutEvacuation,
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

private slots:
    //! @name slots
    //@{
    void PercentageChanged();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Population_Data& data_;
    ADN_Population_ListView* pPopulationList_;
    ADN_EditLine_Int* pMaleEditLine_;
    ADN_EditLine_Int* pFemaleEditLine_;
    ADN_EditLine_Int* pChildrenEditLine_;
    ADN_EditLine_Int* pArmedIndividualsEditLine_;
    //@}
};


#endif // __ADN_Population_GUI_h_
