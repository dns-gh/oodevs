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
#include "clients_gui/Tools.h"
#include "tools/GeneralConfig.h"
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
OptionsPage::OptionsPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config )
    : ContentPage( pages, tools::translate( "OptionsPage", "Options" ), previous )
    , selectedLanguage_( ReadLang() )
{
    languages_[ tools::translate( "OptionsPage", "English" ) ]  = "en";
    languages_[ tools::translate( "OptionsPage", "Français" ) ] = "fr";

    QVBox* mainBox = new QVBox( this );
    QGroupBox* box = new QGroupBox( 2, Qt::Horizontal, mainBox );
    box->setFlat( true );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setInsideSpacing( 10 );
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
}
