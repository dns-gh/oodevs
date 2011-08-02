// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ConfigDialog.h"
#include "moc_ADN_ConfigDialog.cpp"

#include "ADN_Config.h"

// -----------------------------------------------------------------------------
// Name: ADN_ConfigDialog::ADN_ConfigDialog
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
ADN_ConfigDialog::ADN_ConfigDialog( ADN_Config& config )
: QDialog  ( 0, tr( "Configuration" ), true, Qt::WStyle_Customize | Qt::WStyle_NormalBorder | Qt::WStyle_Title | Qt::WStyle_SysMenu )
, pConfig_ ( &config )
{
    setMaximumHeight( 200 );
    setMaximumWidth ( 400 );

    Q3GridLayout* pGrid  = new Q3GridLayout( this, 4, 2, 5, 5 );

    pGrid->addWidget( new QLabel( tr( "SIM path" ), this ), 0, 0 );
    pSimPath_ = new QLineEdit( config.GetSimPath().c_str(), this );
    pGrid->addWidget( pSimPath_, 0, 1 );
    pSimPathBrowser_ = new QPushButton( tr( "Browse" ), this );
    pGrid->addWidget( pSimPathBrowser_, 0, 2 );

    pGrid->addWidget( new QLabel( tr( "SIM parameters" ), this ), 1, 0 );
    pSimArguments_ = new QLineEdit( config.GetSimArguments().c_str(), this );
    pGrid->addMultiCellWidget( pSimArguments_, 1, 1, 1, 2 );

    pOk_     = new QPushButton( tr( "&OK" ), this );
    pGrid->addWidget( pOk_, 2, 1 );
    pCancel_ = new QPushButton( tr( "&Cancel" ), this );
    pGrid->addWidget( pCancel_, 2, 2 );
    connect( pOk_    , SIGNAL( clicked() ), this, SLOT( OnOk() ) );
    connect( pCancel_, SIGNAL( clicked() ), this, SLOT( OnCancel() ) );
    connect( pSimPathBrowser_, SIGNAL( clicked() ), this, SLOT( OnBrowsePath() ) );
    pOk_->setFocus();
}


//-----------------------------------------------------------------------------
// Name: ADN_ConfigDialog destructor
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
ADN_ConfigDialog::~ADN_ConfigDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ConfigDialog::OnOk
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
void ADN_ConfigDialog::OnOk()
{
    pConfig_->SetSimPath     ( std::string( pSimPath_->text() ) );
    pConfig_->SetSimArguments( std::string( pSimArguments_->text() ) );
    emit accept();
}

// -----------------------------------------------------------------------------
// Name: ADN_ConfigDialog::OnCancel
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
void ADN_ConfigDialog::OnCancel()
{
    emit reject();
}

// -----------------------------------------------------------------------------
// Name: ADN_ConfigDialog::OnBrowsePath
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
void ADN_ConfigDialog::OnBrowsePath()
{
    QString strSimPath = Q3FileDialog::getOpenFileName( pSimPath_->text(), tr( "Sword executable (*.exe)" ) , this, "", tr( "Select SIM to use for data test" ) );
    if( strSimPath == QString::null )
        return;
    pSimPath_->setText( strSimPath );
}
