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
#include "ADN_Languages_Dialog.h"

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
    , menu_( 0 )
    , dialog_( new ADN_Languages_Dialog( data ) )
    , mapper_( 0 )
    , currentAction_( 0 )
    , masterAction_( 0 )
{
    mapper_ = new QSignalMapper( this );
    connect( mapper_, SIGNAL( mapped( const QString& ) ), this, SLOT( OnLanguageChanged( const QString& ) ) );
    connect( dialog_.get(), SIGNAL( LanguagesEdited() ), this, SLOT( OnLanguagesEdited() ) );
    connect( dialog_.get(), SIGNAL( MasterChanged() ), this, SLOT( OnMasterChanged() ) );
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
// Name: ADN_Languages_GUI::SetMenu
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::SetMenu( QMenu* menu )
{
    menu_ = menu;
    OnLanguagesEdited();
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::CreateMasterText
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
QString ADN_Languages_GUI::CreateMasterText() const
{
    return data_.Master().empty()
        ? tr( "(master)" )
        : QString( "%1 %2" ).arg( data_.GetAllLanguages().Get( data_.Master() )->GetName().c_str() ).arg( tr( "(master)" ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::OnLanguagesEdited
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::OnLanguagesEdited()
{
    if( !menu_ )
        throw MASA_EXCEPTION( "Language Menu not defined" );

    bool wasMaster = currentAction_ && currentAction_ == masterAction_;
    QString previousLanguage = currentAction_ ? currentAction_->text() : "";
    actions_.clear();
    menu_->clear();

    masterAction_ = AddToMenu( CreateMasterText(), menu_, mapper_ );
    masterAction_->setChecked( wasMaster );
    currentAction_ = 0;
    actions_.push_back( masterAction_ );

    for( auto it = data_.GetActiveLanguages().begin(); it != data_.GetActiveLanguages().end(); ++it )
    {
        QAction* action = AddToMenu( ( *it )->GetName().c_str(), menu_, mapper_ );
        if( ( *it )->GetName() == previousLanguage.toStdString() )
            currentAction_ = action;
        actions_.push_back( action );
    }

    if( currentAction_ )    // current action found, just check it
        currentAction_->setChecked( true );
    else                    // no current action (was master or a deleted language, it fall back to master anyway)
    {
        if( wasMaster )         // was master, just check it
        {
            currentAction_ = masterAction_;
            currentAction_->setChecked( true );
        }
        else                    // wasn't master, change language
            OnLanguageChanged( masterAction_->text() );
    }

    menu_->addSeparator();
    menu_->addAction( tr( "Edit..." ), dialog_.get(), SLOT( exec() ) );

    emit LanguagesEdited();
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::OnLanguageChanged
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::OnLanguageChanged( const QString& language )
{
    if( currentAction_ && currentAction_->text() == language )
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
        currentAction_ = masterAction_;
    currentAction_->setChecked( true );
    for( auto it = data_.GetActiveLanguages().begin(); it != data_.GetActiveLanguages().end(); ++it )
        if( ( *it )->GetName() == language.toStdString() )
        {
            ChangeLanguage( ( *it )->GetCode() );
            return;
        }
    ChangeLanguage( data_.Master() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::ChangeLanguage
// Created: ABR 2013-07-17
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::ChangeLanguage( const std::string& language )
{
    kernel::Language::SetCurrent( language );
    ADN_Workspace::GetWorkspace().SetIsSwappingLanguage( true );
    emit LanguageChanged();
    emit PostLanguageChanged();
    ADN_Workspace::GetWorkspace().SetIsSwappingLanguage( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::OnMasterChanged
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::OnMasterChanged()
{
    masterAction_->setText( CreateMasterText() );
}
