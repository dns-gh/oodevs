// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_LocalFireClass_GUI_h_
#define __ADN_LocalFireClass_GUI_h_

#include "ADN_GUI_ABC.h"

// =============================================================================
/** @class  ADN_LocalFireClass_GUI
    @brief  ADN_LocalFireClass_GUI
*/
// Created: JSR 2010-12-01
// =============================================================================
class ADN_LocalFireClass_GUI : public ADN_GUI_ABC
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
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
             ADN_LocalFireClass_GUI( ADN_LocalFireClass_Data& );
    virtual ~ADN_LocalFireClass_GUI();
    //@}

    //! @name Operations
    //@{
    virtual void Build();
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    ADN_LocalFireClass_Data& data_;
    //@}
};

#endif // __ADN_LocalFireClass_GUI_h_
