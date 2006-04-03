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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Maintenance_GUI.h $
// $Author: Ape $
// $Modtime: 21/03/05 12:18 $
// $Revision: 1 $
// $Workfile: ADN_Maintenance_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_Maintenance_GUI_h_
#define __ADN_Maintenance_GUI_h_

#include "ADN_GUI_ABC.h"

#include "ADN_Maintenance_Data.h"


// =============================================================================
/** @class  ADN_Maintenance_GUI
    @brief  ADN_Maintenance_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Maintenance_GUI : public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_Maintenance_GUI )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Maintenance_GUI( ADN_Maintenance_Data& data );
    ~ADN_Maintenance_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Maintenance_Data& data_;
    //@}
};


#endif // __ADN_Maintenance_GUI_h_
