// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_WizardPage_ABC.h $
// $Author: Ape $
// $Modtime: 11/01/05 11:23 $
// $Revision: 2 $
// $Workfile: ADN_WizardPage_ABC.h $
//
// *****************************************************************************

#ifndef __ADN_WizardPage_ABC_h_
#define __ADN_WizardPage_ABC_h_

#include <qvbox.h>

class QWizard;
class ADN_ImageViewer;


// =============================================================================
/** @class  ADN_WizardPage_ABC
    @brief  ADN_WizardPage_ABC
    @par    Using example
    @code
    ADN_WizardPage_ABC;
    @endcode
*/
// Created: AGN 2003-11-18
// =============================================================================
class ADN_WizardPage_ABC
: public QHBox
{
    MT_COPYNOTALLOWED( ADN_WizardPage_ABC )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_WizardPage_ABC( QWizard* pParent, const char* szName = 0, const char* szImageName = "adn_wizard.bmp" );
    virtual ~ADN_WizardPage_ABC();
    //@}

    //! @name Operations
    //@{
    //@}

    //! @name Accessors
    //@{
    //@}

    //! @name Modifiers
    //@{
    //@}

    //! @name Operators
    //@{
    //@}

private:
    //! @name Helpers
    //@{
    //@}

protected:
    //! @name Member data
    //@{
    QWizard* pWizard_;
    ADN_ImageViewer* pImage_;
    QVBox* pRightSide_;
    //@}
};


#endif // __ADN_WizardPage_ABC_h_
