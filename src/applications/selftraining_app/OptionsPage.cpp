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
#include <qcombobox.h>
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
OptionsPage::OptionsPage( QWidgetStack* pages, Page_ABC& previous )
    : ContentPage( pages, tools::translate( "OptionsPage", "Options" ), previous )
    , selectedLanguage_( ReadLang() )
{
    languages_[ tools::translate( "OptionsPage", "English" ) ]  = "en";
    languages_[ tools::translate( "OptionsPage", "Français" ) ] = "fr";

    QVBox* mainBox = new QVBox( this );
    {
        QHBox* hbox = new QHBox( mainBox );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );
        hbox->setSpacing( 10 );
        {
            QLabel* label = new QLabel( tools::translate( "OptionsPage", "Language: " ), hbox );
            label->setBackgroundOrigin( QWidget::WindowOrigin );
            QComboBox* combo = new QComboBox( hbox );
            BOOST_FOREACH( const T_Languages::value_type& lang, languages_ )
            {
                combo->insertItem( lang.first );
                if( lang.second == selectedLanguage_ )
                    combo->setCurrentText( lang.first );
            }
            connect( combo, SIGNAL( activated( const QString& ) ), SLOT( OnChangeLanguage( const QString& ) ) );
        }
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
}
