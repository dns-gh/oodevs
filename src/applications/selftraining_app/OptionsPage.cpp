// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "OptionsPage.h"
#include "moc_OptionsPage.cpp"
#include "Application.h"
#include "MessageDialog.h"
#include "DataWidget.h"
#include "ImportWidget.h"
#include "ExportWidget.h"
#include "Config.h"
#include "Launcher.h"
#include "clients_gui/FileDialog.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/LanguageController.h"
#include "clients_kernel/Tools.h"
#include "tools/Languages.h"
#include <boost/foreach.hpp>
#pragma warning( push, 1 )
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: OptionsPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
OptionsPage::OptionsPage( Application& app, QWidget* parent, QStackedWidget* pages,
                          Page_ABC& previous, Config& config,
                          const tools::Loader_ABC& loader, kernel::Controllers& controllers,
                          frontend::LauncherClient& launcher )
    : LauncherClientPage( pages, previous, eButtonBack | eButtonApply, launcher )
    , app_               ( app )
    , parent_            ( parent )
    , tabs_              ( new QTabWidget() )
    , config_            ( config )
    , loader_            ( loader )
    , controllers_       ( controllers )
    , selectedLanguage_  ( tools::Language::Current() )
    , hasChanged_        ( false )
    , languageHasChanged_( false )
{
    setObjectName( "OptionsPage" );
    layout()->setMargin( 5 );

    SetSettingsLayout();
    SetImportLayout();
    SetExportLayout();
    SetDataLayout();
    AddContent( tabs_ );

    connect( tabs_, SIGNAL( currentChanged( int ) ), SLOT( UpdateButton() ) );

    // Init data
    Reset();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
OptionsPage::~OptionsPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::SetSettingsLayout
// Created: BAX 2012-10-24
// -----------------------------------------------------------------------------
void OptionsPage::SetSettingsLayout()
{
    // Language
    languageLabel_ = new QLabel();
    languageLabel_->setFixedHeight( 25 );
    languageCombo_ = new QComboBox();

    for( auto it = config_.GetLanguages().GetVector().begin(); it != config_.GetLanguages().GetVector().end(); ++it )
        if( it->IsSupported() )
        {
            languageCombo_->addItem( it->GetName().c_str() );
            if( it->GetCode() == selectedLanguage_ )
                languageCombo_->setCurrentIndex( languageCombo_->findText( it->GetName().c_str() ) );
        }
    connect( languageCombo_, SIGNAL( activated( const QString& ) ), SLOT( OnChangeLanguage( const QString& ) ) );

    // Data
    dataLabel_ = new QLabel();
    dataLabel_->setFixedHeight( 25 );

    dataDirectory_ = new QLineEdit();
    dataButton_ = new QPushButton();
    connect( dataButton_, SIGNAL( clicked() ), SLOT( OnChangeDataDirectory() ) );
    connect( dataDirectory_, SIGNAL( textChanged( const QString& ) ), SLOT( OnEditDataDirectory( const QString& ) ) );

    //titles layout
    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->addWidget( languageLabel_ );
    titleLayout->addWidget( dataLabel_ );
    titleLayout->addStretch();

    //box layout
    QHBoxLayout* directoryLayout = new QHBoxLayout();
    directoryLayout->addWidget( dataDirectory_ );
    directoryLayout->addWidget( dataButton_ );

    QVBoxLayout* boxLayout = new QVBoxLayout();
    boxLayout->addWidget( languageCombo_ );
    boxLayout->addLayout( directoryLayout );
    boxLayout->addStretch();

    // main layout
    QWidget* mainBox = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout( mainBox );
    mainLayout->addLayout( titleLayout );
    mainLayout->addLayout( boxLayout );

    tabs_->addTab( mainBox, "" );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::SetImportLayout
// Created: BAX 2012-10-24
// -----------------------------------------------------------------------------
void OptionsPage::SetImportLayout()
{
    import_ = new ImportWidget( parent_, config_ );
    tabs_->addTab( import_, "" );
    connect( import_, SIGNAL( ButtonChanged( bool, const QString& ) ),
             SLOT( OnButtonChanged( bool, const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::SetExportLayout
// Created: BAX 2012-10-24
// -----------------------------------------------------------------------------
void OptionsPage::SetExportLayout()
{
    export_ = new ExportWidget( app_, parent_, config_, loader_, controllers_ );
    tabs_->addTab( export_, "" );
    connect( export_, SIGNAL( ButtonChanged( bool, const QString& ) ),
             SLOT( OnButtonChanged( bool, const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::SetModelsLayout
// Created: BAX 2012-10-24
// -----------------------------------------------------------------------------
void OptionsPage::SetDataLayout()
{
    data_ = new DataWidget( parent_, tabs_, config_ );
    connect( data_, SIGNAL( ButtonChanged( bool, const QString& ) ),
             SLOT( OnButtonChanged( bool, const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnLanguageChanged
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::OnLanguageChanged()
{
    // Title
    SetTitle( tools::translate( "OptionsPage", "Administration" ) );

    // tabs
    tabs_->setTabText( eTabs_Settings, tools::translate( "OptionsPage", "Settings" ) );
    tabs_->setTabText( eTabs_Import,   tools::translate( "OptionsPage", "Import" ) );
    tabs_->setTabText( eTabs_Export,   tools::translate( "OptionsPage", "Export" ) );
    tabs_->setTabText( eTabs_Terrains, tools::translate( "OptionsPage", "Terrains" ) );
    tabs_->setTabText( eTabs_Models,   tools::translate( "OptionsPage", "Models" ) );

    // settings
    languageLabel_->setText( tools::translate( "OptionsPage", "Language: " ) );
    dataLabel_->setText( tools::translate( "OptionsPage", "Data directory: " ) );
    dataButton_->setText( tools::translate( "OptionsPage", "..." ) );

    // Parent
    ContentPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnChangeLanguage
// Created: SBO 2009-04-08
// -----------------------------------------------------------------------------
void OptionsPage::OnChangeLanguage( const QString& lang )
{
    for( auto it = config_.GetLanguages().GetVector().begin(); it != config_.GetLanguages().GetVector().end(); ++it )
        if( it->GetName() == lang.toStdString() )
            selectedLanguage_ = it->GetCode();
    hasChanged_ = true;
    languageHasChanged_ = true;
    UpdateButton();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnChangeDataDirectory
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
void OptionsPage::OnChangeDataDirectory()
{
    const tools::Path directory = gui::FileDialog::getExistingDirectory( this , "", tools::Path::FromUnicode( dataDirectory_->text().toStdWString() ) );
    if( directory.IsEmpty() )
        return;
    selectedDataDir_ = directory;
    dataDirectory_->setText( directory.ToUTF8().c_str() );
    hasChanged_ = true;
    UpdateButton();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnEditDataDirectory
// Created: ABR 2011-11-25
// -----------------------------------------------------------------------------
void OptionsPage::OnEditDataDirectory( const QString& text )
{
    selectedDataDir_ = tools::Path::FromUnicode( text.toStdWString() );
    hasChanged_ = true;
    UpdateButton();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnBack
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::OnBack()
{
    if( !hasChanged_ )
    {
        Page_ABC::OnBack();
        return;
    }

    MessageDialog message( parent_, tools::translate( "OptionsPage", "Unsaved changes" ),
        tools::translate( "OptionsPage", "Unsaved changes will be lost, continue anyway ?" ),
        QMessageBox::Yes, QMessageBox::No );
    if( message.exec() != QMessageBox::Yes )
        return;

    Reset();
    hasChanged_ = false;
    languageHasChanged_ = false;
    Page_ABC::OnBack();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::OnButtonChanged
// Created: BAX 2012-24-10
// -----------------------------------------------------------------------------
void OptionsPage::OnButtonChanged( bool enabled, const QString& text )
{
    EnableButton ( eButtonApply, enabled );
    SetButtonText( eButtonApply, text );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::UpdateButton
// Created: BAX 2012-10-24
// -----------------------------------------------------------------------------
void OptionsPage::UpdateButton()
{
    switch( tabs_->currentIndex() )
    {
        default:
            break;

        case eTabs_Settings:
            EnableButton ( eButtonApply, hasChanged_ );
            SetButtonText( eButtonApply, tools::translate( "Page_ABC", "Apply" ) );
            break;

        case eTabs_Import:
            import_->OnButtonChanged();
            break;

        case eTabs_Export:
            export_->OnButtonChanged();
            break;

        case eTabs_Models:
        case eTabs_Terrains:
            data_->OnButtonChanged();
            break;
    }
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::Reconnect
// Created: BAX 2012-10-25
// -----------------------------------------------------------------------------
void OptionsPage::Reconnect()
{
    Connect( "localhost", config_.GetLauncherPort() );
    export_->Update();
    data_->Update();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::CreateDataDirectory
// Created: ABR 2011-11-25
// -----------------------------------------------------------------------------
void OptionsPage::CreateDataDirectory()
{
    const tools::Path directory = tools::Path::FromUnicode( dataDirectory_->text().toStdWString() );
    if( directory.Exists() )
        return;

    MessageDialog message( parent_, tools::translate( "OptionsPage", "Invalid directory" ),
        tools::translate( "OptionsPage", "Directory \'%1\' doesn't exist. Do you want to create it ?" ).arg( directory.ToUTF8().c_str() ),
        QMessageBox::Yes, QMessageBox::No );
    if( message.exec() != QMessageBox::Yes )
    {
        selectedDataDir_ = config_.GetRootDir();
        dataDirectory_->setText( selectedDataDir_.ToUTF8().c_str() );
        return;
    }

    try
    {
        selectedDataDir_ = directory;
        directory.CreateDirectories();
    }
    catch( const std::exception& e )
    {
        selectedDataDir_ = config_.GetRootDir();
        dataDirectory_->setText( selectedDataDir_.ToUTF8().c_str() );
        MessageDialog message( parent_, tools::translate( "OptionsPage", "Error" ),
            tools::translate( "OptionsPage", "Can't create directory \'%1\', error \'%2\' happen." ).arg( directory.ToUTF8().c_str() ).arg( tools::GetExceptionMsg( e ).c_str() ), QMessageBox::Ok );
        message.exec();
    }
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnApply
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::OnApply()
{
    switch( tabs_->currentIndex() )
    {
        default:
            break;

        case eTabs_Settings:
            ApplyAction();
            Reconnect();
            break;

        case eTabs_Import:
            import_->InstallExercise();
            Reconnect();
            break;

        case eTabs_Export:
            export_->ExportPackage();
            break;

        case eTabs_Models:
        case eTabs_Terrains:
            data_->OnDelete();
            Reconnect();
            break;
    }
    UpdateButton();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::ApplyAction
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::ApplyAction()
{
    CreateDataDirectory();
    assert( hasChanged_ );
    Commit();
    if( languageHasChanged_ )
        controllers_.languages_.ChangeLanguage( selectedLanguage_.c_str() );
    languageHasChanged_ = false;
    hasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::Commit
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::Commit()
{
    QSettings settings( "MASA Group", "SWORD" );
    settings.setValue( "/Common/Language", selectedLanguage_.c_str() );
    settings.setValue( "/Common/DataDirectory", selectedDataDir_.ToUTF8().c_str() );

    app_.GetLauncher().SetRootDir( selectedDataDir_ );
    config_.SetRootDir( selectedDataDir_ );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::Reset
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::Reset()
{
    Reconnect();

    selectedLanguage_ = tools::Language::Current();
    selectedDataDir_ = config_.GetRootDir();
    languageCombo_->setCurrentIndex( languageCombo_->findText( config_.GetLanguages().Get( selectedLanguage_ ).GetName().c_str() ) );
    dataDirectory_->setText( selectedDataDir_.ToUTF8().c_str() );

    hasChanged_ = false;
    UpdateButton();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::ShowPackageInstallation
// Created: BAX 2012-10-24
// -----------------------------------------------------------------------------
void OptionsPage::ShowPackageInstallation( const tools::Path& package )
{
    import_->SelectPackage( package );
    tabs_->setCurrentIndex( tabs_->indexOf( import_ ) );
    QTimer::singleShot( 1, this, SLOT( show() ) );
}