// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_People_GUI_h_
#define __ADN_People_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_EditLine.h"

class ADN_People_Data;
class ADN_People_ListView;

// =============================================================================
/** @class  ADN_People_GUI
    @brief  ADN_People_GUI
*/
// Created: SLG 2010-11-23
// =============================================================================
class ADN_People_GUI : public ADN_GUI_ABC
{

public:
    //! @name Types
    //@{
    enum E_GuiElements
    {
        eName,
        eModel,
        eMale,
        eFemale,
        eChildren,
        eLossOnFire,
        eGainPerHour,
        eHealthNeed,
        eTransferTime,
        eConsumptions,
        eNbrGuiElements,
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_People_GUI( ADN_People_Data& data );
    virtual ~ADN_People_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_People_Data& data_;
    //@}
};

#endif // __ADN_People_GUI_h_
