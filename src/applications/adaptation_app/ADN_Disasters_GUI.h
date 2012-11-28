// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Disasters_GUI_h_
#define __ADN_Disasters_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Disasters_Data;

// =============================================================================
/** @class  ADN_Disasters_GUI
    @brief  ADN_Disasters_GUI
*/
// Created: LGY 2012-11-12
// =============================================================================
class ADN_Disasters_GUI: public ADN_GUI_ABC
{
public:
    enum E_GuiElements
    {
        eName,
        eConcentrationThresholds,
        eAttritionThresholds,
        eNbcSuitRatio,
        eToxicityExponent,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Disasters_GUI( ADN_Disasters_Data& data );
    virtual ~ADN_Disasters_GUI();
    //@}

    //! @name Operations
    //@{
    virtual void Build();
    //@}

private:
    //! @name Member Data
    //@{
    ADN_Disasters_Data& data_;
    //@}
};

#endif // __ADN_Disasters_GUI_h_
