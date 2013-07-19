// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Languages_GUI.h"
#include "moc_ADN_Languages_GUI.cpp"
#include "ADN_Languages_Data.h"
#include "clients_kernel/Language.h"

namespace
{
    QAction* AddToMenu( const QString& text, QMenu* menu, QSignalMapper* mapper )
    {
        QAction* action = new QAction( text, menu );
        action->setCheckable( true );
        QObject::connect( action, SIGNAL( triggered() ), mapper, SLOT( map() ) );
        mapper->setMapping( action, text );
        menu->addAction( action );
        return action;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI constructor
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
ADN_Languages_GUI::ADN_Languages_GUI( ADN_Languages_Data& data )
    : ADN_GUI_ABC( eLanguages )
    , data_( data )
    , mapper_( 0 )
    , menu_( 0 )
    , currentAction_( 0 )
    , defaultAction_( 0 )
    , currentLanguage_( kernel::Language::default_ )
{
    mapper_ = new QSignalMapper( this );
    connect( mapper_, SIGNAL( mapped( const QString& ) ), this, SLOT( OnLanguageChanged( const QString& ) ) );
    connect( this, SIGNAL( LanguageChanged( const std::string& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnLanguageChanged( const std::string& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI destructor
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
ADN_Languages_GUI::~ADN_Languages_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::Build
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::Build()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::CreateMenu
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
QMenu* ADN_Languages_GUI::CreateMenu( QWidget* parent )
{
    menu_ = new QMenu( tr( "Languages" ), parent );
    defaultAction_ = AddToMenu( tr( "Default" ), menu_, mapper_ );
    defaultAction_->setChecked( true );
    actions_.push_back( defaultAction_ );
    currentAction_ = defaultAction_;
    return menu_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::FillMenu
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::FillMenu()
{
    const std::string currentLanguageShort = tools::readLang();
    std::string currentLanguage = kernel::Language::default_;

    for( auto it = data_.languages_.begin(); it != data_.languages_.end(); ++it )
    {
        actions_.push_back( AddToMenu( it->GetName().c_str(), menu_, mapper_ ) );
        if( it->GetShortName() == currentLanguageShort )
            currentLanguage = it->GetName();
    }

    menu_->addSeparator();
    QAction* editAction = menu_->addAction( tr( "Edit..." ), this, SLOT( OnEditLanguages() ) );
    editAction->setEnabled( false );

    OnLanguageChanged( currentLanguage.c_str() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::OnLanguageChanged
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::OnLanguageChanged( const QString& language )
{
    assert( currentAction_ && defaultAction_ );

    if( currentAction_->text() == language )
    {
        currentAction_->setChecked( true );
        return;
    }

    currentAction_ = 0;
    for( auto it = actions_.begin(); it != actions_.end(); ++it )
        if( ( *it )->text() == language )
            currentAction_ = *it;
        else
            ( *it )->setChecked( false );

    if( currentAction_ == 0 )
        currentAction_ = defaultAction_;

    currentAction_->setChecked( true );
    for( auto it = data_.languages_.begin(); it != data_.languages_.end(); ++it )
        if( it->GetName() == language.toStdString() )
        {
            ChangeLanguage( it->GetShortName() );
            return;
        }
    ChangeLanguage( kernel::Language::default_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::GetCurrentLanguage
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
const std::string& ADN_Languages_GUI::GetCurrentLanguage() const
{
    return currentLanguage_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::ChangeLanguage
// Created: ABR 2013-07-17
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::ChangeLanguage( const std::string& language )
{
    currentLanguage_ = language;
    emit LanguageChanged( currentLanguage_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::OnEditLanguages
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::OnEditLanguages()
{
    // $$$$ ABR 2013-07-08: Do something here to edit languages
}
