// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "InstallPackagePanel.h"
#include "moc_InstallPackagePanel.cpp"
#include "InfoBubble.h"
#include "resources.h"
#include "frontend/commands.h"
#include "frontend/Config.h"
#include "clients_gui/Tools.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#pragma warning( disable: 4996 )
#include <zipstream/zipstream.h>
#pragma warning( pop )

#include <qaction.h>
#include <qcursor.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qtextedit.h>
#include <qapplication.h>

#pragma warning( push, 0 )
#pragma warning( disable: 4127 4244 4245 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: InstallPackagePanel constructor
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
InstallPackagePanel::InstallPackagePanel( QWidgetStack* widget, QAction& action, const frontend::Config& config, ActionsContext& context )
    : Panel_ABC( widget, action, context, "InstallPackagePanel" )
    , config_( config )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, action.text(), box );
    {
        new QLabel( tr( "Package to be installed:" ), group );
        QHBox* hBox = new QHBox( group );
        package_ = new QLineEdit( hBox );
        QButton* browseBtn = new QPushButton( tr( "Browse..." ), hBox );
        connect( browseBtn, SIGNAL( clicked() ), SLOT( BrowseClicked() ) );
    }
    {
        QLabel* label = new QLabel( tr( "Package content:" ), group );
        label->setAlignment( Qt::AlignTop );
        QVBox* vBox = new QVBox( group );
        vBox->setSpacing( 5 );
        name_ = new QLineEdit( vBox );
        name_->setReadOnly( true );
        description_ = new QTextEdit( vBox );
        description_->setMaximumHeight( 80 );
        description_->setReadOnly( true );
        content_ = new QListBox( vBox );
    }
    progress_ = new QProgressBar( box );
    progress_->hide();

    bubble_ = new InfoBubble( box );
    QHBox* btnBox = new QHBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    okay_ = new QPushButton( MAKE_PIXMAP( next ), action.text(), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay_->setFont( font );

    connect( okay_, SIGNAL( pressed() ), SLOT( InstallPackage() ) );

    package_->setText( config_.GetPackageFile().c_str() );

    Update();
}

// -----------------------------------------------------------------------------
// Name: InstallPackagePanel destructor
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
InstallPackagePanel::~InstallPackagePanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InstallPackagePanel::BrowseClicked
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
void InstallPackagePanel::BrowseClicked()
{
    const QString filename = QFileDialog::getOpenFileName( "", "Officer Training packages (*.otpak)", this, "", tr( "Select a package" ) );
    package_->setText( filename );
    if( filename.isEmpty() )
        return;
    Update();
}

namespace
{
    bfs::path Normalize( bfs::path& p )
    {
        bfs::path result;
        for( bfs::path::iterator it = p.begin(); it != p.end(); ++it )
            if( *it == ".." ) // $$$$ SBO 2008-03-18: && !result.is_root()
                result.remove_leaf();
            else if( *it != "." )
                result /= *it;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: InstallPackagePanel::GetDestinationDirectory
// Created: SBO 2008-03-18
// -----------------------------------------------------------------------------
std::string InstallPackagePanel::GetDestinationDirectory() const
{
    return config_.GetRootDir();
}

namespace
{
    struct Progress
    {
        Progress( QProgressBar* progress ) : progress_( progress ), count_( 0 ) {}
        void operator()()
        {
            progress_->setProgress( ++count_ );
            qApp->processEvents();
        }
        QProgressBar* progress_;
        unsigned count_;
    };
}

// -----------------------------------------------------------------------------
// Name: InstallPackagePanel::InstallPackage
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
void InstallPackagePanel::InstallPackage()
{
    if( content_->count() )
    {
        zip::izipfile archive( package_->text() );
        if( archive.isOk() )
        {
            progress_->show();
            progress_->setProgress( 0, content_->count() );
            setCursor( QCursor::waitCursor );
            okay_->setDisabled( true );
            frontend::commands::InstallPackageFile( archive, GetDestinationDirectory(), Progress( progress_ ) );
            setCursor( QCursor::arrowCursor );
            okay_->setDisabled( false );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: InstallPackagePanel::Update
// Created: SBO 2008-03-14
// -----------------------------------------------------------------------------
void InstallPackagePanel::Update()
{
    name_->setText( "" );
    description_->setText( "" );
    content_->clear();
    if( ReadContentFile() )
    {
        okay_->setDisabled( false );
        bubble_->ShowInfo( tr( "Click \"Install package\" button to install the content of the package.\nPackage will be installed to: %1" ).arg( GetDestinationDirectory().c_str() ) );
        content_->insertStringList( frontend::commands::ListPackageFiles( package_->text().ascii() ) );
    }
    else
    {
        okay_->setDisabled( true );
        bubble_->ShowError( tr( "Selected file is not a valid %1 package." ).arg( tools::translate( "Application", "SWORD" ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: InstallPackagePanel::ReadContentFile
// Created: SBO 2008-03-17
// -----------------------------------------------------------------------------
bool InstallPackagePanel::ReadContentFile()
{
    zip::izipfile archive( package_->text() );
    if( !archive.isOk() )
        return false;
    zip::izipstream zipStream( archive, "content.xml" );
    if( zipStream.bad() )
        return false;
    try
    {
        std::string name, description;
        xml::xistreamstream xis( zipStream );
        xis >> xml::start( "content" )
                >> xml::content( "name", name )
                >> xml::content( "description", description )
            >> xml::end;
        name_->setText( name.c_str() );
        description_->setText( description.c_str() );
    }
    catch( xml::exception& e )
    {
        bubble_->ShowError( e.what() );
        return false;
    }
    return true;
}
