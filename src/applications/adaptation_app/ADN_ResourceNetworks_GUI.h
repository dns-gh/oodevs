// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ResourceNetworks_GUI_h_
#define __ADN_ResourceNetworks_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_ComboBox_Vector.h"

class ADN_ResourceNetworks_Data;

// =============================================================================
/** @class  ADN_ResourceNetworks_GUI
    @brief  ADN_ResourceNetworks_GUI
*/
// Created: JSR 2010-09-13
// =============================================================================
class ADN_ResourceNetworks_GUI : public ADN_GUI_ABC
{
    Q_OBJECT
public:
    enum E_GuiElements
    {
        eName,
        eCategory,
        eProduction,
        eColor,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ResourceNetworks_GUI( ADN_ResourceNetworks_Data& data );
    virtual ~ADN_ResourceNetworks_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_ResourceNetworks_Data& data_;
    //@}
};

#endif // __ADN_ResourceNetworks_GUI_h_
