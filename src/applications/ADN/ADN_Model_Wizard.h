// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-12-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Model_Wizard.h $
// $Author: Ape $
// $Modtime: 14/02/05 17:15 $
// $Revision: 5 $
// $Workfile: ADN_Model_Wizard.h $
//
// *****************************************************************************

#ifndef __ADN_Model_Wizard_h_
#define __ADN_Model_Wizard_h_

#include "ADN_Models_Data.h"
#include "ADN_Wizard_ABC.h"


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
class ADN_Model_Wizard
: public ADN_Wizard_ABC<ADN_Models_Data::ModelInfos>
{
    MT_COPYNOTALLOWED( ADN_Model_Wizard )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Model_Wizard( ADN_Models_Data::ModelInfos::E_ModelEntityType eEntityType, QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Model_Wizard();
    //@}
};


#endif // __ADN_Model_Wizard_h_
