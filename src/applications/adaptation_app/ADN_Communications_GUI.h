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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Communications_GUI.h $
// $Author: Ape $
// $Modtime: 21/03/05 11:08 $
// $Revision: 1 $
// $Workfile: ADN_Communications_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_Communications_GUI_h_
#define __ADN_Communications_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Communications_Data;


// =============================================================================
/** @class  ADN_Communications_GUI
    @brief  ADN_Communications_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Communications_GUI : public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_Communications_GUI )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Communications_GUI( ADN_Communications_Data& data );
    ~ADN_Communications_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Communications_Data& data_;
    //@}
};


#endif // __ADN_Communications_GUI_h_
