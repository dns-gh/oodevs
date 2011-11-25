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
#include "clients_gui/Tools.h"
#include "Config.h"
#include <boost/foreach.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

namespace bfs = boost::filesystem;

namespace
{
    std::string ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        std::string result = settings.readEntry( "/Common/Language", QTextCodec::locale() ).ascii();
        return ( result.empty() ) ? "en" : result;
    }
}

// -----------------------------------------------------------------------------
// Name: OptionsPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
OptionsPage::OptionsPage( QWidget* parent, Q3WidgetStack* pages, Page_ABC& previous, Config& config )
    : ContentPage( pages, previous, eButtonBack | eButtonApply )
    , parent_            ( parent )
    , config_            ( config )
    , selectedLanguage_  ( ReadLang() )
    , selectedDataDir_   ( "" )
    , hasChanged_        ( false )
    , languageHasChanged_( false )
{
    Q3VBox* mainBox = new Q3VBox( this );
    mainBox->setMargin( 10 );
    Q3GroupBox* box = new Q3GroupBox( 2, Qt::Horizontal, mainBox );
    box->setFrameShape( Q3GroupBox::NoFrame );
    // Language
    languageLabel_ = new QLabel( box );
    languageCombo_ = new QComboBox( box );
    languages_[ "en" ] = "English";
    languages_[ "fr" ] = "Français";
    languages_[ "es" ] = "Español";
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
    connect( dataDirectory_, SIGNAL( returnPressed() ), SLOT( OnEditDataDirectory() ) );

    AddContent( mainBox );
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
    selectedDataDir_ = directory.ascii();
    dataDirectory_->setText( directory );
    hasChanged_ = true;
    EnableButton( eButtonApply, true );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnEditDataDirectory
// Created: ABR 2011-11-15
// -----------------------------------------------------------------------------
void OptionsPage::OnEditDataDirectory()
{
    const std::string directory = dataDirectory_->text().ascii();
    if( !bfs::exists( directory ) )
    {
        MessageDialog message( parent_, tools::translate( "OptionsPage", "Invalid directory" ), tools::translate( "OptionsPage", "Directory \'%1\' doesn't exist. Do you want to create it ?" ).arg( directory.c_str() ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
        {
            try
            {
                bfs::create_directories( std::string( directory ) );
            }
            catch ( std::exception& e )
            {
                MessageDialog message( parent_, tools::translate( "OptionsPage", "Error" ), tools::translate( "OptionsPage", "Can't create directory \'%1\', error \'%2\' happen." ).arg( directory.c_str() ).arg( e.what() ), QMessageBox::Ok );
                message.exec();
                return;
            }
        }
        else
        {
            dataDirectory_->setText( selectedDataDir_.c_str() );
            return;
        }
    }
    selectedDataDir_ = directory;
    dataDirectory_->setText( selectedDataDir_.c_str() );
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
// Name: OptionsPage::OnApply
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::OnApply()
{
    assert( hasChanged_ );
    if( languageHasChanged_ )
        static_cast< Application* >( qApp )->DeleteTranslators();
    Commit();
    if( languageHasChanged_ )
        static_cast< Application* >( qApp )->CreateTranslators();
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
    QSettings settings;
    settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
    settings.writeEntry( "/Common/Language", selectedLanguage_.c_str() );
    settings.writeEntry( "/Common/DataDirectory", selectedDataDir_.c_str() );

    static_cast< Application* >( qApp )->SetLauncherRootDir( selectedDataDir_ );
    config_.SetRootDir( selectedDataDir_ );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::Reset
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void OptionsPage::Reset()
{
    selectedLanguage_ = ReadLang();
    selectedDataDir_ = config_.GetRootDir();

    assert( languages_.find( selectedLanguage_ ) != languages_.end() );
    languageCombo_->setCurrentText( languages_.find( selectedLanguage_ )->second );
    dataDirectory_->setText( QDir::convertSeparators( selectedDataDir_.c_str() ) );

    EnableButton( eButtonApply, false );
}
