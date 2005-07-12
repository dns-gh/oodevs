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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_HLA_GUI.h $
// $Author: Ape $
// $Modtime: 22/03/05 11:06 $
// $Revision: 1 $
// $Workfile: ADN_HLA_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_HLA_GUI_h_
#define __ADN_HLA_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_HLA_Data;


// =============================================================================
/** @class  ADN_HLA_GUI
    @brief  ADN_HLA_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_HLA_GUI : public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_HLA_GUI )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_HLA_GUI( ADN_HLA_Data& data );
    ~ADN_HLA_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_HLA_Data& data_;
    //@}
};

#endif // __ADN_HLA_GUI_h_
