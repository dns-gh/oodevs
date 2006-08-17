// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_NBC_GUI.h $
// $Author: Ape $
// $Modtime: 25/03/05 16:38 $
// $Revision: 6 $
// $Workfile: ADN_NBC_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_NBC_GUI_h_
#define __ADN_NBC_GUI_h_

#include "ADN_GUI_ABC.h"

class QWidget;
class ADN_NBC_Datas;
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
    MT_COPYNOTALLOWED( ADN_NBC_GUI )

public:
    enum E_GuiElements
    {
        eName,
        eNbrOk,
        eNbrHurt1,
        eNbrHurt2,
        eNbrHurt3,
        eNbrHurt4,
        eNbrDead,
        eLifetime,
        eSpreadAngle,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
     ADN_NBC_GUI( ADN_NBC_Datas& data );
    ~ADN_NBC_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    ADN_NBC_Datas& data_;
};


#endif // __ADN_NBC_GUI_h_
