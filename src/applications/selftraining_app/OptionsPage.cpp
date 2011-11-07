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
#include "clients_gui/Tools.h"
#include "Config.h"
#include <boost/foreach.hpp>

namespace
{
    QString ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }
}

// -----------------------------------------------------------------------------
// Name: OptionsPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
OptionsPage::OptionsPage( Q3WidgetStack* pages, Page_ABC& previous, Config& config )
    : ContentPage( pages, tools::translate( "OptionsPage", "Options" ), previous, eButtonBack | eButtonQuit )
    , config_( config )
    , selectedLanguage_( ReadLang() )
{
    languages_[ tools::translate( "OptionsPage", "English" ) ]  = "en";
    languages_[ tools::translate( "OptionsPage", "Français" ) ] = "fr";
    languages_[ tools::translate( "OptionsPage", "Español" ) ] = "es";

    Q3VBox* mainBox = new Q3VBox( this );
    mainBox->setMargin( 10 );
    Q3GroupBox* box = new Q3GroupBox( 2, Qt::Horizontal, mainBox );
    box->setFrameShape( Q3GroupBox::NoFrame );
    {
        new QLabel( tools::translate( "OptionsPage", "Language: " ), box );
        QComboBox* combo = new QComboBox( box );
        BOOST_FOREACH( const T_Languages::value_type& lang, languages_ )
        {
            combo->insertItem( lang.first );
            if( lang.second == selectedLanguage_ )
                combo->setCurrentText( lang.first );
        }
        connect( combo, SIGNAL( activated( const QString& ) ), SLOT( OnChangeLanguage( const QString& ) ) );
    }
    {
        new QLabel( tools::translate( "OptionsPage", "Data directory: " ), box );
        Q3HBox* hbox = new Q3HBox( box );
        dataDirectory_ = new QLineEdit( hbox );
        dataDirectory_->setText( QDir::convertSeparators( config.GetRootDir().c_str() ) );
        QPushButton* browse = new QPushButton( tools::translate( "OptionsPage", "..." ), hbox );
        connect( browse, SIGNAL( clicked() ), SLOT( OnChangeDataDirectory() ) );
    }
    AddContent( mainBox );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
OptionsPage::~OptionsPage()
{
    Commit();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnChangeLanguage
// Created: SBO 2009-04-08
// -----------------------------------------------------------------------------
void OptionsPage::OnChangeLanguage( const QString& lang )
{
    selectedLanguage_ = languages_[ lang ];
    Commit();
    static_cast< Application* >( qApp )->CreateTranslators();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::Commit
// Created: SBO 2009-04-08
// -----------------------------------------------------------------------------
void OptionsPage::Commit()
{
    QSettings settings;
    settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
    settings.writeEntry( "/Common/Language", selectedLanguage_.c_str() );
    settings.writeEntry( "/Common/DataDirectory", dataDirectory_->text() );
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
    dataDirectory_->setText( directory );
    Commit();
}
