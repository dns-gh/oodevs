// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-14 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Wizard_FirstPage_Default.h $
// $Author: Ape $
// $Modtime: 11/04/05 10:57 $
// $Revision: 3 $
// $Workfile: ADN_Wizard_FirstPage_Default.h $
//
// *****************************************************************************

#ifndef __ADN_Wizard_FirstPage_Default_h_
#define __ADN_Wizard_FirstPage_Default_h_

#include <QtGui/qlabel.h>
#include <Qt3Support/q3buttongroup.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qlineedit.h>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3wizard.h>
#include <QtGui/qmessagebox.h>
#include "ADN_Wizard_FirstPage_ABC.h"

// =============================================================================
/** @class  ADN_Wizard_FirstPage_Default
    @brief  ADN_Wizard_FirstPage_Default
*/
// Created: APE 2005-02-14
// =============================================================================
template< typename T >
class ADN_Wizard_FirstPage_Default : public ADN_Wizard_FirstPage_ABC< T >
{

public:
    typedef std::vector< T* >                     T_ItemVector;
    typedef typename T_ItemVector::iterator       IT_ItemVector;
    typedef typename T_ItemVector::const_iterator CIT_ItemVector;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Wizard_FirstPage_Default( T_ItemVector& existingItemsList, Q3Wizard* pParent, const char* szName = 0, const char* szImageName = "adn_wizard.bmp" );
    virtual ~ADN_Wizard_FirstPage_Default();
    //@}

    //! @name Operations
    //@{
    virtual T* CreateObject();
    void SetCaptions( const char* szPageName, const char* szColumnHeader );
    //@}

protected:
    //! @name Helpers
    //@{
    void Initialize();
    virtual T* NewT();
    //@}

    void showEvent( QShowEvent* pEvent );

protected:
    //! @name Member data
    //@{
    T_ItemVector& existingItemsList_;

    QLabel*    pNameLabel_;
    QLineEdit* pNameEdit_;
    Q3ListView* pExistingItemsListView_;
    QRadioButton* pNewRadioButton_;
    QRadioButton* pCopyRadioButton_;
    //@}
};

#include "ADN_Wizard_FirstPage_Default.inl"

#endif // __ADN_Wizard_FirstPage_Default_h_
