// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_OpenMode_Dialog.cpp $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 4 $
// $Workfile: ADN_OpenMode_Dialog.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_OpenMode_Dialog.h"

#include "ADN_Enums.h"

// -----------------------------------------------------------------------------
// Name: ADN_OpenMode_Dialog constructor
/** @param  pParent
*/
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
ADN_OpenMode_Dialog::ADN_OpenMode_Dialog( QWidget* pParent )
: QDialog( pParent, 0, true, Qt::WType_Dialog | Qt::WStyle_Customize | Qt::WStyle_NoBorder | Qt::WStyle_Title | Qt::WType_Dialog )
, pPassword_( 0 )
, pNormalMode_( 0 )
, pAdminMode_( 0 )
{
    setCaption( qApp->translate("ADN_OpenMode_Dialog","Open mode configuration") );

    Q3VBoxLayout* pMainLayout = new Q3VBoxLayout( this );
    pMainLayout->setSpacing( 10 );
    pMainLayout->setMargin( 10 );

    // Mode selection
    Q3ButtonGroup* pModeGroup = new Q3ButtonGroup( 2, Qt::Vertical, this );
    pModeGroup->setRadioButtonExclusive( true );

    pNormalMode_ = new QRadioButton( qApp->translate("ADN_OpenMode_Dialog","Normal mode"), pModeGroup );
    pNormalMode_->setChecked( true );

    pAdminMode_ = new QRadioButton( qApp->translate("ADN_OpenMode_Dialog","Admin mode"), pModeGroup );

    pMainLayout->addWidget( pModeGroup );

    // Password edition
    Q3Grid* pPasswordGrid = new Q3Grid( 2, this );
    pPasswordGrid->setSpacing( 10 );

    new QLabel( qApp->translate("ADN_OpenMode_Dialog","Password:"), pPasswordGrid );
    pPassword_ = new QLineEdit( pPasswordGrid );
    pPassword_->setEchoMode( QLineEdit::Password );
    pPassword_->setEnabled( false );

    pMainLayout->addWidget( pPasswordGrid );

    // connect the password line edit to the admin button
    connect( pAdminMode_, SIGNAL( toggled( bool ) ), pPassword_, SLOT( setEnabled( bool ) ) );

    // add a ok/cancel button group
    Q3HBox* pButtonLayout = new Q3HBox( this );
    pButtonLayout->setSpacing( 10 );

    QPushButton* pButton = new QPushButton( qApp->translate("ADN_OpenMode_Dialog","Ok"), pButtonLayout );
    connect( pButton, SIGNAL( clicked() ), this, SLOT( accept() ) );

    pButton = new QPushButton( qApp->translate("ADN_OpenMode_Dialog","Cancel"), pButtonLayout );
    connect( pButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    pMainLayout->addWidget( pButtonLayout );

    resize( QSize( 225, sizeHint().height() ) );
    setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_OpenMode_Dialog destructor
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
ADN_OpenMode_Dialog::~ADN_OpenMode_Dialog()
{

}

// -----------------------------------------------------------------------------
// Name: ADN_OpenMode_Dialog::GetOpenMode
/** @return
*/
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
E_OpenMode ADN_OpenMode_Dialog::GetOpenMode() const
{
    assert( pNormalMode_ != 0 );
    assert( pAdminMode_ != 0 );

    if( pNormalMode_->isOn() )
        return eOpenMode_Normal;
    else
    {
        assert( pAdminMode_->isOn() );
        return eOpenMode_Admin;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_OpenMode_Dialog::GetPassword
/** @return
*/
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
QString ADN_OpenMode_Dialog::GetPassword() const
{
    assert( pPassword_ != 0 );
    return pPassword_->text();
}
