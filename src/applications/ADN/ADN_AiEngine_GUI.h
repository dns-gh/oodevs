// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-06-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_AiEngine_GUI.h $
// $Author: Ape $
// $Modtime: 4/03/05 11:36 $
// $Revision: 4 $
// $Workfile: ADN_AiEngine_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_AiEngine_GUI_h_
#define __ADN_AiEngine_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_AiEngine_Data;


// =============================================================================
/** @class  ADN_AiEngine_GUI
    @brief  ADN_AiEngine_GUI
*/
// Created: AGN 2004-06-15
// =============================================================================
class ADN_AiEngine_GUI 
: public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_AiEngine_GUI )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_AiEngine_GUI( ADN_AiEngine_Data& data );
    ~ADN_AiEngine_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_AiEngine_Data& data_;
    //@}
};


#endif // __ADN_AiEngine_GUI_h_
