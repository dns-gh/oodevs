// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Launcher_Wizard.h $
// $Author: Ape $
// $Modtime: 28/01/05 15:29 $
// $Revision: 5 $
// $Workfile: ADN_Launcher_Wizard.h $
//
// *****************************************************************************

#ifndef __ADN_Launcher_Wizard_h_
#define __ADN_Launcher_Wizard_h_

#include "ADN_Launchers_Data.h"
#include "ADN_Wizard_ABC.h"


// =============================================================================
/** @class  ADN_Launcher_Wizard
    @brief  ADN_Launcher_Wizard
    @par    Using example
    @code
    ADN_Launcher_Wizard;
    @endcode
*/
// Created: AGN 2003-11-03
// =============================================================================
class ADN_Launcher_Wizard
: public ADN_Wizard_ABC<ADN_Launchers_Data::LauncherInfos>
{
    MT_COPYNOTALLOWED( ADN_Launcher_Wizard )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Launcher_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Launcher_Wizard();
    //@}
};


#endif // __ADN_Launcher_Wizard_h_
