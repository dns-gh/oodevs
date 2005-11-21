//*****************************************************************************
//
// $Created: JDY 03-06-19 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_MainWindow.cpp $
// $Author: Ape $
// $Modtime: 29/04/05 16:42 $
// $Revision: 30 $
// $Workfile: ADN_MainWindow.cpp $
//
//*****************************************************************************

#include "ADN_pch.h"
#include "ADN_MainWindow.h"
#include "moc_ADN_MainWindow.cpp"

#include "ADN_Workspace.h"
#include "ADN_Resources.h"
#include "ADN_Tools.h"
#include "ADN_Table.h"
#include "ADN_TableDialog.h"
#include "ADN_Enums.h"

#include "ADN_DataException.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_Xml_Exception.h"

#include "ADN_OpenMode_Dialog.h"

#include "ADN_Project_Data.h"

#include "ADN_ProgressBar.h"
#include "ADN_SplashScreen.h"

#include "qtundo.h"

#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qfiledialog.h>
#include <qtabwidget.h>
#include <qvbox.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qsettings.h>
#include <qwhatsthis.h>
#include <qtimer.h>
#include <qlayout.h>


//-----------------------------------------------------------------------------
// Name: ADN_MainWindow constructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_MainWindow::ADN_MainWindow()
: QMainWindow       ()
, workspace_        ( ADN_Workspace::GetWorkspace() )
, rIdSaveAs_        ( 0 )
, rIdClose_         ( 0 )
, nIdChangeOpenMode_( 0 )
, pProjectMenu_     ( 0 )
, pCoheranceTablesMenu_( 0 )
, pHelpMenu_        ( 0 )
, bNeedSave_        ( false )
, strAdminPassword_ ( "" )
{
    setCaption( tr( "Scipio Adaptation Tool - No Project" ) );
}


//-----------------------------------------------------------------------------
// Name: ADN_MainWindow destructor
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
ADN_MainWindow::~ADN_MainWindow()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::Build
// Created: JDY 03-06-23
//-----------------------------------------------------------------------------
void ADN_MainWindow::Build()
{
    ADN_SplashScreen splashScreen( QPixmap( "ADN.jpg" ) );
    splashScreen.show();
    workspace_.SetProgressIndicator( &splashScreen );

    // Main widget
    QVBox* pBox = new QVBox( this );
    setCentralWidget( pBox );
    pTab_ = new QTabWidget( pBox );
    pTab_->hide();

    // Actions
    QAction* pProjectNewAction = new QAction( MAKE_PIXMAP(filenew), tr("&New"), CTRL+Key_N, this, "new" );
    connect( pProjectNewAction, SIGNAL( activated() ) , this, SLOT( NewProject() ) );

    QAction* pProjectLoadAction = new QAction( MAKE_PIXMAP(fileopen), tr("&Open"), CTRL+Key_O, this, "open" );
    connect( pProjectLoadAction, SIGNAL( activated() ) , this, SLOT( OpenProject() ) );

    QAction* pProjectSaveAction = new QAction( MAKE_PIXMAP(filesave), tr("&Save"), CTRL+Key_S, this, "save" );
    connect( pProjectSaveAction, SIGNAL( activated() ) , this, SLOT( SaveProject() ) );
    pActionSave_=pProjectSaveAction;

// $$$ UNDO DISABLED
//    QAction* pUndoAction = QtUndoManager::manager()->createUndoAction( this );
//    pUndoAction->setAccel( QKeySequence( "Ctrl+Z" ) );

    // Menu & ToolBars

    // Project toolbar
    QToolBar * pToolBar = new QToolBar( this );
    pProjectNewAction->addTo( pToolBar );
    pProjectLoadAction->addTo( pToolBar );
    pProjectSaveAction->addTo( pToolBar );
// $$$ UNDO DISABLED
//    pToolBar->addSeparator();
//    pUndoAction->addTo( pToolBar );

    // Project menu
    pProjectMenu_ = new QPopupMenu( this );
    menuBar()->insertItem( tr("&Project"), pProjectMenu_ );
    pProjectNewAction->addTo( pProjectMenu_ );
    pProjectLoadAction->addTo( pProjectMenu_ );
    rIdClose_ = pProjectMenu_->insertItem( tr("&Close"),  this, SLOT(CloseProject()) );
    pProjectMenu_->insertSeparator(); 
    pProjectSaveAction->addTo( pProjectMenu_ );
    rIdSaveAs_ = pProjectMenu_->insertItem( tr("Save &As"),  this, SLOT(SaveAsProject()) );
                 pProjectMenu_->insertItem( tr("&Export HTML"),  this, SLOT(ExportHtml()) );
    pProjectMenu_->insertSeparator(); 
    pProjectMenu_->insertItem( tr("E&xit"),  this, SLOT(close()) );

    // Coherance tables menu
    pCoheranceTablesMenu_ = new QPopupMenu( this );
    menuBar()->insertItem( tr( "Consistency &tables" ), pCoheranceTablesMenu_ );

    // Help menu
    pHelpMenu_ = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&Help" ), pHelpMenu_ );
    pHelpMenu_->insertItem( tr( "&About" ), this, SLOT(About()), CTRL+Key_F1 );

    // Disable the menus.
    SetMenuEnabled( false );

    // Build all childs interfaces
    workspace_.Build( *this );

    // Status Bar
    QStatusBar* pStatus = statusBar();
    ADN_ProgressBar* pProgressBar = new ADN_ProgressBar( pStatus );
    pStatus->addWidget( pProgressBar, 0, true );
    workspace_.SetProgressIndicator( pProgressBar );

    connect( pCoheranceTablesMenu_, SIGNAL( activated( int ) ), this, SLOT( ShowCoheranceTable( int ) ) );
    connect( & workspace_.GetUndoStack(), SIGNAL( cleanChanged(bool) ), this, SLOT( ChangeSaveState(bool) ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::AddPage
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_MainWindow::AddPage( const QString& strPageName, QWidget& page )
{
    pTab_->addTab( &page, strPageName );
}


// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::AddTable
// Created: APE 2005-03-29
// -----------------------------------------------------------------------------
void ADN_MainWindow::AddTable( const QString& strTableName, ADN_Callback_ABC<ADN_Table*>* pCallback )
{
    vTableRegistrations_.push_back( T_TableRegistrationItem( strTableName, pCallback ) );
    pCoheranceTablesMenu_->insertItem( strTableName, vTableRegistrations_.size() - 1, -1 );
}


//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::SetMenuEnabled
// Created: JDY 03-06-24
//-----------------------------------------------------------------------------
void ADN_MainWindow::SetMenuEnabled( bool bEnabled )
{
    pActionSave_->setEnabled( bEnabled );
    pProjectMenu_->setItemEnabled( rIdClose_, bEnabled );
    pProjectMenu_->setItemEnabled( rIdSaveAs_, bEnabled );
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
        QApplication::restoreOverrideCursor();	// restore original cursor
        QMessageBox::critical( this, exception.GetExceptionTitle().c_str(), exception.GetExceptionMessage().c_str() );
        return;
    }

    QApplication::restoreOverrideCursor();	// restore original cursor

    if( ! bNoReadOnlyFiles )
        return; // we were not able to save all the datas

    std::string szProject = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() 
                         + workspace_.GetProject().GetData().GetFileInfos().GetFileName().GetData();
    setCaption( tr( "Scipio Adaptation Tool - " ) + szProject.c_str() );
}


//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::SaveAsProject
// Created: JDY 03-06-24
//-----------------------------------------------------------------------------
void ADN_MainWindow::SaveAsProject()
{
    QString strFileName = QFileDialog::getSaveFileName( QString::null, tr( "Scipio File (scipio.xml)" ) , this, "", tr( "Save project as" ) );
    if( strFileName == QString::null )
        return;
    
    QApplication::setOverrideCursor( waitCursor ); // this might take time

    try
    {
        workspace_.SaveAs( ADN_Tools::Replace(strFileName.ascii(),'\\','/') ); 
    }
    catch( ADN_Exception_ABC& exception )
    {
        QApplication::restoreOverrideCursor();	// restore original cursor
        QMessageBox::critical( this, exception.GetExceptionTitle().c_str(), exception.GetExceptionMessage().c_str() );
        return;
    }

    QApplication::restoreOverrideCursor();	// restore original cursor
    
    QString strCaption = tr( "Scipio Adaptation Tool - " ) + strFileName;
    setCaption( strCaption );
}


//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::NewProject
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_MainWindow::NewProject()
{
    QString qfilename=QFileDialog::getSaveFileName ( QString::null,tr("Scipio File (scipio.xml)") , this, "", tr("Create new project"));
    if (qfilename==QString::null)
        return;

    workspace_.SetOpenMode( eOpenMode_Admin );
    emit OpenModeToggled();

//    if( ! SelectOpenMode() )
//    return;

    SetMenuEnabled(false);
    pTab_->hide();

    workspace_.Reset(ADN_Tools::Replace(qfilename.ascii(),'\\','/'));

    SetMenuEnabled(true);
    pTab_->show();
    QString strCaption = tr( "Scipio Adaptation Tool - " ) + qfilename;
    setCaption( strCaption );
}


//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::OpenProject
// Created: JDY 03-06-19
//-----------------------------------------------------------------------------
void ADN_MainWindow::OpenProject()
{
  //  QString qfilename = "scipio.xml";

    QString qfilename=QFileDialog::getOpenFileName ( QString::null,tr("Scipio File (scipio.xml)") , this, "", tr("Open Scipio project"));
    if (qfilename==QString::null)
        return;
    this->OpenProject( qfilename );
}

// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::OpenProject
// Created: APE 2005-04-14
// -----------------------------------------------------------------------------
void ADN_MainWindow::OpenProject( const char* szFilename )
{
//    if( ! SelectOpenMode() )
//    return;

    workspace_.SetOpenMode( eOpenMode_Admin );
    emit OpenModeToggled();

    SetMenuEnabled( false );
    pTab_->hide();

    QApplication::setOverrideCursor( waitCursor ); // this might take time

    try
    {
        //workspace_.Load(ADN_Tools::Replace(szFilename,'\\','/'));  // $$$$ SBO 2005-11-18: Does not work on network
        if( szFilename[ 0 ] == '/' && szFilename[ 1 ] == '/' )
            workspace_.Load( ADN_Tools::Replace( szFilename, '/', '\\' ) );
        else
            workspace_.Load( szFilename );
    }
    catch( ADN_Exception_ABC& exception )
    {
        QApplication::restoreOverrideCursor();	// restore original cursor
        QMessageBox::critical( this, exception.GetExceptionTitle().c_str(), exception.GetExceptionMessage().c_str() );
        return;
    }

    QApplication::restoreOverrideCursor();	// restore original cursor

    QString strCaption = tr( "Scipio Adaptation Tool - " ) + szFilename;
    setCaption( strCaption );

    SetMenuEnabled(true);
    pTab_->show();
}


// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::ExportHtml
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_MainWindow::ExportHtml()
{
    QString strPath = QFileDialog::getExistingDirectory( ADN_Workspace::GetWorkspace().GetProject().GetData().GetWorkDirInfos().GetWorkingDirectory().GetData().c_str(), this );
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
    // $$$$ Disabled for ease of testing.
    /*
    if( ! this->OfferToSave() )
        return;
    */

    workspace_.Reset( ADN_Project_Data::FileInfos::szUntitled_ );
    pTab_->hide();
}



extern const char* szVersionNumber;


//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::About
// Created: JDY 03-09-11
//-----------------------------------------------------------------------------
void ADN_MainWindow::About()
{
    std::stringstream strMsg;
    strMsg << "Version : " << tr( "Scipio Adaptation Tool" ).ascii() << " " << szVersionNumber << std::endl << " " << std::endl;

    QMessageBox::about( this , tr( "Scipio Adaptation Tool" ), strMsg.str().c_str() );
}


//-----------------------------------------------------------------------------
// Name: ADN_MainWindow::closeEvent 
// Created: JDY 03-09-03
//-----------------------------------------------------------------------------
void ADN_MainWindow::closeEvent( QCloseEvent * e )
{
    e->accept();

    // $$$$ Disabled for ease of testing.
    /*
    if( this->OfferToSave() == true )
        e->accept();
    else
        e->ignore();
    */
}


// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::ChangeSaveState
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_MainWindow::ChangeSaveState( bool bNoCommand )
{
    bNeedSave_ = ! bNoCommand;
    if( bNeedSave_ )
    {
        setCaption( caption() + "*" );
    }
    else
    {
        std::string szProject = workspace_.GetProject().GetData().GetFileInfos().GetFileName().GetData();
        if( szProject == ADN_Project_Data::FileInfos::szUntitled_ )
            setCaption( tr( "Scipio Adaptation Tool - No Project" ) );
        else
        {
            szProject = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + szProject;
            setCaption( tr( "Scipio Adaptation Tool - " ) + szProject.c_str() );
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
    assert( nId < (int)vTableRegistrations_.size() );
    ADN_Callback_ABC<ADN_Table*>* pCallback = vTableRegistrations_[nId].second;

    ADN_Table* pTable = (*pCallback)();
    ADN_TableDialog* pDialog = new ADN_TableDialog( this, vTableRegistrations_[nId].first, pTable );

    pDialog->exec();
    delete pDialog;
}


// -----------------------------------------------------------------------------
// Name: ADN_MainWindow::OfferToSave
// Created: APE 2005-04-12
// -----------------------------------------------------------------------------
bool ADN_MainWindow::OfferToSave()
{
    if( ! bNeedSave_ )
        return true;

    QString strMessage = tr( "Save changes to project %1?" ).arg( workspace_.GetProject().GetData().GetFileInfos().GetFileNameFull().c_str() );
    int nResult = QMessageBox::information( this, tr( "Scipio Adaptation Tool" ), strMessage, QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel );
    switch( nResult )
    {
        case QMessageBox::Yes:
        {
            SaveProject();
            return true;
        }
        case QMessageBox::No:
        {
            return true;
        }
    }
    return false; // Cancel whatever action prompted the save proposition.
}
