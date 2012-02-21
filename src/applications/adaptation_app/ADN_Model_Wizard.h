// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Model_Wizard_h_
#define __ADN_Model_Wizard_h_

#include "ADN_Models_Data.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC<ADN_Models_Data::ModelInfos>  ADN_Wizard_ABC_ADN_Models_Data_ModelInfos_;

// =============================================================================
/** @class  ADN_Model_Wizard
    @brief  ADN_Model_Wizard
    @par    Using example
    @code
    ADN_Model_Wizard;
    @endcode
*/
// Created: AGN 2003-12-03
// =============================================================================
class ADN_Model_Wizard : public ADN_Wizard_ABC_ADN_Models_Data_ModelInfos_
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Model_Wizard( ADN_Models_Data::ModelInfos::E_ModelEntityType eEntityType, QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Model_Wizard();
    //@}
};

#endif // __ADN_Model_Wizard_h_
