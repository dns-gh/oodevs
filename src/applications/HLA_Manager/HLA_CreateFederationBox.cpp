// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-03-07 $
// $Archive: /MVW_v10/Build/SDK/HLA_Manager/src/HLA_CreateFederationBox.cpp $
// $Author: Age $
// $Modtime: 8/03/05 10:31 $
// $Revision: 3 $
// $Workfile: HLA_CreateFederationBox.cpp $
//
// *****************************************************************************

#include "HLA_Manager_pch.h"
#include "HLA_CreateFederationBox.h"

#include "moc_HLA_CreateFederationBox.cpp"

#include <qpushbutton.h>
#include <qlineedit.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qbuttongroup.h>
#include <qfiledialog.h>

// -----------------------------------------------------------------------------
// Name: HLA_CreateFederationBox constructor
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
HLA_CreateFederationBox::HLA_CreateFederationBox( QWidget* pParent, const std::string& strDefaultName )
    : QDialog( pParent, "Create Federation", true )
    , pNameEdit_( 0 )
    , pFOMEdit_( 0 )
    , pBrowse_( 0 )
{
    setCaption( tr( "Create Federation" ) );
    QGridLayout* pLayout = new QGridLayout( this, 5, 7 );
    pLayout->setMargin( 10 );
    pLayout->setSpacing( 10 );
    pLayout->addColSpacing( 1, 10 );
    pLayout->addColSpacing( 3, 10 );
    pLayout->addColSpacing( 5, 10 );

    pLayout->addMultiCellWidget(              new QLabel( tr("Federation Name"), this )    , 0, 0, 0, 2 );
    pLayout->addMultiCellWidget( pNameEdit_ = new QLineEdit( strDefaultName.c_str(), this ), 0, 0, 4, 6 );

    pLayout->addWidget         (             new QLabel( tr("FOM file"), this )      , 1, 0 );
    pLayout->addMultiCellWidget( pFOMEdit_ = new QLineEdit( "FOM.fed", this )        , 1, 1, 2, 4 );
    pLayout->addWidget         ( pBrowse_  = new QPushButton( tr( "Browse" ), this ) , 1, 6 );

    QHBox* pButtons = new QHBox( this );
    pLayout->addMultiCellWidget( pButtons, 2, 2, 0, 6 );
    pOk_     = new QPushButton( "Ok"    , pButtons );
    pCancel_ = new QPushButton( "Cancel", pButtons );

    connect( pOk_,     SIGNAL( clicked() ), this, SLOT( OnOk()     ) );
    connect( pCancel_, SIGNAL( clicked() ), this, SLOT( OnCancel() ) );
    connect( pBrowse_, SIGNAL( clicked() ), this, SLOT( OnBrowse() ) );
}

// -----------------------------------------------------------------------------
// Name: HLA_CreateFederationBox destructor
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
HLA_CreateFederationBox::~HLA_CreateFederationBox()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_CreateFederationBox::OnOk
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void HLA_CreateFederationBox::OnOk()
{
    if( ! pNameEdit_->text().isEmpty() && ! pFOMEdit_->text().isEmpty() )
    {
        emit FederationSettings( pNameEdit_->text().ascii(), pFOMEdit_->text().ascii() );
        done( 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: HLA_CreateFederationBox::OnCancel
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void HLA_CreateFederationBox::OnCancel()
{
    done( 0 );
}

// -----------------------------------------------------------------------------
// Name: HLA_CreateFederationBox::OnBrowse
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void HLA_CreateFederationBox::OnBrowse()
{
    pFOMEdit_->setText( QFileDialog::getOpenFileName( "", tr( "Fed files (*.fed)" ), this, "open file dialog", tr( "Choose FED file" ) ) );
}
