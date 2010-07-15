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
#include "clients_gui/Tools.h"
#include "Config.h"
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qsettings.h>
#include <qtextcodec.h>
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
OptionsPage::OptionsPage( QWidgetStack* pages, Page_ABC& previous, Config& config )
    : ContentPage( pages, tools::translate( "OptionsPage", "Options" ), previous, eButtonBack | eButtonQuit )
    , config_( config )
    , selectedLanguage_( ReadLang() )
{
    languages_[ tools::translate( "OptionsPage", "English" ) ]  = "en";
    languages_[ tools::translate( "OptionsPage", "Fran�ais" ) ] = "fr";

    QVBox* mainBox = new QVBox( this );
    mainBox->setBackgroundOrigin( QWidget::WindowOrigin );
    mainBox->setMargin( 10 );
    QGroupBox* box = new QGroupBox( 2, Qt::Horizontal, mainBox );
    box->setFrameShape( NoFrame );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    {
        QLabel* label = new QLabel( tools::translate( "OptionsPage", "Language: " ), box );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
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
        QLabel* label = new QLabel( tools::translate( "OptionsPage", "Data directory: " ), box );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        QHBox* hbox = new QHBox( box );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );
        dataDirectory_ = new QLineEdit( hbox );
        dataDirectory_->setText( config.GetRootDir().c_str() );
        QButton* browse = new QPushButton( tools::translate( "OptionsPage", "..." ), hbox );
        connect( browse, SIGNAL( clicked() ), SLOT( OnChangeDataDirectory() ) );
    }
    {
        QLabel* label = new QLabel( tools::translate( "OptionsPage", "Profile: " ), box );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        QComboBox* combo = new QComboBox( box );
        combo->insertItem( tools::translate( "OptionsPage", "Terrain" ), Config::eTerrain );
        combo->insertItem( tools::translate( "OptionsPage", "User" ), Config::eUser );
        combo->insertItem( tools::translate( "OptionsPage", "Advanced User" ), Config::eAdvancedUser );
        combo->insertItem( tools::translate( "OptionsPage", "Administrator" ), Config::eAdministrator );
        combo->setCurrentItem( config_.GetProfile() );
        connect( combo, SIGNAL( activated( int ) ), SLOT( OnChangeProfile( int ) ) );
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
    const QString directory = QFileDialog::getExistingDirectory( dataDirectory_->text(), this );
    if( directory.isEmpty() )
        return;
    dataDirectory_->setText( directory );
    Commit();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnChangeProfile
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void OptionsPage::OnChangeProfile( int index )
{
    config_.SetProfile( static_cast< Config::EProfile >( index ) );
}
