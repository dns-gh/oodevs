// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Math�matiques Appliqu�es SA (MASA)
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
    MT_COPYNOTALLOWED( ADN_NBC_Wizard )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_NBC_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_NBC_Wizard();
    //@}
};


#endif // __ADN_NBC_Wizard_h_
