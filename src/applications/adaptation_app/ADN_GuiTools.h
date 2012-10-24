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
    static bool MultiRefWarning( ADN_Ref_ABC* data = 0 );
    static bool DeletionWarning();
    static bool MissingConvoyWarning();
    static bool MissingConvoyMissionWarning();
    static bool MissingBreakdownWarning( const std::string& name );
    static bool MissingDecisionalModel( const std::string& unit );
    static bool MissingGeometry( const std::string& name );
    static bool MissingPCOnAutomat( const std::string& name );
    static bool BadAutomatComposition( const std::string& name );
    static bool MissingParameterChoices( const std::string& name );
    static bool MissingArmor();
};

#endif // __ADN_GuiTools_h_
