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
#include "ADN_ConsistencyDialog.h"
#include "ADN_Tools.h"
#include "ADN_Table.h"
#include "ADN_TableDialog.h"
#include "ADN_GeneralConfig.h"
#include "ADN_Languages_GUI.h"
#include "ADN_ListView.h"
#include "ADN_ListViewDialog.h"
#include "ADN_MainTabWidget.h"
#include "ADN_Enums.h"
#include "ADN_OpenMode_Dialog.h"
#include "ADN_RunProcessDialog.h"
#include "ADN_Project_Data.h"
#include "ADN_ProgressBar.h"
#include "ADN_FileLoaderObserver.h"
#include "clients_gui/FileDialog.h"
#include "clients_gui/GlContext.h"
#include "clients_gui/HelpSystem.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/resources.h"
#include "tools/DefaultLoader.h"
#include "tools/VersionHelper.h"
#include <shlobj.h>
#include <xeumeuleu/xml.hpp>

#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace
{
    tools::Path ReadDataDirectory()
    {
        QSettings settings( "MASA Group", "SWORD" );
        return tools::Path::FromUnicode( settings.readEntry( "/Common/DataDirectory", "" ).toStdWString() );
    }

    tools::Path GetDefaultRoot( const std::string& appName )
    {
        const tools::Path regDir = ReadDataDirectory();
        if( !regDir.IsEmpty() )
            return regDir;
        char myDocuments[ MAX_PATH ];
        SHGetSpecialFolderPath( 0, myDocuments, CSIDL_PERSONAL, 0 );
        return tools::Path( myDocuments ) / tools::Path::FromUTF8( appName );
    }

    QString ReadPassword()
    {
        try
        {
            tools::Xifstream xis( tools::GeneralConfig::BuildResourceChildFile( "authoring.xml" ) );
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
    : QMainWindow ()
    , generalConfig_( new ADN_GeneralConfig( GetDefaultRoot( qApp->translate( "Application", "SWORD" ).toStdString() ) ) )
    , fileLoaderObserver_( new ADN_FileLoaderObserver() )
    , fileLoader_( new tools::DefaultLoader( *fileLoaderObserver_ ) )
    , strAdminPassword_( ReadPassword() )
    , workspace_( ADN_Workspace::GetWorkspace() )
    , openGLContext_( new gui::GlContext() )
    , config_( config )
    , pProjectLoadAction_( 0 )
    , pCoheranceTablesMenu_( 0 )
    , pConfigurationMenu_( 0 )
    , pActionSave_( 0 )
    , pActionSaveAs_( 0 )
    , rIdSaveAs_( 0 )
    , nIdSaveTable_( 0 )
    , nIdPrint_( 0 )
    , nIdChangeOpenMode_( 0 )
    , bSkipSave_( false )
{
    openGLContext_->Init( this->winId() );
    generalConfig_->Parse( argc, argv );
    setMinimumSize( 640, 480 );
    setIcon( gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ) ) );
    setCaption( tr( "Sword Adaptation Tool - No Project" ) + "[*]" );
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
    // Main widget
    Q3VBox* pBox = new Q3VBox( this );
    setCentralWidget( pBox );
    mainTabWidget_.reset( new ADN_MainTabWidget( pBox ) );
    connect( this, SIGNAL( ChangeTab( E_WorkspaceElements ) ), mainTabWidget_.get(), SLOT( OnChangeTab( E_WorkspaceElements ) ) );
    mainTabWidget_->hide();

    consistencyDialog_.reset( new ADN_ConsistencyDialog( this ) );

    // Actions
    QAction* pProjectNewAction = new QAction( MAKE_ICON( new ), tr("&New"), Qt::CTRL + Qt::Key_N, this, "new" );
    connect( pProjectNewAction, SIGNAL( activated() ) , this, SLOT( NewProject() ) );
    pProjectLoadAction_ = new QAction( MAKE_ICON( open ), tr("&Open"), Qt::CTRL + Qt::Key_O, this, "open" );
    connect( pProjectLoadAction_, SIGNAL( activated() ) , this, SLOT( OpenProject() ) );
    pActionSave_ = new QAction( MAKE_ICON( save ), tr("&Save"), Qt::CTRL + Qt::Key_S, this, "save" );
    connect( pActionSave_, SIGNAL( activated() ) , this, SLOT( SaveProject() ) );
    pActionSaveAs_ = new QAction( MAKE_ICON( saveas ), tr( "Save &As" ), Qt::CTRL + Qt::SHIFT + Qt::Key_S, this, "saveas" );
    connect( pActionSaveAs_, SIGNAL( activated() ) , this, SLOT( SaveAsProject() ) );
    // Navigation
    QAction* pBack = new QAction( qApp->style()->standardIcon( QStyle::SP_ArrowLeft ), tr( "Back (ctrl + left)" ), Qt::CTRL + Qt::Key_Left, this, "back" );
    connect( pBack, SIGNAL( activated() ), mainTabWidget_.get(), SLOT( OnBack() ) );
    connect( mainTabWidget_.get(), SIGNAL( BackEnabled( bool ) ), pBack, SLOT( setEnabled( bool ) ) );
    QAction* pForward = new QAction( qApp->style()->standardIcon( QStyle::SP_ArrowRight ), tr( "Forward (ctrl + right)" ), Qt::CTRL + Qt::Key_Right, this, "forward" );
    connect( pForward, SIGNAL( activated() ), mainTabWidget_.get(), SLOT( OnForward() ) );
    pOptional_ = new QAction( tr( "Show optional fields" ), 0 );
    pOptional_->setCheckable( true );
    pOptional_->setChecked( true );
    connect( pOptional_, SIGNAL( toggled( bool ) ), &workspace_, SLOT( OnChooseOptional( bool ) ) );
    connect( mainTabWidget_.get(), SIGNAL( ForwardEnabled( bool ) ), pForward, SLOT( setEnabled( bool ) ) );

    // Project toolbar
    {
        QToolBar * pToolBar = new QToolBar( this );
        pProjectNewAction->addTo( pToolBar );
        pProjectLoadAction_->addTo( pToolBar );
        pActionSave_->addTo( pToolBar );
        pActionSaveAs_->addTo( pToolBar );
        pToolBar->addSeparator();
        pBack->addTo( pToolBar );
        pForward->addTo( pToolBar );
        addToolBar( pToolBar );
    }

    // Project menu
    {
        Q3PopupMenu* pProjectMenu = new Q3PopupMenu( this );
        menuBar()->insertItem( tr("&Project"), pProjectMenu );
        pProjectNewAction->addTo( pProjectMenu );
        pProjectLoadAction_->addTo( pProjectMenu );

        pProjectMenu->insertSeparator();
        pActionSave_->addTo( pProjectMenu );
        pActionSaveAs_->addTo( pProjectMenu );
        pProjectMenu->insertSeparator();
        pProjectMenu->insertItem( tr( "&Export HTML" ), this, SLOT(ExportHtml()) );
        pProjectMenu->insertItem( tr( "Consistency analysis" ), consistencyDialog_.get(), SLOT( Display() ) );
        pOptional_->addTo( pProjectMenu );

        pProjectMenu->insertSeparator();
        pProjectMenu->insertItem( tr("E&xit"),  this, SLOT(close()) );
    }
    // Consistency tables menu
    {
        pCoheranceTablesMenu_ = new Q3PopupMenu( this );
        menuBar()->insertItem( tr( "Consistency &tables" ), pCoheranceTablesMenu_ );
    }
    // Language menu
    {
        pLanguagesMenu_ = workspace_.GetLanguages().GetGui().CreateMenu( this );
        menuBar()->addMenu( pLanguagesMenu_ );
    }
    // Help menu
    {
        gui::HelpSystem* help = new gui::HelpSystem( this, generalConfig_->BuildResourceChildFile( "help/adaptation.xml" ) );
        Q3PopupMenu* pHelpMenu = new Q3PopupMenu( this );
        menuBar()->insertItem( tr( "&Help" ), pHelpMenu );
        pHelpMenu->insertItem( tr( "Help" ), help, SLOT( ShowHelp() ) );
        pHelpMenu->insertSeparator();
        pHelpMenu->insertItem( tr( "&About" ), this, SLOT(About()), Qt::CTRL+Qt::Key_F1 );
    }

    // Disable the menus.
    SetMenuEnabled( false );

    // Status Bar
    {
        QStatusBar* pStatus = statusBar();
        ADN_ProgressBar* pProgressBar = new ADN_ProgressBar( pStatus );
        pStatus->addWidget( pProgressBar );
        workspace_.SetProgressIndicator( pProgressBar );
    }

    // Build all children interfaces
    workspace_.Build( *this, generalConfig_->IsDevMode() );

    connect( pCoheranceTablesMenu_, SIGNAL( activated( int ) ), this, SLOT( ShowCoheranceTable( int ) ) );
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
// Created: JSR 2012-11-07
// -----------------------------------------------------------------------------
void ADN_MainWindow::AddTable( const QString& strTableName, ADN_Callback_ABC<ADN_Table*>*    pCallback )
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
    pActionSaveAs_->setEnabled( bEnabled );
    pLanguagesMenu_->setEnabled( bEnabled );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::SaveProjectAs
// Created: SBO 2006-11-16
// -----------------------------------------------------------------------------
void ADN_MainWindow::SaveProjectAs( const tools::Path& filename )
{
    workspace_.SaveAs( filename, *fileLoader_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::SaveProject
// Created: JSR 2012-04-26
// -----------------------------------------------------------------------------
void ADN_MainWindow::SaveProject()
{
    if( workspace_.GetProject().GetDataInfos().IsReadOnly() )
        SaveAsProject();
    else
        DoSaveProject();
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::DoSaveProject
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_MainWindow::DoSaveProject()
{
    QApplication::setOverrideCursor( Qt::waitCursor ); // this might take time

    bool bNoReadOnlyFiles = true;
    try
    {
        if( !ADN_ConsistencyChecker::GetLoadingErrors().empty() )
            ADN_ConsistencyChecker::ClearLoadingErrors();
        consistencyDialog_->CheckConsistency();
        bNoReadOnlyFiles = workspace_.Save( *fileLoader_ );
    }
    catch( const std::exception& e )
    {
        QApplication::restoreOverrideCursor();    // restore original cursor
        QMessageBox::critical( this, tr( "Error" ), tools::GetExceptionMsg( e ).c_str() );
        return;
    }

    QApplication::restoreOverrideCursor();    // restore original cursor
    setWindowModified( false );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::SaveAsProject
// Created: JDY 03-06-24
//-----------------------------------------------------------------------------
void ADN_MainWindow::SaveAsProject()
{
    tools::Path directory = gui::FileDialog::getExistingDirectory( this, tr( "Save project as" ), generalConfig_->GetModelsDir() );
    if( directory.IsEmpty() )
        return;

    directory /= "physical.xml";
    if( directory.Exists() )
    {
        bool readOnly = false;
        tools::Xifstream xis( directory );
        xis >> xml::optional >> xml::start( "physical" )
            >> xml::optional >> xml::attribute( "read-only", readOnly );
        if( readOnly )
        {
            QMessageBox::warning( this, tr( "Warning" ), tr( "The database you are trying to override is read-only. Please select another directory." ) );
            SaveAsProject();
            return;
        }
    }

    QApplication::setOverrideCursor( Qt::waitCursor ); // this might take time
    bool hasSaved = true;
    try
    {
        hasSaved = workspace_.SaveAs( directory, *fileLoader_ );
        if( !hasSaved )
            QMessageBox::critical( this, tr( "Saving error" ), tr( "Something went wrong during the saving process." ) );
    }
    catch( const std::exception& e )
    {
        hasSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tools::GetExceptionMsg( e ).c_str() );
    }

    QApplication::restoreOverrideCursor();    // restore original cursor
    if( hasSaved )
    {
        QString title = tr( "Sword Adaptation Tool - " ) + directory.ToUTF8().c_str();
        if( workspace_.GetProject().GetDataInfos().IsReadOnly() )
            title += tr( " [ Read Only ]" );
        setCaption( title + "[*]" );
        setWindowModified( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::NewProject
// Created: JSR 2013-03-20
// -----------------------------------------------------------------------------
void ADN_MainWindow::NewProject( const tools::Path& filename )
{
    filename.Parent().CreateDirectories();

    workspace_.SetOpenMode( eOpenMode_Admin );
    emit OpenModeToggled();

    if( ! SelectOpenMode() )
        return;

    SetMenuEnabled( false );
    mainTabWidget_->hide();

    workspace_.Reset( filename );

    SetMenuEnabled( true );
    mainTabWidget_->show();
    setCaption( tr( "Sword Adaptation Tool - " ) + filename.ToUTF8().c_str() + "[*]" );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::NewProject
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_MainWindow::NewProject()
{
    tools::Path path = gui::FileDialog::getExistingDirectory( this, tr( "Create new project" ), generalConfig_->GetModelsDir() );
    if( path.IsEmpty() )
        return;

    path /= "physical.xml";

    NewProject( path );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::OpenProject
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_MainWindow::OpenProject()
{
    tools::Path filename = gui::FileDialog::getOpenFileName( this, tr( "Open physical model project" ), generalConfig_->GetModelsDir(), tr( "Physical model file (physical.xml)" ) );
    if( filename.IsEmpty() )
        return;
    try
    {
        OpenProject( filename );
    }
    catch( const std::exception& e )
    {
        QApplication::restoreOverrideCursor();    // restore original cursor
        workspace_.ResetProgressIndicator();
        QMessageBox::critical( 0, tr( "Error" ), tools::GetExceptionMsg( e ).c_str() );
        CloseApplication( false );  // workspace_.Reset must be fixed (return to a consistent 'cleaned memory' state) to call CloseProject instead of CloseApplication
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::OpenProject
// Created: APE 2005-04-14
// -----------------------------------------------------------------------------
void ADN_MainWindow::OpenProject( const tools::Path& szFilename, bool isAdminMode )
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
    workspace_.Load( szFilename, *fileLoader_ );
    QApplication::restoreOverrideCursor();    // restore original cursor
    QString title = tr( "Sword Adaptation Tool - %1" ).arg( szFilename.ToUTF8().c_str() );
    if( workspace_.GetProject().GetDataInfos().IsReadOnly() )
        title += tr( " [ Read Only ]" );
    setCaption( title + "[*]" );
    SetMenuEnabled( true );
    mainTabWidget_->show();
    if( !ADN_ConsistencyChecker::GetLoadingErrors().empty() )
        consistencyDialog_->CheckConsistency();
    else
        setWindowModified( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::ExportHtml
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_MainWindow::ExportHtml()
{
    tools::Path path = gui::FileDialog::getExistingDirectory( this, QString(), ADN_Workspace::GetWorkspace().GetProject().GetWorkDirInfos().GetWorkingDirectory().GetData() );
    if( path.IsEmpty() )
        return;
    workspace_.ExportHtml( path );
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

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::CloseApplication
// Created: MMC 2013-01-23
//-----------------------------------------------------------------------------
void ADN_MainWindow::CloseApplication( bool bAskSave /*= true*/ )
{
    if( !bAskSave )
        bSkipSave_ = true;
    close();
}


// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::TestData
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
void ADN_MainWindow::TestData()
{
    if( isWindowModified() )
    {
        int nResult = QMessageBox::question( this, tr( "Data test" ), tr( "Project will be saved in order to execute data test." ), QMessageBox::Ok, QMessageBox::Cancel );
        if( nResult == QMessageBox::Cancel )
            return;
        DoSaveProject();
    }

    try
    {
        std::wstring strCommandLine = config_.GetSimPath().ToUnicode() + L" " + config_.GetSimArguments();
        if( workspace_.GetProject().GetFileInfos().GetFileNameFull().IsEmpty() )
        {
            int nResult = QMessageBox::question( this, tr( "Data test" ), tr( "No project loaded, continue anyway?" ), QMessageBox::Yes, QMessageBox::No );
            if( nResult == QMessageBox::No )
                return;
        }

        static ADN_RunProcessDialog* pDialog = new ADN_RunProcessDialog( this, tr( "Running data check" ) );
        pDialog->RunCommand( strCommandLine );
    }
    catch( const std::exception& e )
    {
        QMessageBox::critical( this, tr( "Data test" ), tr( "Data test failed" ) + "\n" + tools::GetExceptionMsg( e ).c_str() );
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
        QString( "Sword Adaptation Tool\n%1\nCopyright (c) MASA Group %2" ).arg( tools::AppProjectVersion() ).arg( QDateTime::currentDateTime().date().year() ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::closeEvent
// Created: JDY 03-09-03
//-----------------------------------------------------------------------------
void ADN_MainWindow::closeEvent( QCloseEvent * e )
{
    if( this->OfferToSave() == true )
    {
        workspace_.Reset( ADN_Project_Data::FileInfos::szUntitled_ );
        e->accept();
    }
    else
        e->ignore();
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

            if( strAdminPassword_ == strGivenPassword )
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
    auto itTable = vTableRegistrations_.find( nId );
    if( itTable != vTableRegistrations_.end() )
    {
        T_TableRegistrationItem& item = itTable->second;
        ADN_Callback_ABC<ADN_Table*>* pCallback = item.second;

        ADN_Table* pTable = (*pCallback)();
        assert( pTable != 0 );
        ADN_TableDialog* pDialog = new ADN_TableDialog( this, item.first, *pTable );

        pDialog->exec();
        delete pDialog;
        return;
    }
    auto itListView = vListViewRegistrations_.find( nId );
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

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::OfferToSave
// Created: APE 2005-04-12
// -----------------------------------------------------------------------------
bool ADN_MainWindow::OfferToSave()
{
    if( bSkipSave_ )
        return true;
    if( !isWindowModified() )
        return true;
    QString strMessage = tr( "Save changes to project %1?" ).arg( workspace_.GetProject().GetFileInfos().GetFileNameFull().ToUTF8().c_str() );
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

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::mousePressEvent
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
void ADN_MainWindow::mousePressEvent( QMouseEvent * event )
{
    if( event && event->button() == Qt::XButton1 )
    {
        mainTabWidget_->OnBack();
        event->accept();
    }
    else if( event && event->button() == Qt::XButton2 )
    {
        mainTabWidget_->OnForward();
        event->accept();
    }
    else
        QMainWindow::mousePressEvent( event );
}
