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
#include "ScenarioEditPage.h"
#include "clients_gui/Tools.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "tools/GeneralConfig.h"

#include <xeumeuleu/xml.hpp>

#pragma warning( push, 0 )
#include <zipstream/zipstream.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: ImportWidget constructor
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
ImportWidget::ImportWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config )
    : gui::LanguageChangeObserver_ABC< Q3GroupBox >( 2, Qt::Vertical, parent )
    , page_( page )
    , config_( config )
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
        contentLabel_ = new QLabel( group );
        contentLabel_->setAlignment( Qt::AlignTop );
        Q3VBox* vBox = new Q3VBox( group );
        vBox->setSpacing( 5 );
        packageName_ = new QLineEdit( vBox );
        packageName_->setReadOnly( true );
        packageDescription_ = new Q3TextEdit( vBox );
        packageDescription_->setMaximumHeight( 80 );
        packageDescription_->setReadOnly( true );
        packageContent_ = new Q3ListBox( vBox );
    }
    packageProgress_ = new Q3ProgressBar( this );
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
    browseButton_->setText( tools::translate( "ImportWidget", "Browse..." ) );
}

namespace
{
    struct Progress
    {
        Progress( Q3ProgressBar* progress ) : progress_( progress ), count_( 0 ) {}
        void operator()()
        {
            progress_->setProgress( ++count_ );
            qApp->processEvents();
        }
        Q3ProgressBar* progress_;
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
            packageProgress_->setProgress( 0, packageContent_->count() );
            setCursor( Qt::waitCursor );
            frontend::commands::InstallPackageFile( archive, config_.GetRootDir(), Progress( packageProgress_ ) );
            setCursor( Qt::arrowCursor );
            packageProgress_->hide();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::EnableEditButton
// Created: JSR 2010-07-19
// -----------------------------------------------------------------------------
bool ImportWidget::EnableEditButton()
{
    return packageContent_->count() != 0;
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
        std::string name, description;
        xml::xistreamstream xis( zipStream );
        xis >> xml::start( "content" )
                >> xml::content( "name", name )
                >> xml::content( "description", description )
            >> xml::end();
        packageName_->setText( name.c_str() );
        packageDescription_->setText( description.c_str() );
    }
    catch( xml::exception& )
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
    const QString filename = Q3FileDialog::getOpenFileName( "", "Officer Training packages (*.otpak)", this, "", tools::translate( "ImportWidget", "Select a package" ) );
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
        packageContent_->clear();
        if( ReadPackageContentFile() )
            packageContent_->insertStringList( frontend::commands::ListPackageFiles( package_->text().ascii() ) );
        else
            packageName_->setText( tools::translate( "ImportWidget", "otpak corrupted: unable to load content properly" ) );
    }
    page_.UpdateEditButton();
}
