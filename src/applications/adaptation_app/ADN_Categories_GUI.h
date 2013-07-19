// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Categories_GUI_h_
#define __ADN_Categories_GUI_h_

#include "ADN_Grid_Container.h"

class ADN_Categories_Data;

// =============================================================================
/** @class  ADN_Categories_GUI
    @brief  ADN_Categories_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Categories_GUI : public ADN_Grid_Container
{
    Q_OBJECT

public:
    //! @name  Constructors/Destructor
    //@{
             ADN_Categories_GUI( ADN_Categories_Data& data );
    virtual ~ADN_Categories_GUI();
    //@}
};

#endif // __ADN_Categories_GUI_h_
