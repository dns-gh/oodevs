// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Logistic_GUI_h_
#define __ADN_Logistic_GUI_h_

#include "ADN_Tab_Container.h"

class ADN_Logistic_Data;

// =============================================================================
/** @class  ADN_Logistic_GUI
    @brief  ADN_Logistic_GUI
*/
// Created: ABR 2012-01-18
// =============================================================================
class ADN_Logistic_GUI : public ADN_Tab_Container
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Logistic_GUI( ADN_Logistic_Data& data );
    virtual ~ADN_Logistic_GUI();
    //@}
};

#endif // __ADN_Logistic_GUI_h_
