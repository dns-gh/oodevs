// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Health_GUI_h_
#define __ADN_Health_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_Enums.h"

class ADN_Health_Data;
class ADN_Table;

// =============================================================================
/** @class  ADN_Health_GUI
    @brief  ADN_Health_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Health_GUI : public ADN_GUI_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Health_GUI( ADN_Health_Data& data );
    virtual ~ADN_Health_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Helpers
    //@{
    ADN_Table* BuildWoundsTable();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Health_Data& data_;
    //@}
};


#endif // __ADN_Health_GUI_h_
