// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipement_WizardPage_h_
#define __ADN_Equipement_WizardPage_h_

#include "ADN_WizardPage.h"
#include "ADN_Equipement_Data.h"

// =============================================================================
/** @class  ADN_Equipement_WizardPage
    @brief  ADN_Equipement_WizardPage
*/
// Created: ABR 2012-07-31
// =============================================================================
class ADN_Equipement_WizardPage : public ADN_WizardPage< ADN_Equipement_Data::CategoryInfo >
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Equipement_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* pParent = 0, ADN_Equipement_Data::ResourceInfos* parentDotation = 0 )
                 : ADN_WizardPage< ADN_Equipement_Data::CategoryInfo >( existingItems, pageTitle, pParent )
                 , parentDotation_( *parentDotation )
             {
                 // NOTHING
             }
    virtual ~ADN_Equipement_WizardPage()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual ADN_Equipement_Data::CategoryInfo* NewT()
    {
        if( parentDotation_.nType_ == eDotationFamily_Munition ||
            parentDotation_.nType_ == eDotationFamily_Mine ||
            parentDotation_.nType_ == eDotationFamily_Explosif )
            return new ADN_Equipement_Data::AmmoCategoryInfo( parentDotation_ );
        else
            return new ADN_Equipement_Data::CategoryInfo( parentDotation_ );
    }
    //@}

private:
    //! @name Member data
    //@{
    ADN_Equipement_Data::ResourceInfos& parentDotation_;
    //@}
};

#endif // __ADN_Equipement_WizardPage_h_
