// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Natures_GUI_h_
#define __ADN_Natures_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Natures_Data;

// =============================================================================
/** @class  ADN_Natures_GUI
    @brief  ADN_Natures_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Natures_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    enum E_DotationNatureGuiElements
    {
        eDotationNatureName,
        eNbrDotationNatureGuiElements
    };
    //@}

    //! @name Constructors/Destructor
    //@{
    explicit ADN_Natures_GUI( ADN_Natures_Data& data );
    virtual ~ADN_Natures_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Natures_Data& data_;
    //@}
};

#endif // __ADN_Natures_GUI_h_
