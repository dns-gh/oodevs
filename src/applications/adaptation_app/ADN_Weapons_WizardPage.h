// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Weapons_WizardPage_h_
#define __ADN_Weapons_WizardPage_h_

#include "ADN_WizardPage.h"
#include "ADN_Weapons_Data.h"

// =============================================================================
/** @class  ADN_Weapons_WizardPage
    @brief  ADN_Weapons_WizardPage
*/
// Created: ABR 2012-07-31
// =============================================================================
class ADN_Weapons_WizardPage : public ADN_WizardPage< ADN_Weapons_Data::WeaponInfos >
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Weapons_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* pParent = 0 );
    virtual ~ADN_Weapons_WizardPage();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual QWidget* CreateNameField();
    virtual std::string GetName();
    virtual bool validatePage();
    virtual void ApplyOptions();
    //@}

private:
    //! @name Member data
    //@{
    QComboBox* pComboLauncher_;
    QComboBox* pComboAmmo_;
    //@}
};
#endif // __ADN_Weapons_WizardPage_h_
