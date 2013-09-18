// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_NBC_GUI_h_
#define __ADN_NBC_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_NBC_Data;
class ADN_ListView;
class ADN_Connector_ABC;

// =============================================================================
/** @class  ADN_NBC_GUI
    @brief  ADN_NBC_GUI
*/
// Created: AGN 2004-05-06
// =============================================================================
class ADN_NBC_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    enum E_GuiElements
    {
        eName,
        eLiquidGroup,
        eLiquidGroupPresent,
        eGazGroupPresent,
        eGazGroup,
        eGazLifetime,
        eGazSpreadAngle,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_NBC_GUI( ADN_NBC_Data& data );
    virtual ~ADN_NBC_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    ADN_NBC_Data& data_;
};

#endif // __ADN_NBC_GUI_h_
