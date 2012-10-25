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
    , hasChanged_        ( false )
    , languageHasChanged_( false )
{
    setObjectName( "OptionsPage" );

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
    QWidget* mainBox = new QWidget( pages );
    QHBoxLayout* mainLayout = new QHBoxLayout( mainBox );
    mainLayout->addLayout( titleLayout );
    mainLayout->addLayout( boxLayout );
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
    const QString directory = QDir::convertSeparators( QFileDialog::getExistingDirectory( this , "", dataDirectory_->text() ) );
    if( directory.isEmpty() )
        return;
    selectedDataDir_ = directory.toAscii().constData();
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
    selectedDataDir_ = text.toAscii().constData();
    hasChanged_ = true;
    EnableButton( eButtonApply, true );
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
// Name: OptionsPage::CreateDataDirectory
// Created: ABR 2011-11-25
// -----------------------------------------------------------------------------
void OptionsPage::CreateDataDirectory()
{
    const std::string directory = dataDirectory_->text().toAscii().constData();
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
    settings.setValue( "/Common/Language", selectedLanguage_.c_str() );
    settings.setValue( "/Common/DataDirectory", selectedDataDir_.c_str() );

    static_cast< Application* >( qApp )->GetLauncher().SetRootDir( selectedDataDir_ );
    config_.SetRootDir( selectedDataDir_ );
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

    assert( languages_.find( selectedLanguage_ ) != languages_.end() );
    languageCombo_->setCurrentIndex( languageCombo_->findText( languages_.find( selectedLanguage_ )->second ) );
    dataDirectory_->setText( QDir::convertSeparators( selectedDataDir_.c_str() ) );

    EnableButton( eButtonApply, enableApplyButton );
}
