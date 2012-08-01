// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Equipement_Wizard_h_
#define __ADN_Equipement_Wizard_h_

#include "ADN_Wizard.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Equipement_WizardPage.h"

// =============================================================================
/** @class  ADN_Equipement_Wizard
    @brief  ADN_Equipement_Wizard
    ADN_Equipement_Wizard;
    @endcode
*/
// Created: APE 2005-01-19
// =============================================================================
class ADN_Equipement_Wizard : public ADN_Wizard< ADN_Equipement_Data::CategoryInfo, ADN_Equipement_WizardPage >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Equipement_Wizard( ADN_Equipement_Data::ResourceInfos& parentDotation, const QString& elementName, QWidget* pParent = 0 )
        : ADN_Wizard< ADN_Equipement_Data::CategoryInfo, ADN_Equipement_WizardPage >( elementName, parentDotation.categories_, pParent )
        , parentDotation_( parentDotation )
    {
        // NOTHING
    }
    virtual ~ADN_Equipement_Wizard()
    {
        // NOTHING
    }
    //@}

public:
    //! @name Helpers
    //@{
    virtual ADN_Equipement_WizardPage* CreatePage()
    {
        return new ADN_Equipement_WizardPage( vector_, title_, this, &parentDotation_ );
    }
    //@}

private:
    //! @name Member data
    //@{
    ADN_Equipement_Data::ResourceInfos& parentDotation_;
    //@}
};

#endif // __ADN_Equipement_Wizard_h_
