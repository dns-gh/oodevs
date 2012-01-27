// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MainWindow.h"
#include "moc_ADN_MainWindow.cpp"
#include "ADN_Workspace.h"
#include "ADN_Config.h"
#include "ADN_Resources.h"
#include "ADN_Tools.h"
#include "ADN_Table.h"
#include "ADN_TableDialog.h"
#include "ADN_ListView.h"
#include "ADN_ListViewDialog.h"
#include "ADN_MainTabWidget.h"
#include "ADN_Enums.h"
#include "ADN_DataException.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_OpenMode_Dialog.h"
#include "ADN_RunProcessDialog.h"
#include "ADN_Project_Data.h"
#include "ADN_ProgressBar.h"
#include "ADN_SplashScreen.h"
#include "ADN_FileLoaderObserver.h"
#include "tools/DefaultLoader.h"
#include "tools/GeneralConfig.h"
#include "tools/Version.h"
#include "qtundo.h"
#include <shlobj.h>
#include <xeumeuleu/xml.hpp>

#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

namespace
{
    QString ReadDataDirectory()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/DataDirectory", "" );
    }

    std::string GetDefaultRoot( const std::string& appName )
    {
        const QString regDir = ReadDataDirectory();
        if( !regDir.isEmpty() )
            return regDir.ascii();
        char myDocuments[ MAX_PATH ];
        SHGetSpecialFolderPath( 0, myDocuments, CSIDL_PERSONAL, 0 );
        return ( bfs::path( myDocuments, bfs::native ) / appName ).native_file_string();
    }

    QString ReadPassword()
    {
        try
        {
            xml::xifstream xis( tools::GeneralConfig::BuildResourceChildFile( "authoring.xml" ) );
            xis >> xml::start( "authoring" )
                    >> xml::start( "restricted-access" );
            return xis.attribute< std::string >( "password" ).c_str();
        }
        catch( ... )
        {
            return ""; // default password is empty
        }
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow constructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_MainWindow::ADN_MainWindow( ADN_Config& config, int argc, char** argv )
    : QMainWindow       ()
    , generalConfig_     ( new tools::GeneralConfig( GetDefaultRoot( qApp->translate( "Application", "SWORD" ).ascii() ) ) )
    , fileLoaderObserver_( new ADN_FileLoaderObserver() )
    , fileLoader_        ( new tools::DefaultLoader( *fileLoaderObserver_ ) )
    , workspace_         ( ADN_Workspace::GetWorkspace() )
    , config_            ( config )
    , rIdSaveAs_         ( 0 )
    , nIdChangeOpenMode_ ( 0 )
    , pProjectMenu_      ( 0 )
    , pCoheranceTablesMenu_( 0 )
    , pHelpMenu_         ( 0 )
    , bNeedSave_         ( false )
    , strAdminPassword_  ( ReadPassword() )
{
    generalConfig_->Parse( argc, argv );
    setMinimumSize( 640, 480 );
    setIcon( QPixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ) );
    setCaption( tr( "Sword Adaptation Tool - No Project" ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow destructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_MainWindow::~ADN_MainWindow()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::Build
// Created: JDY 03-06-23
//-----------------------------------------------------------------------------
void ADN_MainWindow::Build()
{
    ADN_SplashScreen splashScreen( QPixmap( "ADN.jpg" ) );
    if( splashScreen.IsValid() )
        splashScreen.show();
    workspace_.SetProgressIndicator( &splashScreen );

    // Main widget
    Q3VBox* pBox = new Q3VBox( this );
    setCentralWidget( pBox );
    mainTabWidget_.reset( new ADN_MainTabWidget( pBox ) );
    connect( this, SIGNAL( ChangeTab( E_WorkspaceElements ) ), mainTabWidget_.get(), SLOT( OnChangeTab( E_WorkspaceElements ) ) );
    mainTabWidget_->hide();

    // Actions
    QAction* pProjectNewAction = new QAction( MAKE_PIXMAP(filenew), tr("&New"), Qt::CTRL + Qt::Key_N, this, "new" );
    connect( pProjectNewAction, SIGNAL( activated() ) , this, SLOT( NewProject() ) );

    pProjectLoadAction_ = new QAction( MAKE_PIXMAP(fileopen), tr("&Open"), Qt::CTRL + Qt::Key_O, this, "open" );
    connect( pProjectLoadAction_, SIGNAL( activated() ) , this, SLOT( OpenProject() ) );

    pActionSave_ = new QAction( MAKE_PIXMAP(filesave), tr("&Save"), Qt::CTRL + Qt::Key_S, this, "save" );
    connect( pActionSave_, SIGNAL( activated() ) , this, SLOT( SaveProject() ) );

    QAction* pBack = new QAction( qApp->style()->standardIcon( QStyle::SP_ArrowLeft ), tr( "Back  (ctrl + left)" ), Qt::CTRL + Qt::Key_Left, this, "back" );
    connect( pBack, SIGNAL( activated() ), mainTabWidget_.get(), SLOT( OnBack() ) );
    connect( mainTabWidget_.get(), SIGNAL( BackEnabled( bool ) ), pBack, SLOT( setEnabled( bool ) ) );
    QAction* pForward = new QAction( qApp->style()->standardIcon( QStyle::SP_ArrowRight ), tr( "Forward (ctrl + right)" ), Qt::CTRL + Qt::Key_Right, this, "forward" );
    connect( pForward, SIGNAL( activated() ), mainTabWidget_.get(), SLOT( OnForward() ) );
    connect( mainTabWidget_.get(), SIGNAL( ForwardEnabled( bool ) ), pForward, SLOT( setEnabled( bool ) ) );

    // Test Data
    //QAction* pProjectTestDataAction = new QAction( MAKE_PIXMAP(testdata), tr("&Test data"), CTRL+Key_T, this, "testdata" );
    //connect( pProjectTestDataAction, SIGNAL( activated() ), this, SLOT( TestData() ) );

    // Undo
    //QAction* pUndoAction = QtUndoManager::manager()->createUndoAction( this );
    //pUndoAction->setAccel( QKeySequence( "Ctrl+Z" ) );

    // Project toolbar
    {
        QToolBar * pToolBar = new QToolBar( this );
        pProjectNewAction->addTo( pToolBar );
        pProjectLoadAction_->addTo( pToolBar );
        pActionSave_->addTo( pToolBar );
        pToolBar->addSeparator();
        pBack->addTo( pToolBar );
        pForward->addTo( pToolBar );
        //pToolBar->addSeparator();
        //pProjectTestDataAction->addTo( pToolBar );
        //pUndoAction->addTo( pToolBar );
        addToolBar( pToolBar );
    }

    // Project menu
    {
        pProjectMenu_ = new Q3PopupMenu( this );
        menuBar()->insertItem( tr("&Project"), pProjectMenu_ );
        pProjectNewAction->addTo( pProjectMenu_ );
        pProjectLoadAction_->addTo( pProjectMenu_ );

        pProjectMenu_->insertSeparator();
        pActionSave_->addTo( pProjectMenu_ );
        rIdSaveAs_ = pProjectMenu_->insertItem( tr("Save &As"),  this, SLOT(SaveAsProject()) );
        pProjectMenu_->insertItem( tr("&Export HTML"),  this, SLOT(ExportHtml()) );
        pProjectMenu_->insertSeparator();
        pProjectMenu_->insertItem( tr("E&xit"),  this, SLOT(close()) );
    }

    // Consistency tables menu
    {
        pCoheranceTablesMenu_ = new Q3PopupMenu( this );
        menuBar()->insertItem( tr( "Consistency &tables" ), pCoheranceTablesMenu_ );
    }

    // Configuration menu
    //{
    //    pConfigurationMenu_ = new QPopupMenu( this );
    //    menuBar()->insertItem( tr( "Confi&guration" ), pConfigurationMenu_ );
    //    pConfigurationMenu_->insertItem( tr( "Data test..." ), this, SLOT( ConfigureDataTest() ) );
    //}

    // Help menu
    {
        pHelpMenu_ = new Q3PopupMenu( this );
        menuBar()->insertItem( tr( "&Help" ), pHelpMenu_ );
        pHelpMenu_->insertItem( tr( "&About" ), this, SLOT(About()), Qt::CTRL+Qt::Key_F1 );
    }

    // Disable the menus.
    SetMenuEnabled( false );

    // Build all children interfaces
    workspace_.Build( *this );

    // Status Bar
    {
        QStatusBar* pStatus = statusBar();
        ADN_ProgressBar* pProgressBar = new ADN_ProgressBar( pStatus );
        pStatus->addWidget( pProgressBar );
        workspace_.SetProgressIndicator( pProgressBar );
    }

    connect( pCoheranceTablesMenu_, SIGNAL( activated( int ) ), this, SLOT( ShowCoheranceTable( int ) ) );
    connect( &workspace_.GetUndoStack(), SIGNAL( cleanChanged(bool) ), this, SLOT( ChangeSaveState(bool) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::AddPage
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_MainWindow::AddPage( E_WorkspaceElements element, QWidget& page, const QString& title )
{
    mainTabWidget_->AddPage( element, page, title );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::AddTable
// Created: APE 2005-03-29
// -----------------------------------------------------------------------------
void ADN_MainWindow::AddTable( const QString& strTableName, ADN_Callback_ABC<ADN_Table*>* pCallback )
{
    vTableRegistrations_.insert( std::make_pair( pCoheranceTablesMenu_->count(), T_TableRegistrationItem( strTableName, pCallback ) ) );
    pCoheranceTablesMenu_->insertItem( strTableName, pCoheranceTablesMenu_->count() );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::AddListView
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
void ADN_MainWindow::AddListView( const QString& strTableName, ADN_Callback_ABC<ADN_ListView*>* pCallback )
{
    vListViewRegistrations_.insert( std::make_pair( pCoheranceTablesMenu_->count(), T_ListViewRegistrationItem( strTableName, pCallback ) ) );
    pCoheranceTablesMenu_->insertItem( strTableName, pCoheranceTablesMenu_->count() );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::SetMenuEnabled
// Created: JDY 03-06-24
//-----------------------------------------------------------------------------
void ADN_MainWindow::SetMenuEnabled( bool bEnabled )
{
    pActionSave_->setEnabled( bEnabled );
    pProjectMenu_->setItemEnabled( rIdSaveAs_, bEnabled );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::SaveProject
// Created: SBO 2006-11-16
// -----------------------------------------------------------------------------
void ADN_MainWindow::SaveProjectAs( const std::string& filename )
{
    std::string res( filename );
    std::replace( res.begin(), res.end(), '\\', '/' );
    workspace_.SaveAs( res );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::SaveProject
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_MainWindow::SaveProject()
{
//    if( ! bNeedSave_ )
//    return;

    QApplication::setOverrideCursor( Qt::waitCursor ); // this might take time

    bool bNoReadOnlyFiles = true;
    try
    {
        bNoReadOnlyFiles = workspace_.Save();
    }
    catch( ADN_Exception_ABC& exception )
    {
        QApplication::restoreOverrideCursor();    // restore original cursor
        QMessageBox::critical( this, exception.GetExceptionTitle().c_str(), exception.GetExceptionMessage().c_str() );
        return;
    }

    QApplication::restoreOverrideCursor();    // restore original cursor

    if( ! bNoReadOnlyFiles )
        return; // we were not able to save all the datas

    std::string szProject = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + workspace_.GetProject().GetFileInfos().GetFileName().GetData();
    setCaption( tr( "Sword Adaptation Tool - " ) + szProject.c_str() );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::SaveAsProject
// Created: JDY 03-06-24
//-----------------------------------------------------------------------------
void ADN_MainWindow::SaveAsProject()
{
    QString strDirectoryName = QFileDialog::getExistingDirectory( generalConfig_->GetModelsDir().c_str(), this, "", tr( "Save project as" ), true );
    if( strDirectoryName.isEmpty() )
        return;

    QApplication::setOverrideCursor( Qt::waitCursor ); // this might take time
    std::string res = strDirectoryName.toStdString();
    std::replace( res.begin(), res.end(), '\\', '/' );
    res += "/physical.xml";
    bool hasSaved = true;
    try
    {
        hasSaved = workspace_.SaveAs( res );
        if( !hasSaved )
            QMessageBox::critical( this, tr( "Saving error" ), tr( "Something went wrong during the saving process." ) );
    }
    catch( ADN_Exception_ABC& exception )
    {
        hasSaved = false;
        QMessageBox::critical( this, exception.GetExceptionTitle().c_str(), exception.GetExceptionMessage().c_str() );
    }

    QApplication::restoreOverrideCursor();    // restore original cursor
    if( hasSaved )
        setCaption( tr( "Sword Adaptation Tool - " ) + res.c_str() );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::NewProject
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_MainWindow::NewProject()
{
    QString qfilename = Q3FileDialog::getSaveFileName( generalConfig_->GetModelsDir().c_str(), tr( "Physical model file (physical.xml)" ), this, "", tr( "Create new project" ) );
    if ( qfilename == QString::null )
        return;

    workspace_.SetOpenMode( eOpenMode_Admin );
    emit OpenModeToggled();

    if( ! SelectOpenMode() )
        return;

    SetMenuEnabled(false);
    mainTabWidget_->hide();

    std::string res( qfilename );
    std::replace( res.begin(), res.end(), '\\', '/' );
    workspace_.Reset( res );

    SetMenuEnabled(true);
    mainTabWidget_->show();
    QString strCaption = tr( "Sword Adaptation Tool - " ) + qfilename;
    setCaption( strCaption );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::OpenProject
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_MainWindow::OpenProject()
{
    QString qfilename = Q3FileDialog::getOpenFileName( generalConfig_->GetModelsDir().c_str(), tr("Physical model file (physical.xml)"), this, "", tr("Open physical model project"));
    if( qfilename == QString::null )
        return;
    try
    {
        OpenProject( qfilename.ascii() );
    }
    catch( ADN_Exception_ABC& exception )
    {
        QApplication::restoreOverrideCursor();    // restore original cursor
        QMessageBox::critical( this, exception.GetExceptionTitle().c_str(), exception.GetExceptionMessage().c_str() );
    }
    catch( std::exception& e )
    {
        QApplication::restoreOverrideCursor();    // restore original cursor
        workspace_.ResetProgressIndicator();
        QMessageBox::critical( 0, "Error", e.what() );
        CloseProject();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::OpenProject
// Created: APE 2005-04-14
// -----------------------------------------------------------------------------
void ADN_MainWindow::OpenProject( const std::string& szFilename, const bool isAdminMode )
{
    if( isAdminMode )
    {
        workspace_.SetOpenMode( eOpenMode_Admin );
        emit OpenModeToggled();
    }
    else if( ! SelectOpenMode() )
        return;

    SetMenuEnabled( false );
    mainTabWidget_->hide();

    QApplication::setOverrideCursor( Qt::waitCursor ); // this might take time
    if( QString( szFilename.c_str() ).startsWith( "//" ) )
    {
        std::string res( szFilename );
        std::replace( res.begin(), res.end(), '/', '\\' );
        workspace_.Load( res, *fileLoader_ );
    }
    else
        workspace_.Load( szFilename, *fileLoader_ );
    QApplication::restoreOverrideCursor();    // restore original cursor
    setCaption( tr( "Sword Adaptation Tool - %1" ).arg( szFilename.c_str() ) );
    SetMenuEnabled( true );
    mainTabWidget_->show();
    if( !isAdminMode && !fileLoaderObserver_->GetInvalidSignedFiles().empty() )
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        QMessageBox::warning( this, qApp->translate( "Application", "SWORD" )
                    , tr( "The signatures for the following files do not exist or are invalid : " ) + "\n" + fileLoaderObserver_->GetInvalidSignedFiles().c_str() );
    }
    pProjectLoadAction_->setVisible( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::ExportHtml
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_MainWindow::ExportHtml()
{
    QString strPath = Q3FileDialog::getExistingDirectory( ADN_Workspace::GetWorkspace().GetProject().GetWorkDirInfos().GetWorkingDirectory().GetData().c_str(), this );
    if( strPath == QString::null )
        return;

    if( strPath.at( strPath.length() - 1 ) != '/' )
        strPath += '/';

    workspace_.ExportHtml( strPath.ascii() );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::CloseProject
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_MainWindow::CloseProject()
{
    if( ! this->OfferToSave() )
        return;
    workspace_.Reset( ADN_Project_Data::FileInfos::szUntitled_ );
    mainTabWidget_->hide();
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::TestData
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
void ADN_MainWindow::TestData()
{
    if( bNeedSave_ )
    {
        int nResult = QMessageBox::question( this, tr( "Data test" ), tr( "Project will be saved in order to execute data test." ), QMessageBox::Ok, QMessageBox::Cancel );
        if( nResult == QMessageBox::Cancel )
            return;
        SaveProject();
    }

    try
    {
        std::string strCommandLine = config_.GetSimPath() + " " + config_.GetSimArguments();
        if( workspace_.GetProject().GetFileInfos().GetFileNameFull().empty() )
        {
            int nResult = QMessageBox::question( this, tr( "Data test" ), tr( "No project loaded, continue anyway?" ), QMessageBox::Yes, QMessageBox::No );
            if( nResult == QMessageBox::No )
                return;
        }
        //  else
        //      strCommandLine += " -conffile \"" + workspace_.GetProject().GetFileInfos().GetFileNameFull() + "\"";

        static ADN_RunProcessDialog* pDialog = new ADN_RunProcessDialog( this, tr( "Running data check" ) );
        pDialog->RunCommand( strCommandLine );
    }
    catch( const std::exception& e )
    {
        QMessageBox::critical( this, tr( "Data test" ), tr( "Data test failed" ) + "\n" + e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::ConfigureDataTest
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
void ADN_MainWindow::ConfigureDataTest()
{
    config_.Configure();
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::About
// Created: JDY 03-09-11
//-----------------------------------------------------------------------------
void ADN_MainWindow::About()
{
    QMessageBox::about( this , tr( "Sword Adaptation Tool" ),
        QString( "Sword Adaptation Tool\n%1\nCopyright (c) MASA Group 2011" ).arg( tools::AppVersion() ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::closeEvent
// Created: JDY 03-09-03
//-----------------------------------------------------------------------------
void ADN_MainWindow::closeEvent( QCloseEvent * e )
{
    if( this->OfferToSave() == true )
        e->accept();
    else
        e->ignore();
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::ChangeSaveState
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_MainWindow::ChangeSaveState( bool bNoCommand )
{
    bNeedSave_ = ! bNoCommand;
    if( bNeedSave_ )
        setCaption( caption() + "*" );
    else
    {
        std::string szProject = workspace_.GetProject().GetFileInfos().GetFileName().GetData();
        if( szProject == ADN_Project_Data::FileInfos::szUntitled_ )
            setCaption( tr( "Sword Adaptation Tool - No Project" ) );
        else
        {
            szProject = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + szProject;
            setCaption( tr( "Sword Adaptation Tool - " ) + szProject.c_str() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::SelectOpenMode
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
bool ADN_MainWindow::SelectOpenMode()
{
    E_OpenMode nMode = eOpenMode_Normal;

    if( strAdminPassword_.isEmpty() || strAdminPassword_.isNull() )
        nMode = eOpenMode_Admin;
    else
        for(;;)
        {
            // let the user choose the open mode of its project
            ADN_OpenMode_Dialog openModeDialog( this );

            if( openModeDialog.exec() == QDialog::Rejected )
                return false;

            nMode = openModeDialog.GetOpenMode();
            if( nMode == eOpenMode_Normal )
                break; // normal mode, we can continue

            // admin mode, we need to check the password
            assert( nMode == eOpenMode_Admin );
            QString strGivenPassword = openModeDialog.GetPassword();

            if( strAdminPassword_ == strGivenPassword.ascii() )
                break; // password ok

            // wrong password, inform the user and let him try again
            QMessageBox::critical( this,
                                   tr( "Invalid password"),
                                   tr( "The password you entered is not valid. Please try again."),
                                   QMessageBox::Ok,
                                   QMessageBox::NoButton );
        }

    workspace_.SetOpenMode( nMode );

    emit OpenModeToggled();
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::ShowCoheranceTable
// Created: APE 2005-04-04
// -----------------------------------------------------------------------------
void ADN_MainWindow::ShowCoheranceTable( int nId )
{
    //assert( nId < (int)vTableRegistrations_.size() );
    IT_TableRegistrationMap itTable = vTableRegistrations_.find( nId );
    if( itTable != vTableRegistrations_.end() )
    {
        T_TableRegistrationItem& item = itTable->second;
        ADN_Callback_ABC<ADN_Table*>* pCallback = item.second;

        ADN_Table* pTable = (*pCallback)();
        ADN_TableDialog* pDialog = new ADN_TableDialog( this, item.first, pTable );

        pDialog->exec();
        delete pDialog;
    }
    else
    {
        IT_ListViewRegistrationMap itListView = vListViewRegistrations_.find( nId );
        if( itListView != vListViewRegistrations_.end() )
        {
            T_ListViewRegistrationItem& item = itListView->second;
            ADN_Callback_ABC<ADN_ListView*>* pCallback = item.second;

            ADN_ListView* pListView = (*pCallback)();
            ADN_ListViewDialog* pDialog = new ADN_ListViewDialog( this, item.first, pListView );

            pDialog->exec();
            delete pDialog;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::OfferToSave
// Created: APE 2005-04-12
// -----------------------------------------------------------------------------
bool ADN_MainWindow::OfferToSave()
{
    if( ! bNeedSave_ )
        return true;

    QString strMessage = tr( "Save changes to project %1?" ).arg( workspace_.GetProject().GetFileInfos().GetFileNameFull().c_str() );
    int nResult = QMessageBox::information( this, tr( "Sword Adaptation Tool" ), strMessage, QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel );
    switch( nResult )
    {
        case QMessageBox::Yes:
            SaveProject();
        case QMessageBox::No:
            return true;
    }
    return false; // Cancel whatever action prompted the save proposition.
}
