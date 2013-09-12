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

namespace
{
    QAction* CreateAction( const QString& text, QObject* parent, const char* name )
    {
        QAction* action = new QAction( text, parent );
        action->setObjectName( name );
        return action;
    }
    QAction* CreateAction( const QString& text, QObject* parent, const char* name, const QKeySequence& shortcut )
    {
        QAction* action = CreateAction( text, parent, name );
        action->setShortcut( shortcut );
        return action;
    }
    QAction* CreateAction( const QString&text, QObject* parent, const char* name, const QIcon& icon )
    {
        QAction* action = new QAction( icon, text, parent );
        action->setObjectName( name );
        return action;
    }
    QAction* CreateAction( const QString&text, QObject* parent, const char* name, const QIcon& icon, const QKeySequence& shortcut )
    {
        QAction* action = CreateAction( text, parent, name, icon );
        action->setShortcut( shortcut );
        return action;
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow constructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_MainWindow::ADN_MainWindow( const ADN_GeneralConfig& config )
    : QMainWindow ()
    , config_( config )
    , fileLoaderObserver_( new ADN_FileLoaderObserver() )
    , fileLoader_( new tools::DefaultLoader( *fileLoaderObserver_ ) )
    , openGLContext_( new gui::GlContext() )
    , actionClose_( 0 )
    , actionSave_( 0 )
    , actionSaveAs_( 0 )
    , actionOptional_( 0 )
    , actionBack_( 0 )
    , actionForward_( 0 )
    , actionExportHtml_( 0 )
    , actionConsistencyAnalysis_( 0 )
    , menuConsistencyTables_( 0 )
    , menuLanguages_( 0 )
    , skipSave_( false )
    , isOpen_( false )
{
    // Init
    openGLContext_->Init( this->winId() );
    setMinimumSize( 640, 480 );
    setIcon( gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ) ) );
    setCaption( tr( "Sword Adaptation Tool - No Project" ) + "[*]" );

    // Actions
    QAction* actionNew = CreateAction( tr("&New"), this, "new", MAKE_ICON( new ), Qt::CTRL + Qt::Key_N );
    QAction* actionLoad = CreateAction( tr("&Open"), this, "open", MAKE_ICON( open ), Qt::CTRL + Qt::Key_O );
    actionClose_ = CreateAction( tr("&Close"), this, "close", Qt::CTRL + Qt::Key_W );
    actionSave_ = CreateAction( tr("&Save"), this, "save", MAKE_ICON( save ), Qt::CTRL + Qt::Key_S );
    actionSaveAs_ = CreateAction( tr("Save &As"), this, "saveas", MAKE_ICON( saveas ), Qt::CTRL + Qt::SHIFT + Qt::Key_S );
    QAction* actionExit = CreateAction( tr( "E&xit" ), this, "exit", Qt::CTRL + Qt::Key_Q );
    actionBack_ = CreateAction( tr("Back (ctrl + left)"), this, "back", qApp->style()->standardIcon( QStyle::SP_ArrowLeft ), Qt::CTRL + Qt::Key_Left );
    actionForward_ = CreateAction( tr( "Forward (ctrl + right)" ), this, "forward", qApp->style()->standardIcon( QStyle::SP_ArrowRight ), Qt::CTRL + Qt::Key_Right );
    actionOptional_ = CreateAction( tr( "Show optional fields" ), this, "optional" );
    actionOptional_->setCheckable( true );
    actionOptional_->setChecked( true );
    actionExportHtml_ = CreateAction( tr( "&Export HTML" ), this, "exporthtml", Qt::CTRL + Qt::Key_E );
    actionConsistencyAnalysis_ = CreateAction( tr( "Consistency analysis" ), this, "consistency-analysis" );

    connect( actionNew, SIGNAL( triggered() ), this, SLOT( OnNew() ) );
    connect( actionLoad, SIGNAL( triggered() ), this, SLOT( OnOpen() ) );
    connect( actionClose_, SIGNAL( triggered() ), this, SLOT( OnClose() ) );
    connect( actionExit, SIGNAL( triggered() ), this, SLOT( close() ) );
    connect( actionSave_, SIGNAL( triggered() ), this, SLOT( OnSave() ) );
    connect( actionSaveAs_, SIGNAL( triggered() ), this, SLOT( OnSaveAs() ) );
    connect( actionExportHtml_, SIGNAL( triggered() ), this, SLOT( OnExportHtml() ) );

    // Toolbar
    QToolBar* pToolBar = new QToolBar( this );
    addToolBar( pToolBar );
    pToolBar->addAction( actionNew );
    pToolBar->addAction( actionLoad );
    pToolBar->addAction( actionSave_ );
    pToolBar->addAction( actionSaveAs_ );
    pToolBar->addSeparator();
    pToolBar->addAction( actionBack_ );
    pToolBar->addAction( actionForward_ );

    // Menus
    QMenu* menuProject = new QMenu( tr("&Project"), this );
    menuProject->addAction( actionNew );
    menuProject->addAction( actionLoad );
    menuProject->insertSeparator();
    menuProject->addAction( actionSave_ );
    menuProject->addAction( actionSaveAs_ );
    menuProject->insertSeparator();
    menuProject->addAction( actionClose_ );
    menuProject->insertSeparator();
    menuProject->addAction( actionExportHtml_ ); 
    menuProject->addAction( actionConsistencyAnalysis_ );
    menuProject->addAction( actionOptional_ );
    menuProject->insertSeparator();
    menuProject->addAction( actionExit );

    menuConsistencyTables_ = new QMenu( tr( "Consistency &tables" ), this );
    menuLanguages_ = new QMenu( tr( "Languages" ), this );

    QMenu* menuHelp = new QMenu( tr( "&Help" ), this );
    gui::HelpSystem* help = new gui::HelpSystem( this, config_.BuildResourceChildFile( "help/adaptation.xml" ) );
    menuHelp->addAction( tr( "Help" ), help, SLOT( ShowHelp() ) );
    menuHelp->insertSeparator();
    menuHelp->addAction( tr( "&About" ), this, SLOT( OnAbout() ), Qt::CTRL + Qt::Key_F1 );

    menuBar()->addMenu( menuProject );
    menuBar()->addMenu( menuConsistencyTables_ );
    menuBar()->addMenu( menuLanguages_ );
    menuBar()->addMenu( menuHelp );

    // Status bar
    progressBar_.reset( new ADN_ProgressBar( statusBar() ) );
    statusBar()->addWidget( progressBar_.get() );

    OnOpenned( false );
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
    connect( actionBack_, SIGNAL( triggered() ), mainTabWidget_.get(), SLOT( OnBack() ) );
    connect( actionForward_, SIGNAL( triggered() ), mainTabWidget_.get(), SLOT( OnForward() ) );
    connect( mainTabWidget_.get(), SIGNAL( BackEnabled( bool ) ), actionBack_, SLOT( setEnabled( bool ) ) );
    connect( mainTabWidget_.get(), SIGNAL( ForwardEnabled( bool ) ), actionForward_, SLOT( setEnabled( bool ) ) );

    // Consistency dialog
    consistencyDialog_.reset( new ADN_ConsistencyDialog( this ) );
    connect( actionConsistencyAnalysis_, SIGNAL( triggered() ), consistencyDialog_.get(), SLOT( Display() ) );
    // Consistency tables
    consistencyMapper_ = new QSignalMapper( this );
    connect( consistencyMapper_, SIGNAL( mapped( const QString& ) ), this, SLOT( ShowConsistencyTable( const QString& ) ) );

    // Workspace interaction
    connect( actionOptional_, SIGNAL( triggered( bool ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnChooseOptional( bool ) ) );
    ADN_Workspace::GetWorkspace().SetProgressIndicator( progressBar_.get() );

    ADN_Workspace::GetWorkspace().Build( *this, config_.IsDevMode() );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::New
// Created: JSR 2013-03-20
// -----------------------------------------------------------------------------
void ADN_MainWindow::New( const tools::Path& filename )
{
    filename.Parent().CreateDirectories();
    if( mainTabWidget_.get() )
        mainTabWidget_->setVisible( false );
    ADN_Workspace::GetWorkspace().Reset( filename );
    if( mainTabWidget_.get() )
        mainTabWidget_->setVisible( true );
    setCaption( tr( "Sword Adaptation Tool - " ) + filename.ToUTF8().c_str() + "[*]" );
    OnOpenned( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::OnNew
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_MainWindow::OnNew()
{
    tools::Path path = gui::FileDialog::getExistingDirectory( this, tr( "Create new project" ), config_.GetModelsDir() );
    if( path.IsEmpty() )
        return;
    New( path / "physical.xml" );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::Open
// Created: APE 2005-04-14
// -----------------------------------------------------------------------------
void ADN_MainWindow::Open( const tools::Path& filename )
{
    if( isOpen_ )
        OnClose();
    QApplication::setOverrideCursor( Qt::waitCursor );
    ADN_Workspace::GetWorkspace().Load( filename, *fileLoader_ );
    QApplication::restoreOverrideCursor();
    QString title = tr( "Sword Adaptation Tool - %1" ).arg( filename.ToUTF8().c_str() );
    if( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().IsReadOnly() )
        title += tr( " [ Read Only ]" );
    setCaption( title + "[*]" );
    if( !ADN_ConsistencyChecker::GetLoadingErrors().empty() )
        consistencyDialog_->CheckConsistency();
    else
        setWindowModified( false );
    OnOpenned( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::OnOpen
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_MainWindow::OnOpen()
{
    tools::Path filename = gui::FileDialog::getOpenFileName( this, tr( "Open physical model project" ), config_.GetModelsDir(), tr( "Physical model file (physical.xml)" ) );
    if( filename.IsEmpty() )
        return;
    try
    {
        Open( filename );
    }
    catch( const std::exception& e )
    {
        ADN_Workspace::GetWorkspace().ResetProgressIndicator();
        QMessageBox::critical( 0, tr( "Error" ), tools::GetExceptionMsg( e ).c_str() );
        skipSave_ = true;
        OnClose();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::OfferToSave
// Created: APE 2005-04-12
// -----------------------------------------------------------------------------
bool ADN_MainWindow::OfferToSave()
{
    if( skipSave_ || !isWindowModified() )
        return true;
    QString strMessage = tr( "Save changes to project %1?" ).arg( ADN_Workspace::GetWorkspace().GetProject().GetFileInfos().GetFileNameFull().ToUTF8().c_str() );
    int nResult = QMessageBox::information( this, tr( "Sword Adaptation Tool" ), strMessage, QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel );
    if( nResult == QMessageBox::Yes )
    {
        if( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().IsReadOnly() )
            OnSaveAs();
        else
            OnSave();
    }
    else if( nResult == QMessageBox::Cancel )
        return false;
    return true;
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::OnClose
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_MainWindow::OnClose()
{
    if( !OfferToSave() )
        return;
    skipSave_ = false;
    OnOpenned( false );
    QApplication::setOverrideCursor( Qt::waitCursor );
    ADN_Workspace::GetWorkspace().Reset( ADN_Project_Data::FileInfos::szUntitled_ );
    QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::closeEvent
// Created: JDY 03-09-03
//-----------------------------------------------------------------------------
void ADN_MainWindow::closeEvent( QCloseEvent * e )
{
    if( OfferToSave() )
        e->accept();
    else
        e->ignore();
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::SaveAs
// Created: SBO 2006-11-16
// -----------------------------------------------------------------------------
void ADN_MainWindow::SaveAs( const tools::Path& filename )
{
    if( !IsNewBaseReadOnly( filename ) )
        ADN_Workspace::GetWorkspace().SaveAs( filename, *fileLoader_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::OnSave
// Created: JSR 2012-04-26
// -----------------------------------------------------------------------------
void ADN_MainWindow::OnSave()
{
    QApplication::setOverrideCursor( Qt::waitCursor );
    try
    {
        if( !ADN_ConsistencyChecker::GetLoadingErrors().empty() )
            ADN_ConsistencyChecker::ClearLoadingErrors();
        consistencyDialog_->CheckConsistency();
        ADN_Workspace::GetWorkspace().Save( *fileLoader_ );
    }
    catch( const std::exception& e )
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical( this, tr( "Error" ), tools::GetExceptionMsg( e ).c_str() );
        return;
    }
    QApplication::restoreOverrideCursor();
    setWindowModified( false );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::OnSaveAs
// Created: JDY 03-06-24
//-----------------------------------------------------------------------------
void ADN_MainWindow::OnSaveAs()
{
    tools::Path directory = gui::FileDialog::getExistingDirectory( this, tr( "Save project as" ), config_.GetModelsDir() );
    if( directory.IsEmpty() )
        return;

    directory /= "physical.xml";
    if( IsNewBaseReadOnly( directory ) )
    {
        QMessageBox::warning( this, tr( "Warning" ), tr( "The database you are trying to override is read-only. Please select another directory." ) );
        OnSaveAs();
        return;
    }

    QApplication::setOverrideCursor( Qt::waitCursor );
    bool hasSaved = true;
    try
    {
        hasSaved = ADN_Workspace::GetWorkspace().SaveAs( directory, *fileLoader_ );
        if( !hasSaved )
            QMessageBox::critical( this, tr( "Saving error" ), tr( "Something went wrong during the saving process." ) );
    }
    catch( const std::exception& e )
    {
        hasSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tools::GetExceptionMsg( e ).c_str() );
    }
    QApplication::restoreOverrideCursor();

    if( hasSaved )
    {
        QString title = tr( "Sword Adaptation Tool - " ) + directory.ToUTF8().c_str();
        if( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().IsReadOnly() )
            title += tr( " [ Read Only ]" );
        setCaption( title + "[*]" );
        setWindowModified( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::AddPage
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_MainWindow::AddPage( E_WorkspaceElements element, QWidget& page, const QString& title )
{
    assert( mainTabWidget_.get() );
    mainTabWidget_->AddPage( element, page, title );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::AddTable
// Created: JSR 2012-11-07
// -----------------------------------------------------------------------------
void ADN_MainWindow::AddTable( const QString& name, ADN_Callback_ABC< ADN_Table* >* pCallback )
{
    QAction* action = new QAction( name, this );
    QObject::connect( action, SIGNAL( triggered() ), consistencyMapper_, SLOT( map() ) );
    consistencyMapper_->setMapping( action, name );
    menuConsistencyTables_->addAction( action );
    consistencyTables_[ name ] = pCallback;
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::AddListView
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
void ADN_MainWindow::AddListView( const QString& name, ADN_Callback_ABC< ADN_ListView* >* pCallback )
{
    QAction* action = new QAction( name, this );
    QObject::connect( action, SIGNAL( triggered() ), consistencyMapper_, SLOT( map() ) );
    consistencyMapper_->setMapping( action, name );
    menuConsistencyTables_->addAction( action );
    consistencyListViews_[ name ] = pCallback;
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::OnOpenned
// Created: ABR 2013-09-12
// -----------------------------------------------------------------------------
void ADN_MainWindow::OnOpenned( bool isOpen )
{
    if( mainTabWidget_.get() )
        mainTabWidget_->setVisible( isOpen );
    isOpen_ = isOpen;
    actionSave_->setVisible( isOpen && !ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().IsReadOnly() );
    actionSaveAs_->setVisible( isOpen );
    actionClose_->setVisible( isOpen );
    actionExportHtml_->setVisible( isOpen );
    actionConsistencyAnalysis_->setVisible( isOpen );
    actionOptional_->setVisible( isOpen );
    actionBack_->setVisible( isOpen );
    actionForward_->setVisible( isOpen );
    menuConsistencyTables_->menuAction()->setVisible( isOpen );
    menuLanguages_->menuAction()->setVisible( isOpen );
    if( !isOpen )
        menuLanguages_->clear();
    else
        ADN_Workspace::GetWorkspace().GetLanguages().GetGui().FillMenu( menuLanguages_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::OnExportHtml
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_MainWindow::OnExportHtml()
{
    tools::Path path = gui::FileDialog::getExistingDirectory( this, QString(), ADN_Workspace::GetWorkspace().GetProject().GetWorkDirInfos().GetWorkingDirectory().GetData() );
    if( path.IsEmpty() )
        return;
    ADN_Workspace::GetWorkspace().ExportHtml( path );
}

//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::OnAbout
// Created: JDY 03-09-11
//-----------------------------------------------------------------------------
void ADN_MainWindow::OnAbout()
{
    QMessageBox::about( this , tr( "Sword Adaptation Tool" ),
        QString( "Sword Adaptation Tool\n%1\nCopyright (c) MASA Group %2" ).arg( tools::AppProjectVersion() ).arg( QDateTime::currentDateTime().date().year() ) );
}

namespace
{
    template< typename Dialog, typename Element >
    bool ShowConsistencyDialog( const QString& name, const std::map< QString, ADN_Callback_ABC< Element* >* >& map )
    {
        auto it = map.find( name );
        if( it == map.end() )
            return false;
        ADN_Callback_ABC< Element* >* callback = it->second;
        Element* element = (*callback)();
        assert( element != 0 );
        Dialog dialog( 0, name, *element );
        dialog.exec();
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::ShowCoheranceTable
// Created: ABR 2013-09-12
// -----------------------------------------------------------------------------
void ADN_MainWindow::ShowConsistencyTable( const QString& name ) const
{
    if( !ShowConsistencyDialog< ADN_TableDialog >( name, consistencyTables_ ) &&
        !ShowConsistencyDialog< ADN_ListViewDialog >( name, consistencyListViews_ ) )
        throw MASA_EXCEPTION( "Invalid consistency table " + name.toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::IsNewBaseReadOnly
// Created: ABR 2013-09-12
// -----------------------------------------------------------------------------
bool ADN_MainWindow::IsNewBaseReadOnly( const tools::Path& filename ) const
{
    if( !filename.Exists() )
        return false;
    bool readOnly = false;
    tools::Xifstream xis( filename );
    xis >> xml::optional >> xml::start( "physical" )
        >> xml::optional >> xml::attribute( "read-only", readOnly );
    return readOnly;
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::mousePressEvent
// Created: ABR 2012-03-09
// -----------------------------------------------------------------------------
void ADN_MainWindow::mousePressEvent( QMouseEvent * event )
{
    if( event && event->button() == Qt::XButton1 )
    {
        actionBack_->trigger();
        event->accept();
    }
    else if( event && event->button() == Qt::XButton2 )
    {
        actionForward_->trigger();
        event->accept();
    }
    else
        QMainWindow::mousePressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::createPopupMenu
// Created: ABR 2013-09-12
// -----------------------------------------------------------------------------
QMenu* ADN_MainWindow::createPopupMenu()
{
    return 0; // $$$$ ABR 2013-09-12: Disable "windows" menu
}
