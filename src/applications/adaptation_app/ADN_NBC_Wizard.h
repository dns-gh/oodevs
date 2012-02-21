// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_NBC_Wizard_h_
#define __ADN_NBC_Wizard_h_

#include "ADN_NBC_Datas.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC<ADN_NBC_Datas::NbcAgentInfos> ADN_Wizard_ABC_ADN_NBC_Datas_NbcAgentInfos_;

// =============================================================================
/** @class  ADN_NBC_Wizard
    @brief  ADN_NBC_Wizard
*/
// Created: APE 2005-02-14
// =============================================================================
class ADN_NBC_Wizard : public ADN_Wizard_ABC_ADN_NBC_Datas_NbcAgentInfos_
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_NBC_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_NBC_Wizard();
    //@}
};

#endif // __ADN_NBC_Wizard_h_
