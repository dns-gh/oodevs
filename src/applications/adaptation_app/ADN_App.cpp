//*****************************************************************************
//
// $Created: JDY 03-06-19 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_App.cpp $
// $Author: Ape $
// $Modtime: 14/04/05 15:56 $
// $Revision: 10 $
// $Workfile: ADN_App.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_App.h"

#include "ADN_MainWindow.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

#include <qtextcodec.h>
#include <qtranslator.h>

ADN_App* ADN_App::pApplication_ = 0;


//-----------------------------------------------------------------------------
// Name: ADN_App constructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_App::ADN_App( int argc, char** argv )
: QApplication( argc, argv )
, pMainWindow_( 0 )
{
    assert( pApplication_ == 0 );
    pApplication_ = this;
}



//-----------------------------------------------------------------------------
// Name: ADN_App destructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_App::~ADN_App()
{
    if( pMainWindow_ != 0 )
    {
        pMainWindow_->hide();
        
        // Don't delete the main window to speed up the closing of the application.
        // Restore the next line if checking for memory leaks.
        // delete pMainWindow_;
    }

    pApplication_ = 0;
}


//-----------------------------------------------------------------------------
// Name: ADN_App::Initialize
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_App::Initialize( uint nArgc, char** ppArgv )
{
    // Set the translator files.
    QTranslator* pADNTranslator = new QTranslator( this );
    pADNTranslator->load( QString( "adn_" ) + QTextCodec::locale() + QString( ".qm" ), "." );
    installTranslator( pADNTranslator );

    QTranslator* pENTTranslator = new QTranslator( this );
    pENTTranslator->load( QString( "ENT_" ) + QTextCodec::locale() + QString( ".qm" ), "." );
    installTranslator( pENTTranslator );

    // Initialize all the translations.
    ADN_Tr::InitTranslations();
    ENT_Tr::InitTranslations();

    // Create and set the application's main window.
    pMainWindow_ = new ADN_MainWindow();
    pMainWindow_->Build();
    setMainWidget( pMainWindow_ );
    pMainWindow_->showMaximized();

    // Make sure that once the last window is closed, the application quits.
    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );

    if( nArgc > 1 )
        pMainWindow_->OpenProject( ppArgv[1] );
}

