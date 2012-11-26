// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Resources_WizardPage_h_
#define __ADN_Resources_WizardPage_h_

#include "ADN_WizardPage.h"
#include "ADN_Resources_Data.h"

// =============================================================================
/** @class  ADN_Resources_WizardPage
    @brief  ADN_Resources_WizardPage
*/
// Created: ABR 2012-07-31
// =============================================================================
class ADN_Resources_WizardPage : public ADN_WizardPage< ADN_Resources_Data::CategoryInfo >
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Resources_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* pParent = 0, ADN_Resources_Data::ResourceInfos* parentDotation = 0 )
                 : ADN_WizardPage< ADN_Resources_Data::CategoryInfo >( existingItems, pageTitle, pParent )
                 , parentResource_( *parentDotation )
             {
                 // NOTHING
             }
    virtual ~ADN_Resources_WizardPage()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual ADN_Resources_Data::CategoryInfo* NewT()
    {
        if( parentResource_.nType_ == eDotationFamily_Munition ||
            parentResource_.nType_ == eDotationFamily_Mine ||
            parentResource_.nType_ == eDotationFamily_Explosif )
            return new ADN_Resources_Data::AmmoCategoryInfo( parentResource_ );
        else
            return new ADN_Resources_Data::CategoryInfo( parentResource_ );
    }
    //@}

private:
    //! @name Member data
    //@{
    ADN_Resources_Data::ResourceInfos& parentResource_;
    //@}
};

#endif // __ADN_Resources_WizardPage_h_
