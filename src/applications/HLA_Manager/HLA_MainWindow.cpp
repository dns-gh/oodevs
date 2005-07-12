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
// $Archive: /MVW_v10/Build/SDK/HLA_Manager/src/HLA_MainWindow.cpp $
// $Author: Age $
// $Modtime: 8/03/05 10:26 $
// $Revision: 2 $
// $Workfile: HLA_MainWindow.cpp $
//
// *****************************************************************************

#include "HLA_Manager_pch.h"
#include "HLA_MainWindow.h"
#include "HLA_CreateFederationBox.h"

#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qcursor.h>

#include "MIL/HLA/HLA_TimeFactory.h"
#include "MIL/HLA/HLA_TimeIntervalFactory.h"
#include "MIL/HLA/HLA_TimeInterval.h"

#include "HLA/RtiAmbassador_ABC.h"
#include "HLA/Federate.h"
#include "HLA/HLAException.h"

#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

#include "moc_HLA_MainWindow.cpp"

// -----------------------------------------------------------------------------
// Name: HLA_MainWindow constructor
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
HLA_MainWindow::HLA_MainWindow()
    : QMainWindow( 0, "HLA Manager", Qt::WStyle_NoBorder )
    , nTimeStep_       ( 1 )
    , bJoined_         ( false )
    , pFactory_        ( 0 )
    , pIntervalFactory_( 0 )
    , pAmbassador_     ( 0 )
    , pFederate_       ( 0 )
    , strFederateName_ ( "FederationManager" )
    , strFederationName_( "MyFederation" )
    , strConfigFile_( "scipio.xml" )
{
    setCaption( tr( "HLA Manager" ) );
    pFactory_         = new HLA_TimeFactory();
    pIntervalFactory_ = new HLA_TimeIntervalFactory();
    pAmbassador_      = RtiAmbassador_ABC::CreateAmbassador( *pFactory_, *pIntervalFactory_ );
    pFederate_        = new Federate( *pAmbassador_, "FederationManager", HLA_TimeInterval( 10 ) );

    try
    {
        LoadSettings( strConfigFile_ );
    }
    catch( ... )
    {
    }

    QButtonGroup* pGroup = new QButtonGroup( 2, Qt::Vertical, this, "button group" );
    setCentralWidget( pGroup );
    
    pCreateDestroyButton_ = new QPushButton( tr("Create Federation"), pGroup, "the only button" );
    connect( pCreateDestroyButton_, SIGNAL( clicked() ), this, SLOT( OnCreate() ) );

    pStartButton_ = new QPushButton( tr("Start Federation"), pGroup, "the other only button" );
    pStartButton_->setDisabled( true );
    connect( pStartButton_, SIGNAL( clicked() ), this, SLOT( OnStart() ) );

    pCreateBox_ = new HLA_CreateFederationBox( this, strFederationName_ );
    connect( pCreateBox_, SIGNAL( FederationSettings( const std::string&, const std::string& ) ), this, SLOT( ChangeFederationSettings( const std::string&, const std::string& ) ) );

    resize( 200, 0 );
    setFixedSize( width(), height() );
}

// -----------------------------------------------------------------------------
// Name: HLA_MainWindow destructor
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
HLA_MainWindow::~HLA_MainWindow()
{
    delete pFederate_;
    delete pAmbassador_;
    delete pIntervalFactory_;
    delete pFactory_;
}

// -----------------------------------------------------------------------------
// Name: HLA_MainWindow::OnCreate
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void HLA_MainWindow::OnCreate()
{
    if( pCreateBox_->exec() )
    {
        // Create Federation
        try
        {
            QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
            assert( pFederate_ );
            pFederate_->Create( strFederationName_, strFOMFile_ );
            bJoined_ = pFederate_->Join( strFederationName_ );

            // Change slot
            disconnect( pCreateDestroyButton_, SIGNAL( clicked() ), this, SLOT( OnCreate() ) );
            connect(    pCreateDestroyButton_, SIGNAL( clicked() ), this, SLOT( OnDestroy() ) );
            // Rename button
            pCreateDestroyButton_->setText( tr("Destroy Federation" ) );
            // Enable start
            pStartButton_->setDisabled( false );
            QApplication::restoreOverrideCursor();
        }
        catch( HLAException& e )
        {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical( this, "Error creating federation", e.what(), QMessageBox::Ok, QMessageBox::NoButton );
        }
    }

}

// -----------------------------------------------------------------------------
// Name: HLA_MainWindow::OnDestroy
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void HLA_MainWindow::OnDestroy()
{
    try
    {
        assert( pFederate_ );
        if( bJoined_ )
            pFederate_->Resign();
        pFederate_->Destroy( strFederationName_ );

        // Change slot
        disconnect( pCreateDestroyButton_, SIGNAL( clicked() ), this, SLOT( OnDestroy() ) );
        connect(    pCreateDestroyButton_, SIGNAL( clicked() ), this, SLOT( OnCreate() ) );
        // Rename button
        pCreateDestroyButton_->setText( tr("Create Federation" ) );
        // Disable start
        pStartButton_->setDisabled( true );
    }
    catch( HLAException& e )
    {
        QMessageBox::critical( this, "Error destroying federation", e.what(), QMessageBox::Ok, QMessageBox::NoButton );
    }
}

// -----------------------------------------------------------------------------
// Name: HLA_MainWindow::OnStart
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void HLA_MainWindow::OnStart()
{
    try
    {
        assert( pFederate_ );
        pFederate_->Resign();
        bJoined_ = false;
        // Disable start
        pStartButton_->setDisabled( true );
    }
    catch( HLAException& e )
    {
        QMessageBox::critical( this, "Error starting federation", e.what(), QMessageBox::Ok, QMessageBox::NoButton );
    }
}

// -----------------------------------------------------------------------------
// Name: HLA_MainWindow::LoadSettings
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void HLA_MainWindow::LoadSettings( const std::string& strConfigFile )
{
    MT_XXmlInputArchive archive;
    archive.EnableExceptions( true );
    archive.Open( strConfigFile );
    archive.Section( "Scipio" );
    archive.Section( "SIM" );
    archive.ReadField( "TimeStep", nTimeStep_ );
    archive.EndSection();
    archive.Section( "Donnees" );
    std::string strHlaConfigFile;
    archive.ReadField( "HLA", strHlaConfigFile );

    LoadHlaSettings( strHlaConfigFile );
}

// -----------------------------------------------------------------------------
// Name: HLA_MainWindow::LoadHlaSettings
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void HLA_MainWindow::LoadHlaSettings( const std::string& strHlaFile )
{
    MT_XXmlInputArchive archive;
    archive.EnableExceptions( true );
    archive.Open( strHlaFile );
    archive.Section( "HLA" );
    archive.ReadField( "Federation", strFederationName_ );
}

// -----------------------------------------------------------------------------
// Name: HLA_MainWindow::ChangeFederationSettings
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void HLA_MainWindow::ChangeFederationSettings( const std::string& strName, const std::string& strFOM )
{
    strFederationName_ = strName;
    strFOMFile_ = strFOM;
}
