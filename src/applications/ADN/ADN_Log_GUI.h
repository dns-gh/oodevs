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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Log_GUI.h $
// $Author: Ape $
// $Modtime: 21/03/05 12:18 $
// $Revision: 1 $
// $Workfile: ADN_Log_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_Log_GUI_h_
#define __ADN_Log_GUI_h_

#include "ADN_GUI_ABC.h"

#include "ADN_Log_Data.h"


// =============================================================================
/** @class  ADN_Log_GUI
    @brief  ADN_Log_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Log_GUI : public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_Log_GUI )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Log_GUI( ADN_Log_Data& data );
    ~ADN_Log_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Helpers
    //@{
    QWidget* BuildWorkTimeModifiers( QWidget* pParent, ADN_Log_Data::WorkTimeModifiersInfo& modifiers );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Log_Data& data_;
    //@}
};


#endif // __ADN_Log_GUI_h_
