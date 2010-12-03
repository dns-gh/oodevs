// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_People_GUI_h_
#define __ADN_People_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_People_Data;
class ADN_Table;

// =============================================================================
/** @class  ADN_People_GUI
    @brief  ADN_People_GUI
*/
// Created: SLG 2010-11-23
// =============================================================================
class ADN_People_GUI : public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_People_GUI )

public:
    enum E_GuiElements
    {
        eModel,
        eMale,
        eFemale,
        eChildren,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
             ADN_People_GUI( ADN_People_Data& data );
    virtual ~ADN_People_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_People_Data& data_;
    //@}
};


#endif // __ADN_People_GUI_h_
