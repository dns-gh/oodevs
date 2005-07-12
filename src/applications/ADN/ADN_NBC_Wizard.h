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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_NBC_Wizard.h $
// $Author: Ape $
// $Modtime: 14/02/05 14:55 $
// $Revision: 1 $
// $Workfile: ADN_NBC_Wizard.h $
//
// *****************************************************************************

#ifndef __ADN_NBC_Wizard_h_
#define __ADN_NBC_Wizard_h_

#include "ADN_NBC_Datas.h"
#include "ADN_Wizard_ABC.h"


// =============================================================================
/** @class  ADN_NBC_Wizard
    @brief  ADN_NBC_Wizard
*/
// Created: APE 2005-02-14
// =============================================================================
class ADN_NBC_Wizard
    : public ADN_Wizard_ABC<ADN_NBC_Datas::NbcAgentInfos>
{
    MT_COPYNOTALLOWED( ADN_NBC_Wizard )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_NBC_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_NBC_Wizard();
    //@}
};


#endif // __ADN_NBC_Wizard_h_
