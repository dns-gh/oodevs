// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Breakdowns_GUI_h_
#define __ADN_Breakdowns_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Breakdowns_Data;

// =============================================================================
/** @class  ADN_Breakdowns_GUI
    @brief  ADN_Breakdowns_GUI
*/
// Created: APE 2005-03-17
// =============================================================================
class ADN_Breakdowns_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    enum E_BreakdownGuiElements
    {
        eName,
        eType,
        eNTI,
        eRepairTime,
        eRepairTimeVariance,
        eParts,
        eNbrBreakdownGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Breakdowns_GUI( ADN_Breakdowns_Data& data );
    virtual ~ADN_Breakdowns_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Breakdowns_Data& data_;
    //@}
};

#endif // __ADN_Breakdowns_GUI_h_
