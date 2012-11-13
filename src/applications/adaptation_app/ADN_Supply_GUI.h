// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Supply_GUI( ADN_Supply_Data& data );
    virtual ~ADN_Supply_GUI();
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
