// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
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
class ADN_AiEngine_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_AiEngine_GUI( ADN_AiEngine_Data& data );
    virtual ~ADN_AiEngine_GUI();
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
