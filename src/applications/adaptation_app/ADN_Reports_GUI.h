// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Reports_GUI_h_
#define __ADN_Reports_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Reports_Data;

// =============================================================================
/** @class  ADN_Reports_GUI
    @brief  ADN_Reports_GUI
*/
// Created: SBO 2006-12-14
// =============================================================================
class ADN_Reports_GUI : public ADN_GUI_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Reports_GUI( ADN_Reports_Data& data );
    virtual ~ADN_Reports_GUI();
    //@}

    //! @name Operations
    //@{
    virtual void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Reports_Data& data_;
    //@}
};

#endif // __ADN_Reports_GUI_h_
