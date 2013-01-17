// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Resources_Wizard_h_
#define __ADN_Resources_Wizard_h_

#include "ADN_Wizard.h"
#include "ADN_Resources_Data.h"
#include "ADN_Resources_WizardPage.h"

// =============================================================================
/** @class  ADN_Resources_Wizard
    @brief  ADN_Resources_Wizard
    ADN_Resources_Wizard;
    @endcode
*/
// Created: APE 2005-01-19
// =============================================================================
class ADN_Resources_Wizard : public ADN_Wizard< ADN_Resources_Data::CategoryInfo, ADN_Resources_WizardPage >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Resources_Wizard( ADN_Resources_Data::ResourceInfos& parentDotation, ADN_Resources_Data::T_ResourceInfos_Vector& allResources, const QString& elementName, QWidget* pParent = 0 )
        : ADN_Wizard< ADN_Resources_Data::CategoryInfo, ADN_Resources_WizardPage >( elementName, parentDotation.categories_, pParent )
        , parentDotation_( parentDotation )
        , allResources_( allResources )
    {
        // NOTHING
    }
    virtual ~ADN_Resources_Wizard()
    {
        // NOTHING
    }
    //@}

public:
    //! @name Helpers
    //@{
    virtual ADN_Resources_WizardPage* CreatePage()
    {
        return new ADN_Resources_WizardPage( vector_, title_, this, &parentDotation_, &allResources_ );
    }
    //@}

private:
    //! @name Member data
    //@{
    ADN_Resources_Data::ResourceInfos& parentDotation_;
    ADN_Resources_Data::T_ResourceInfos_Vector& allResources_;
    //@}
};

#endif // __ADN_Resources_Wizard_h_
