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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Wizard_ABC.h $
// $Author: Ape $
// $Modtime: 21/03/05 16:41 $
// $Revision: 5 $
// $Workfile: ADN_Wizard_ABC.h $
//
// *****************************************************************************

#ifndef __ADN_Wizard_ABC_h_
#define __ADN_Wizard_ABC_h_

#include <qwizard.h>
#include "ADN_ObjectCreator_ABC.h"
#include "ADN_Wizard_FirstPage_ABC.h"


// =============================================================================
/** @class  ADN_Wizard_ABC
    @brief  ADN_Wizard_ABC
*/
// Created: AGN 2003-11-18
// =============================================================================
template< typename T >
class ADN_Wizard_ABC
: public QWizard, public ADN_ObjectCreator_ABC
{
    MT_COPYNOTALLOWED( ADN_Wizard_ABC )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Wizard_ABC( QWidget* pParent = 0, const char* szName = 0 );
    ~ADN_Wizard_ABC();
    //@}

    //! @name Operations
    //@{
    virtual ADN_Ref_ABC* CreateObject();

    virtual void showPage( QWidget* pPage );
    //@}

protected:
    //! @name Data validity checks
    //@{
    virtual void accept();

    virtual bool ValidatePage( int nPageIndex );
    virtual bool ValidateAll ();
    //@}

protected:
    T* pResult_;
    ADN_Wizard_FirstPage_ABC<T>* pFirstPage_;
};

#include "ADN_Wizard_ABC.inl"

#endif // __ADN_Wizard_ABC_h_
