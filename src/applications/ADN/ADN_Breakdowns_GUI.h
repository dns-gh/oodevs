// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-17 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Breakdowns_GUI.h $
// $Author: Nld $
// $Modtime: 27/04/05 10:05 $
// $Revision: 3 $
// $Workfile: ADN_Breakdowns_GUI.h $
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
class ADN_Breakdowns_GUI
: public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_Breakdowns_GUI )

public:
    enum E_BreakdownGuiElements
    {
        eName,
        eType,
        eNTI,
        eId,
        eRepairTime,
        eRepairTimeVariance,
        eParts,
        eNbrBreakdownGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Breakdowns_GUI( ADN_Breakdowns_Data& data );
    ~ADN_Breakdowns_GUI();
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
