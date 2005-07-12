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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Wizard_ABC.inl $
// $Author: Ape $
// $Modtime: 21/03/05 16:32 $
// $Revision: 3 $
// $Workfile: ADN_Wizard_ABC.inl $
//
// *****************************************************************************

#include <qapplication.h>
#include <qpushbutton.h>

#include "ADN_WizardPage_ABC.h"


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_ABC constructor
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
template< typename T >
ADN_Wizard_ABC<T>::ADN_Wizard_ABC( QWidget* pParent, const char* szName )
: QWizard              ( pParent, szName )
, ADN_ObjectCreator_ABC()
, pResult_             ( 0 )
, pFirstPage_          ( 0 )
{
    setFixedSize( 600, 400 );
    nextButton()->setText( tr( "Next" ) );
    backButton()->setText( tr( "Previous" ) );
    cancelButton()->setText( tr( "Cancel" ) );
    finishButton()->setText( tr( "Done" ) );
    setTitleFont( QFont( QApplication::font().family(), 12, QFont::Bold ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_ABC destructor
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
template< typename T >
ADN_Wizard_ABC<T>::~ADN_Wizard_ABC()
{
    delete pResult_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_ABC::CreateObject
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
template< typename T >
ADN_Ref_ABC* ADN_Wizard_ABC<T>::CreateObject()
{
    // Get the wizard to create the new object.
    int nResult = this->exec();
    if( nResult == QDialog::Rejected )
        return 0;

    T* pResult = pResult_;
    pResult_ = 0;
    return pResult;  
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_ABC::showPage
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Wizard_ABC<T>::showPage( QWidget* pPage )
{
    if( currentPage() != 0 && currentPage() != pPage )
    {
        // Check if the page we are supposed to leave is valid.
        if( ! ValidatePage( this->indexOf( currentPage() ) ) )
            return;
    }

    QWizard::showPage( pPage );
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_ABC::accept
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Wizard_ABC<T>::accept()
{
    if( ValidateAll() )
        QDialog::accept();
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_ABC::ValidatePage
// Created: APE 2005-01-28
// -----------------------------------------------------------------------------
template< typename T >
bool ADN_Wizard_ABC<T>::ValidatePage( int nPageIndex )
{
    if( nPageIndex != 0 )
        return true;

    if( pResult_ == 0 )
    {
        assert( pFirstPage_ != 0 );
        pResult_ = pFirstPage_->CreateObject();
        if( pResult_ == 0 )
            return false;
    }
    return true;
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_ABC::ValidateAll
// Created: APE 2005-01-28
// -----------------------------------------------------------------------------
template< typename T >
bool ADN_Wizard_ABC<T>::ValidateAll()
{
    return pResult_ != 0;
}
