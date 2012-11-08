// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
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
#include "clients_gui/Tools.h"
#include "Config.h"
#include "Launcher.h"
#include <boost/foreach.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#pragma warning( push, 1 )
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: OptionsPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
OptionsPage::OptionsPage( Application& app, QWidget* parent, Q3WidgetStack* pages,
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
    , selectedLanguage_  ( tools::readLang() )
    , hasChanged_        ( false )
    , languageHasChanged_( false )
{
    setObjectName( "OptionsPage" );
    setMargin( 5 );

    SetSettingsLayout();
    SetImportLayout();
    SetExportLayout();
    SetDataLayout();
    AddContent( tabs_ );

    connect( tabs_, SIGNAL( currentChanged( QWidget* ) ), SLOT( UpdateButton() ) );

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
    languages_[ "en" ] = tools::translate( "OptionsPage", "English" );
    languages_[ "fr" ] = tools::translate( "OptionsPage", "French" );
    languages_[ "es" ] = tools::translate( "OptionsPage", "Spanish" );
    languages_[ "ar" ] = tools::translate( "OptionsPage", "Arabic" );
    BOOST_FOREACH( const T_Languages::value_type& lang, languages_ )
    {
        languageCombo_->addItem( lang.second );
        if( lang.first == selectedLanguage_ )
            languageCombo_->setCurrentIndex( languageCombo_->findText( lang.second ) );
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
    export_ = new ExportWidget( parent_, config_, loader_, controllers_ );
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
    BOOST_FOREACH( const T_Languages::value_type& language, languages_ )
    {
        if( language.second == lang )
        {
            selectedLanguage_ = language.first;
            break;
        }
    }
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
    const QString directory = QDir::convertSeparators( QFileDialog::getExistingDirectory( this , "", dataDirectory_->text() ) );
    if( directory.isEmpty() )
        return;
    selectedDataDir_ = directory.toAscii().constData();
    dataDirectory_->setText( directory );
    hasChanged_ = true;
    UpdateButton();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnEditDataDirectory
// Created: ABR 2011-11-25
// -----------------------------------------------------------------------------
void OptionsPage::OnEditDataDirectory( const QString& text )
{
    selectedDataDir_ = text.toAscii().constData();
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
    switch( tabs_->currentPageIndex() )
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
    const std::string directory = dataDirectory_->text().toAscii().constData();
    if( bfs::exists( directory ) )
        return;

    MessageDialog message( parent_, tools::translate( "OptionsPage", "Invalid directory" ),
        tools::translate( "OptionsPage", "Directory \'%1\' doesn't exist. Do you want to create it ?" ).arg( directory.c_str() ),
        QMessageBox::Yes, QMessageBox::No );
    if( message.exec() != QMessageBox::Yes )
    {
        selectedDataDir_ = config_.GetRootDir();
        dataDirectory_->setText( selectedDataDir_.c_str() );
        return;
    }

    try
    {
        selectedDataDir_ = directory;
        bfs::create_directories( std::string( directory ) );
    }
    catch ( std::exception& e )
    {
        selectedDataDir_ = config_.GetRootDir();
        dataDirectory_->setText( selectedDataDir_.c_str() );
        MessageDialog message( parent_, tools::translate( "OptionsPage", "Error" ),
            tools::translate( "OptionsPage", "Can't create directory \'%1\', error \'%2\' happen." ).arg( directory.c_str() ).arg( e.what() ), QMessageBox::Ok );
        message.exec();
    }
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnApply
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::OnApply()
{
    switch( tabs_->currentPageIndex() )
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
    if( languageHasChanged_ )
        app_.DeleteTranslators();
    Commit();
    if( languageHasChanged_ )
    {
        app_.SetLocale();
        app_.CreateTranslators();
        app_.InitializeLayoutDirection();
    }
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
    settings.setValue( "/Common/DataDirectory", selectedDataDir_.c_str() );

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

    selectedLanguage_ = tools::readLang();
    selectedDataDir_ = config_.GetRootDir();

    assert( languages_.find( selectedLanguage_ ) != languages_.end() );
    languageCombo_->setCurrentIndex( languageCombo_->findText( languages_.find( selectedLanguage_ )->second ) );
    dataDirectory_->setText( QDir::convertSeparators( selectedDataDir_.c_str() ) );

    hasChanged_ = false;
    UpdateButton();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::ShowPackageInstallation
// Created: BAX 2012-10-24
// -----------------------------------------------------------------------------
void OptionsPage::ShowPackageInstallation( const QString& package )
{
    import_->SelectPackage( package );
    tabs_->showPage( import_ );
    QTimer::singleShot( 1, this, SLOT( show() ) );
}