// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_HumanFactors_GUI.h $
// $Author: Ape $
// $Modtime: 21/03/05 12:09 $
// $Revision: 1 $
// $Workfile: ADN_HumanFactors_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_HumanFactors_GUI_h_
#define __ADN_HumanFactors_GUI_h_

#include "ADN_GUI_ABC.h"

#include "ADN_HumanFactors_Data.h"


// =============================================================================
/** @class  ADN_HumanFactors_GUI
    @brief  ADN_HumanFactors_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_HumanFactors_GUI : public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_HumanFactors_GUI )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_HumanFactors_GUI( ADN_HumanFactors_Data& data );
    ~ADN_HumanFactors_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Helpers
    //@{
    QWidget* BuildModifiers( QWidget* pParent, ADN_HumanFactors_Data::ModifiersInfo& modifiers, const char* szName );
    //@}

private:
    //! @name Member data
    //@{
    ADN_HumanFactors_Data& data_;
    //@}
};


#endif // __ADN_HumanFactors_GUI_h_
