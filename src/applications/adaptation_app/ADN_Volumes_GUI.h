// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Volumes_GUI_h_
#define __ADN_Volumes_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Volumes_Data;

// =============================================================================
/** @class  ADN_Volumes_GUI
    @brief  ADN_Volumes_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Volumes_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    enum E_SizeGuiElements
    {
        eSizeName,
        eNbrSizeGuiElements
    };
    //@}

    //! @name Constructors/Destructor
    //@{
    explicit ADN_Volumes_GUI( ADN_Volumes_Data& data );
    virtual ~ADN_Volumes_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Volumes_Data& data_;
    //@}
};

#endif // __ADN_Volumes_GUI_h_
