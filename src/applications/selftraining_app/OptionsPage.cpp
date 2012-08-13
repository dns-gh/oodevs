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
OptionsPage::OptionsPage( QWidget* parent, Q3WidgetStack* pages, Page_ABC& previous, Config& config )
    : ContentPage( pages, previous, eButtonBack | eButtonApply )
    , parent_            ( parent )
    , config_            ( config )
    , selectedLanguage_  ( tools::readLang() )
    , selectedDataDir_   ( "" )
    , selectedProfile_   ( 0 )
    , hasChanged_        ( false )
    , languageHasChanged_( false )
{
    setName( "OptionsPage" );
    Q3VBox* mainBox = new Q3VBox( this );
    mainBox->setMargin( 10 );
    Q3GroupBox* box = new Q3GroupBox( 2, Qt::Horizontal, mainBox );
    box->setFrameShape( Q3GroupBox::NoFrame );
    // Language
    languageLabel_ = new QLabel( box );
    languageCombo_ = new QComboBox( box );
    languages_[ "en" ] = tools::translate( "OptionsPage", "English" );
    languages_[ "fr" ] = tools::translate( "OptionsPage", "French" );
    languages_[ "es" ] = tools::translate( "OptionsPage", "Spanish" );
    languages_[ "ar" ] = tools::translate( "OptionsPage", "Arabic" );
    BOOST_FOREACH( const T_Languages::value_type& lang, languages_ )
    {
        languageCombo_->insertItem( lang.second );
        if( lang.first == selectedLanguage_ )
            languageCombo_->setCurrentText( lang.second );
    }
    connect( languageCombo_, SIGNAL( activated( const QString& ) ), SLOT( OnChangeLanguage( const QString& ) ) );

    // Data
    dataLabel_ = new QLabel( box );
    Q3HBox* hbox = new Q3HBox( box );
    dataDirectory_ = new QLineEdit( hbox );
    dataButton_ = new QPushButton( hbox );
    connect( dataButton_, SIGNAL( clicked() ), SLOT( OnChangeDataDirectory() ) );
    connect( dataDirectory_, SIGNAL( textChanged( const QString& ) ), SLOT( OnEditDataDirectory( const QString& ) ) );
    // Profile
    profileLabel_ = new QLabel( box );
    profileCombo_ = new QComboBox( box );
    connect( profileCombo_, SIGNAL( activated( int ) ), SLOT( OnChangeProfile( int ) ) );
    AddContent( mainBox );

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
// Name: OptionsPage::OnLanguageChanged
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::OnLanguageChanged()
{
    // Title
    SetTitle( tools::translate( "OptionsPage", "Settings" ) );

    // Language
    languageLabel_->setText( tools::translate( "OptionsPage", "Language: " ) );

    // Data
    dataLabel_->setText( tools::translate( "OptionsPage", "Data directory: " ) );
    dataButton_->setText( tools::translate( "OptionsPage", "..." ) );

    // Profile
    profileLabel_->setText( tools::translate( "OptionsPage", "Profile: " ) );
    selectedProfile_ = ( profileCombo_->count() ) ? profileCombo_->currentIndex() : config_.GetProfile();
    profileCombo_->clear();
    profileCombo_->insertItem( tools::translate( "OptionsPage", "Terrain" ),       Config::eTerrain );
    profileCombo_->insertItem( tools::translate( "OptionsPage", "User" ),          Config::eUser );
    profileCombo_->insertItem( tools::translate( "OptionsPage", "Advanced User" ), Config::eAdvancedUser );
    profileCombo_->insertItem( tools::translate( "OptionsPage", "Administrator" ), Config::eAdministrator );
    profileCombo_->setCurrentItem( selectedProfile_ );

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
    EnableButton( eButtonApply, true );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnChangeDataDirectory
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
void OptionsPage::OnChangeDataDirectory()
{
    const QString directory = QDir::convertSeparators( Q3FileDialog::getExistingDirectory( dataDirectory_->text(), this ) );
    if( directory.isEmpty() )
        return;
    selectedDataDir_ = directory.toStdString();
    dataDirectory_->setText( directory );
    hasChanged_ = true;
    EnableButton( eButtonApply, true );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnEditDataDirectory
// Created: ABR 2011-11-25
// -----------------------------------------------------------------------------
void OptionsPage::OnEditDataDirectory( const QString& text )
{
    selectedDataDir_ = text.toStdString();
    hasChanged_ = true;
    EnableButton( eButtonApply, true );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnChangeProfile
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void OptionsPage::OnChangeProfile( int index )
{
    selectedProfile_ = index;
    hasChanged_ = true;
    EnableButton( eButtonApply, true );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnBack
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::OnBack()
{
    if( hasChanged_ )
    {
        MessageDialog message( parent_, tools::translate( "OptionsPage", "Unsaved changes" ), tools::translate( "OptionsPage", "Unsaved changes will be lost, continue anyway ?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
        {
            Reset();
            hasChanged_ = false;
            languageHasChanged_ = false;
            Page_ABC::OnBack();
        }
        else
            return;
    }
    else
        Page_ABC::OnBack();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::CreateDataDirectory
// Created: ABR 2011-11-25
// -----------------------------------------------------------------------------
void OptionsPage::CreateDataDirectory()
{
    const std::string directory = dataDirectory_->text().toStdString();
    if( !bfs::exists( directory ) )
    {
        MessageDialog message( parent_, tools::translate( "OptionsPage", "Invalid directory" ), tools::translate( "OptionsPage", "Directory \'%1\' doesn't exist. Do you want to create it ?" ).arg( directory.c_str() ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
        {
            try
            {
                selectedDataDir_ = directory;
                bfs::create_directories( std::string( directory ) );
            }
            catch ( std::exception& e )
            {
                selectedDataDir_ = config_.GetRootDir();
                dataDirectory_->setText( selectedDataDir_.c_str() );
                MessageDialog message( parent_, tools::translate( "OptionsPage", "Error" ), tools::translate( "OptionsPage", "Can't create directory \'%1\', error \'%2\' happen." ).arg( directory.c_str() ).arg( e.what() ), QMessageBox::Ok );
                message.exec();
                return;
            }
        }
        else
        {
            selectedDataDir_ = config_.GetRootDir();
            dataDirectory_->setText( selectedDataDir_.c_str() );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnApply
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::OnApply()
{
    CreateDataDirectory();
    assert( hasChanged_ );
    Application& application = static_cast< Application& >( *qApp );
    if( languageHasChanged_ )
        application.DeleteTranslators();
    Commit();
    if( languageHasChanged_ )
    {
        application.SetLocale();
        application.CreateTranslators();
        application.InitializeLayoutDirection();
    }
    languageHasChanged_ = false;
    hasChanged_ = false;
    EnableButton( eButtonApply, false );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::Commit
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::Commit()
{
    QSettings settings( "MASA Group", qApp->translate( "Application", "SWORD" ) );
    settings.writeEntry( "/Common/Language", selectedLanguage_.c_str() );
    settings.writeEntry( "/Common/DataDirectory", selectedDataDir_.c_str() );
    settings.writeEntry( "/Common/UserProfile", selectedProfile_ );

    static_cast< Application* >( qApp )->GetLauncher().SetRootDir( selectedDataDir_ );
    config_.SetRootDir( selectedDataDir_ );
    config_.SetProfile( static_cast< Config::EProfile >( selectedProfile_ ) );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::Reset
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::Reset()
{
    bool enableApplyButton = false;
    selectedLanguage_ = tools::readLang();
    selectedDataDir_ = config_.GetRootDir();
    selectedProfile_ = static_cast< int >( config_.GetProfile() );

    assert( languages_.find( selectedLanguage_ ) != languages_.end() );
    languageCombo_->setCurrentText( languages_.find( selectedLanguage_ )->second );
    dataDirectory_->setText( QDir::convertSeparators( selectedDataDir_.c_str() ) );
    profileCombo_->setCurrentIndex( selectedProfile_ );

    EnableButton( eButtonApply, enableApplyButton );
}
