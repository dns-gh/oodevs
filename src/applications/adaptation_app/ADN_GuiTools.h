// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-09 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_GuiTools.h $
// $Author: Ape $
// $Modtime: 6/04/05 16:35 $
// $Revision: 2 $
// $Workfile: ADN_GuiTools.h $
//
// *****************************************************************************

#ifndef __ADN_GuiTools_h_
#define __ADN_GuiTools_h_

#include "ADN_Connector_ABC.h"
#include "ADN_ComboBox_Enum.h"

// =============================================================================
/** @class  ADN_GuiTools
    @brief  ADN_GuiTools
*/
// Created: APE 2005-03-09
// =============================================================================
class ADN_GuiTools
{
public:
    static bool MultiRefWarning();
    static bool DeletionWarning();
    static bool MissingConvoyWarning();
    static bool MissingBreakdownWarning( const std::string& name );
};

#endif // __ADN_GuiTools_h_
