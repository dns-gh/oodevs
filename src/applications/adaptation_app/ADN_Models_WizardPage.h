// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Models_WizardPage_h_
#define __ADN_Models_WizardPage_h_

#include "ADN_WizardPage.h"
#include "ADN_Models_Data.h"

// =============================================================================
/** @class  ADN_Models_WizardPage
    @brief  ADN_Models_WizardPage
*/
// Created: ABR 2012-07-31
// =============================================================================
class ADN_Models_WizardPage : public ADN_WizardPage< ADN_Models_ModelInfos >
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Models_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* pParent = 0, E_EntityType entityType = eNbrEntityType )
                 : ADN_WizardPage< ADN_Models_ModelInfos >( existingItems, pageTitle, pParent )
                 , entityType_( entityType )
             {
                 // NOTHING
             }
    //@}

    //! @name Operations
    //@{
    virtual ADN_Models_ModelInfos* NewT()
    {
        return new ADN_Models_ModelInfos( entityType_ );
    }
    //@}

private:
    //! @name Member data
    //@{
    E_EntityType entityType_;
    //@}
};

#endif // __ADN_Models_WizardPage_h_
