// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ActiveProtections_GUI_h_
#define __ADN_ActiveProtections_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_ActiveProtections_Data;

// =============================================================================
// @class  ADN_ActiveProtections_GUI
// @brief  ADN_ActiveProtections_GUI
//
// Created: LDC 2010-01-13
// =============================================================================
class ADN_ActiveProtections_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    enum E_ActiveProtectionsGuiElements
    {
        eActiveProtectionName,
        eActiveProtectionHardKill,
        eActiveProtectionCoeffiscient,
        eActiveProtectionDotation,
        eActiveProtectionUsage,
        eActiveProtectionWeapons,
        eNbrActiveProtectionsGuiElements
    };

    //! @name Constructors/Destructor
    //@{
    explicit ADN_ActiveProtections_GUI( ADN_ActiveProtections_Data& data);
    virtual ~ADN_ActiveProtections_GUI() {}
    //@}

    //! @name Operations
    //@{
    virtual void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_ActiveProtections_Data& data_;
    //@}
};

#endif // __ADN_ActiveProtections_GUI_h_
