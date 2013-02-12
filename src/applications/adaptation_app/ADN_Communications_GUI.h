// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Communications_GUI( ADN_Communications_Data& data );
    virtual ~ADN_Communications_GUI();
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
