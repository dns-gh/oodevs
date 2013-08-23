// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Missions_Wizard_h_
#define __ADN_Missions_Wizard_h_

#include "ADN_Wizard.h"
#include "ADN_Missions_Data.h"
#include "ADN_Missions_WizardPage.h"

// =============================================================================
/** @class  ADN_Missions_Wizard
    @brief  ADN_Missions_Wizard
*/
// Created: APE 2005-01-19
// =============================================================================
class ADN_Missions_Wizard : public ADN_Wizard< ADN_Missions_ABC, ADN_Missions_WizardPage >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Missions_Wizard( E_MissionType entityType, const QString& elementName, ADN_Missions_Data::T_Mission_Vector& vector, QWidget* pParent = 0 )
        : ADN_Wizard< ADN_Missions_ABC, ADN_Missions_WizardPage >( elementName, vector, pParent )
        , entityType_( entityType )
    {
        // NOTHING
    }
    virtual ~ADN_Missions_Wizard()
    {
        // NOTHING
    }
    //@}

public:
    //! @name Helpers
    //@{
    virtual ADN_Missions_WizardPage* CreatePage()
    {
        return new ADN_Missions_WizardPage( vector_, title_, this, entityType_ );
    }
    //@}

private:
    //! @name Member data
    //@{
    E_MissionType entityType_;
    //@}
};

#endif // __ADN_Missions_Wizard_h_
