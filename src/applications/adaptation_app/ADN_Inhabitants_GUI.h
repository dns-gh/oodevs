// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Inhabitants_GUI_h_
#define __ADN_Inhabitants_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_EditLine.h"

class ADN_Inhabitants_Data;
class ADN_Inhabitants_ListView;

// =============================================================================
/** @class  ADN_Inhabitants_GUI
    @brief  ADN_Inhabitants_GUI
*/
// Created: SLG 2010-11-23
// =============================================================================
class ADN_Inhabitants_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    enum E_GuiElements
    {
        eName,
        eModel,
        eAngryCrowdMission,
        eMale,
        eFemale,
        eChildren,
        eLossOnFire,
        eGainPerHour,
        eHealthNeed,
        eTransferTime,
        eSchedule,
        eConsumptions,
        eNbrGuiElements,
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Inhabitants_GUI( ADN_Inhabitants_Data& data );
    virtual ~ADN_Inhabitants_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Inhabitants_Data& data_;
    //@}
};

#endif // __ADN_Inhabitants_GUI_h_
