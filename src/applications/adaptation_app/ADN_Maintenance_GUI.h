// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Maintenance_GUI_h_
#define __ADN_Maintenance_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Maintenance_Data;

// =============================================================================
/** @class  ADN_Maintenance_GUI
    @brief  ADN_Maintenance_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Maintenance_GUI : public ADN_GUI_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Maintenance_GUI( ADN_Maintenance_Data& data );
    virtual ~ADN_Maintenance_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Helpers
    //@{
    Q3GroupBox* BuildWorkingSchemeTable();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Maintenance_Data& data_;
    //@}
};


#endif // __ADN_Maintenance_GUI_h_
