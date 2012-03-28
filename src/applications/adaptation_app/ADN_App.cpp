// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_App.h"

#include "ADN_MainWindow.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"
#include "ADN_Exception_ABC.h"
#include "ADN_Config.h"

ADN_App* ADN_App::pApplication_ = 0;

//-----------------------------------------------------------------------------
// Name: ADN_App constructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_App::ADN_App( int argc, char** argv )
    : Application_ABC( argc, argv )
    , pMainWindow_( 0 )
    , config_( new ADN_Config() )
{
    if( pApplication_ )
        throw std::runtime_error( "Singleton already initialized" );
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

//$$$$ C DEGUEU !
// -----------------------------------------------------------------------------
// Name: SIM_App::IsAlreadyWrapped
// Created: MCO 2005-02-22
// -----------------------------------------------------------------------------
bool IsAlreadyWrapped( const std::string& content )
{
    return content.find( "WARNING" ) != std::string::npos || content.find( "ERROR" ) != std::string::npos || content.find( "INFO" ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Wrap
// Created: MCO 2005-02-21
// -----------------------------------------------------------------------------
std::string Wrap( const std::string& content, const std::string& prefix )
{
    std::string result;
    std::stringstream input( content );
    std::string line;
    bool bFirst = true;
    while( std::getline( input, line ) )
    {
        if( ! bFirst )
            result += '\n';
        else
            bFirst = false;
        if( ! IsAlreadyWrapped( line ) )
            result += prefix;
        result += line;
    }
    return result;
}

namespace
{
    QString ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_App::Initialize
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
bool ADN_App::Initialize( const std::string& inputFile, const std::string& outputFile, bool nosymbols, int argc, char** argv )
{
    const QString locale = ReadLang();
    AddTranslator( locale, "qt" );
    AddTranslator( locale, "adaptation" );
    AddTranslator( locale, "adaptation_app" );
    AddTranslator( locale, "ENT" );
    AddTranslator( locale, "clients_gui_sword" );
    AddTranslator( locale, "clients_gui" );
    AddTranslator( locale, "clients_gui_app6" );

    // Initialize all the translations.
    ADN_Tr::InitTranslations();
    ENT_Tr::InitTranslations();

    // Create and set the application's main window.
    pMainWindow_ = new ADN_MainWindow( *config_, argc , argv );
    ADN_Workspace::GetWorkspace().ShowSymbols( !nosymbols );
    pMainWindow_->Build();
    setMainWidget( pMainWindow_ );
    pMainWindow_->showMaximized();

    // Make sure that once the last window is closed, the application quits.
    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );

    try
    {
        if( !inputFile.empty() )
            pMainWindow_->OpenProject( inputFile, true );
        if( !outputFile.empty() )
        {
            pMainWindow_->SaveProjectAs( outputFile );
            return false;
        }
    }
    catch( ADN_Exception_ABC& e )
    {
        if( outputFile.empty() )
            QMessageBox::critical( pMainWindow_, e.GetExceptionTitle().c_str(), e.GetExceptionMessage().c_str() );
        else
        {
            std::stringstream ss;
            ss << e.GetExceptionTitle().c_str() << std::endl << e.GetExceptionMessage().c_str() << std::endl;

            MT_LOG_ERROR_MSG( Wrap( ss.str(), "ERROR: " ) );
        }
        return false;
    }
    return true;
}