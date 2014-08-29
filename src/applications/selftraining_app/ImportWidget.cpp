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
#include "clients_gui/FileDialog.h"
#include "clients_kernel/Tools.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "tools/FileWrapper.h"
#include "tools/GeneralConfig.h"
#include "tools/VersionHelper.h"
#include <tools/Zip.h>
#include <xeumeuleu/xml.hpp>

namespace fc = frontend::commands;

// -----------------------------------------------------------------------------
// Name: ImportWidget constructor
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
ImportWidget::ImportWidget( QWidget* parent, const tools::GeneralConfig& config )
    : gui::WidgetLanguageObserver_ABC< QWidget >( parent )
    , config_        ( config )
    , isValidVersion_( false )
{
    //package
    packageLabel_ = new QLabel();
    package_ = new QLineEdit();
    browseButton_ = new QPushButton();
    connect( browseButton_, SIGNAL( clicked() ), SLOT( PackageBrowseClicked() ) );

    //Version
    versionLabel_ = new QLabel();
    packageVersion_ = new QLineEdit();
    packageVersion_->setReadOnly( true );

    //content
    contentLabel_ = new QLabel();
    contentLabel_->setAlignment( Qt::AlignTop );
    packageName_ = new QLineEdit();
    packageName_->setReadOnly( true );
    packageDescription_ = new QTextEdit();
    packageDescription_->setMaximumHeight( 80 );
    packageDescription_->setReadOnly( true );
    packageContent_ = new QListWidget();
    packageContent_->setFont( QFont( "Calibri", 12, QFont::Bold ) );

    //progress bar
    packageProgress_ = new QProgressBar();
    packageProgress_->hide();

    QGridLayout* mainLayout = new QGridLayout( this );
    mainLayout->addWidget( packageLabel_, 0, 0, 1, 1 );
    mainLayout->addWidget( package_, 0, 1, 1, 1 );
    mainLayout->addWidget( browseButton_, 0, 2, 1, 1 );
    mainLayout->addWidget( versionLabel_, 1, 0, 1, 1 );
    mainLayout->addWidget( packageVersion_, 1, 1, 1, 2 );
    mainLayout->addWidget( contentLabel_, 2, 0, 1, 1 );
    mainLayout->addWidget( packageName_, 2, 1, 1, 2 );
    mainLayout->addWidget( packageDescription_, 3, 1, 1, 2 );
    mainLayout->addWidget( packageContent_, 4, 1, 1, 2 );
    mainLayout->addWidget( packageProgress_, 5, 0, 1, 3 );
    mainLayout->setSizeConstraint( QLayout::SetMinAndMaxSize );

    mainLayout->setMargin( 0 );

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
        const tools::Path path = tools::Path::FromUnicode( package_->text().toStdWString() );
        packageProgress_->show();
        packageProgress_->setValue( 0 );
        packageProgress_->setMaximum( packageContent_->count() );
        setCursor( Qt::WaitCursor );
        tools::zip::InstallPackageFiles( path, config_.GetRootDir(), Progress( packageProgress_ ) );
        setCursor( Qt::ArrowCursor );
        packageProgress_->hide();
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
    try
    {
        tools::zip::ReadPackageContentFile(
            tools::Path::FromUnicode( package_->text().toStdWString() ),
            [&]( std::istream& s )
            {
                std::string name, description, version;
                xml::xistreamstream xis( s );
                xis >> xml::start( "content" )
                        >> xml::content( "name", name )
                        >> xml::content( "description", description )
                        >> xml::optional
                        >> xml::content( "version", version )
                    >> xml::end;
                packageName_->setText( name.c_str() );
                isValidVersion_ = tools::CheckVersion( version, tools::AppProjectVersion() );
                QPalette* palette = new QPalette();
                if( !isValidVersion_ )
                    palette->setColor( QPalette::Text, Qt::red );
                packageVersion_->setPalette( *palette );
                packageVersion_->setText( version.c_str() );
                packageDescription_->setText( description.c_str() );
            } );
    }
    catch( std::exception& )
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
    const tools::Path filename = gui::FileDialog::getOpenFileName( this, tools::translate( "ImportWidget", "Select a package" ), "", "Officer Training packages (*.otpak)" );
    SelectPackage( filename );
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::SelectPackage
// Created: SBO 2011-03-30
// -----------------------------------------------------------------------------
void ImportWidget::SelectPackage( const tools::Path& filename )
{
    package_->setText( filename.ToUTF8().c_str() );
    if( !filename.IsEmpty() )
    {
        packageName_->setText( "" );
        packageDescription_->setText( "" );
        packageVersion_->setText( "" );
        packageContent_->clear();
        if( ReadPackageContentFile() )
            tools::zip::ListPackageFiles( filename,
                [&]( const tools::Path& name )
                {
                    packageContent_->addItem( name.ToUTF8().c_str() );
                } );
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
    emit ButtonChanged( IsButtonEnabled(), tools::translate( "Page_ABC", "Import" ), false );
}
