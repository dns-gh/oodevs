// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Models_Wizard_h_
#define __ADN_Models_Wizard_h_

#include "ADN_Wizard.h"
#include "ADN_Models_Data.h"
#include "ADN_Models_WizardPage.h"

// =============================================================================
/** @class  ADN_Models_Wizard
    @brief  ADN_Models_Wizard
*/
// Created: APE 2005-01-19
// =============================================================================
class ADN_Models_Wizard : public ADN_Wizard< ADN_Models_ModelInfos, ADN_Models_WizardPage >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Models_Wizard( E_EntityType entityType, const QString& elementName, ADN_Type_Vector_ABC< ADN_Models_ModelInfos >& vector, QWidget* pParent = 0 )
        : ADN_Wizard< ADN_Models_ModelInfos, ADN_Models_WizardPage >( elementName, vector, pParent )
        , entityType_( entityType )
    {
        // NOTHING
    }
    //@}

public:
    //! @name Helpers
    //@{
    virtual ADN_Models_WizardPage* CreatePage()
    {
        return new ADN_Models_WizardPage( vector_, title_, this, entityType_ );
    }
    //@}

private:
    //! @name Member data
    //@{
    E_EntityType entityType_;
    //@}
};

#endif // __ADN_Models_Wizard_h_
