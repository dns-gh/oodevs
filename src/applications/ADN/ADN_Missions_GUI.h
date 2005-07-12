// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Missions_GUI.h $
// $Author: Ape $
// $Modtime: 21/03/05 14:53 $
// $Revision: 1 $
// $Workfile: ADN_Missions_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_Missions_GUI_h_
#define __ADN_Missions_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Missions_Data;


// =============================================================================
/** @class  ADN_Missions_GUI
    @brief  ADN_Missions_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Missions_GUI : public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_Missions_GUI )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Missions_GUI( ADN_Missions_Data& data );
    ~ADN_Missions_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Missions_Data& data_;
    //@}
};


#endif // __ADN_Missions_GUI_h_
