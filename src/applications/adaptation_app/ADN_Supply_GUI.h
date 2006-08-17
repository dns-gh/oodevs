// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Supply_GUI.h $
// $Author: Ape $
// $Modtime: 22/03/05 15:05 $
// $Revision: 1 $
// $Workfile: ADN_Supply_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_Supply_GUI_h_
#define __ADN_Supply_GUI_h_

#include "ADN_GUI_ABC.h"
class ADN_Supply_Data;


// =============================================================================
/** @class  ADN_Supply_GUI
    @brief  ADN_Supply_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Supply_GUI : public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_Supply_GUI )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Supply_GUI( ADN_Supply_Data& data );
    ~ADN_Supply_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Supply_Data& data_;
    //@}
};


#endif // __ADN_Supply_GUI_h_
