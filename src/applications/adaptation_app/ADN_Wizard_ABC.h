// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Wizard_ABC_h_
#define __ADN_Wizard_ABC_h_

#include <Qt3Support/q3wizard.h>
#include "ADN_ObjectCreator_ABC.h"
#include "ADN_Wizard_FirstPage_ABC.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_Wizard_ABC
    @brief  ADN_Wizard_ABC
*/
// Created: AGN 2003-11-18
// =============================================================================
template< typename T >
class ADN_Wizard_ABC : public Q3Wizard
                     , public ADN_ObjectCreator_ABC
                     , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Wizard_ABC( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Wizard_ABC();
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
