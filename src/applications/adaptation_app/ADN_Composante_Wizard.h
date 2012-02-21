// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Composante_Wizard_h_
#define __ADN_Composante_Wizard_h_

#include "ADN_Composantes_Data.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC<ADN_Composantes_Data::ComposanteInfos> ADN_Wizard_ABC_ADN_Composantes_Data_ComposanteInfos_;

// =============================================================================
/** @class  ADN_Composante_Wizard
    @brief  ADN_Composante_Wizard
    @par    Using example
    @code
    ADN_Composante_Wizard;
    @endcode
*/
// Created: AGN 2003-11-03
// =============================================================================
class ADN_Composante_Wizard : public ADN_Wizard_ABC_ADN_Composantes_Data_ComposanteInfos_
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Composante_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Composante_Wizard();
    //@}
};

#endif // __ADN_Composante_Wizard_h_
