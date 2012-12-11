// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ImportWidget.h"
#include "moc_ImportWidget.cpp"
#include "clients_gui/Tools.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "tools/GeneralConfig.h"
#include "tools/Version.h"
#include <xeumeuleu/xml.hpp>

#pragma warning( push, 0 )
#include <zipstream/zipstream.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: ImportWidget constructor
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
ImportWidget::ImportWidget( QWidget* parent, const tools::GeneralConfig& config )
    : gui::LanguageChangeObserver_ABC< Q3GroupBox >( 2, Qt::Vertical, parent )
    , config_        ( config )
    , isValidVersion_( false )
{
    setFrameShape( Q3GroupBox::DummyFrame::NoFrame );
    setMargin( 0 );
    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Horizontal, this );
    group->setFrameShape( Q3GroupBox::DummyFrame::NoFrame );
    {
        packageLabel_ = new QLabel( group );
        Q3HBox* hBox = new Q3HBox( group );
        package_ = new QLineEdit( hBox );
        browseButton_ = new QPushButton( hBox );
        connect( browseButton_, SIGNAL( clicked() ), SLOT( PackageBrowseClicked() ) );
    }
    {
        versionLabel_ = new QLabel( group );
        Q3HBox* hBox = new Q3HBox( group );
        packageVersion_ = new QLineEdit( hBox );
        packageVersion_->setReadOnly( true );
    }
    {
        contentLabel_ = new QLabel( group );
        contentLabel_->setAlignment( Qt::AlignTop );
        Q3VBox* vBox = new Q3VBox( group );
        vBox->setSpacing( 5 );
        packageName_ = new QLineEdit( vBox );
        packageName_->setReadOnly( true );
        packageDescription_ = new QTextEdit( vBox );
        packageDescription_->setMaximumHeight( 80 );
        packageDescription_->setReadOnly( true );
        packageContent_ = new QListWidget( vBox );
        packageContent_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    }
    packageProgress_ = new QProgressBar( this );
    packageProgress_->hide();
}

// -----------------------------------------------------------------------------
// Name: ImportWidget destructor
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
ImportWidget::~ImportWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void ImportWidget::OnLanguageChanged()
{
    packageLabel_->setText( tools::translate( "ImportWidget", "Package to be installed:" ) );
    contentLabel_->setText( tools::translate( "ImportWidget", "Package content:" ) );
    versionLabel_->setText( tools::translate( "ImportWidget", "Version:" ) );
    browseButton_->setText( tools::translate( "ImportWidget", "Browse..." ) );
}

namespace
{
    struct Progress
    {
        Progress( QProgressBar* progress ) : progress_( progress ), count_( 0 ) {}
        void operator()()
        {
            progress_->setValue( ++count_ );
            qApp->processEvents();
        }
        QProgressBar* progress_;
        unsigned count_;
    };
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::InstallExercise
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void ImportWidget::InstallExercise()
{
    if( packageContent_->count() )
    {
        zip::izipfile archive( package_->text() );
        if( archive.isOk() )
        {
            packageProgress_->show();
            packageProgress_->setValue( 0 );
            packageProgress_->setMaximum( packageContent_->count() );
            setCursor( Qt::waitCursor );
            frontend::commands::InstallPackageFile( archive, config_.GetRootDir(), Progress( packageProgress_ ) );
            setCursor( Qt::arrowCursor );
            packageProgress_->hide();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::IsButtonEnabled
// Created: JSR 2010-07-19
// -----------------------------------------------------------------------------
bool ImportWidget::IsButtonEnabled() const
{
    return isValidVersion_ && packageContent_->count() != 0;
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::ReadPackageContentFile
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
bool ImportWidget::ReadPackageContentFile()
{
    zip::izipfile archive( package_->text() );
    if( !archive.isOk() )
        return false;
    zip::izipstream zipStream( archive, "content.xml" );
    if( zipStream.bad() )
        return false;
    try
    {
        std::string name, description, version;
        xml::xistreamstream xis( zipStream );
        xis >> xml::start( "content" )
                >> xml::content( "name", name )
                >> xml::content( "description", description )
                >> xml::optional
                >> xml::content( "version", version )
            >> xml::end;
        packageName_->setText( name.c_str() );
        isValidVersion_ = tools::CheckVersion( version, tools::AppProjectVersion() ) || version.empty();
        QPalette* palette = new QPalette();
        if( !isValidVersion_ )
            palette->setColor( QPalette::Text, Qt::red );
        packageVersion_->setPalette( *palette );
        packageVersion_->setText( version.c_str() );
        packageDescription_->setText( description.c_str() );
    }
    catch( const xml::exception& )
    {
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::PackageBrowseClicked
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void ImportWidget::PackageBrowseClicked()
{
    const QString filename = QFileDialog::getOpenFileName( this, tools::translate( "ImportWidget", "Select a package" ), QString(), "Officer Training packages (*.otpak)" );
    SelectPackage( filename );
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::SelectPackage
// Created: SBO 2011-03-30
// -----------------------------------------------------------------------------
void ImportWidget::SelectPackage( const QString& filename )
{
    package_->setText( filename );
    if( !filename.isEmpty() )
    {
        packageName_->setText( "" );
        packageDescription_->setText( "" );
        packageVersion_->setText( "" );
        packageContent_->clear();
        if( ReadPackageContentFile() )
            packageContent_->addItems( frontend::commands::ListPackageFiles( package_->text().toStdString() ) );
        else
            packageName_->setText( tools::translate( "ImportWidget", "otpak corrupted: unable to load content properly" ) );
    }
    OnButtonChanged();
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::OnButtonChanged
// Created: BAX 2012-10-24
// -----------------------------------------------------------------------------
void ImportWidget::OnButtonChanged()
{
    emit ButtonChanged( IsButtonEnabled(), tools::translate( "Page_ABC", "Import" ) );
}