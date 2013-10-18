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
#include "ADN_Languages_Dialog.h"
#include "ADN_Workspace.h"
#include "ADN_WorkspaceElement.h"
#include "clients_kernel/Context.h"
#include "clients_kernel/LanguageController.h"
#include "clients_Kernel/LocalizedString.h"
#include "clients_Kernel/Tools.h"
#include "tools/Languages.h"

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
        : QString( "%1 %2" ).arg( data_.GetAllLanguages().Get( data_.Master() ).GetName().c_str() ).arg( tr( "(master)" ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::BuildMenu
// Created: ABR 2013-10-04
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::BuildMenu()
{
    if( !menu_ )
        throw MASA_EXCEPTION( "Language Menu not defined" );
    actions_.clear();
    menu_->clear();

    masterAction_ = AddToMenu( CreateMasterText(), menu_, mapper_ );
    currentAction_ = masterAction_;
    actions_.push_back( masterAction_ );

    for( auto it = data_.GetActiveLanguages().begin(); it != data_.GetActiveLanguages().end(); ++it )
        actions_.push_back( AddToMenu( it->GetName().c_str(), menu_, mapper_ ) );

    swapSeparator_ = menu_->addSeparator();
    swapAction_ = menu_->addAction( tr( "Set current language as master" ), this, SLOT( OnSwap() ) );
    menu_->addSeparator();
    menu_->addAction( tr( "Edit..." ), dialog_.get(), SLOT( exec() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::UpdateMenu
// Created: ABR 2013-10-04
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::UpdateMenu()
{
    // uncheck all
    masterAction_->setChecked( false );
    for( auto it = actions_.begin(); it != actions_.end(); ++it )
        ( *it )->setChecked( false );
    // check current
    currentAction_->setChecked( true );
    // set swap actions visible
    swapSeparator_->setVisible( !data_.IsCurrentMaster() );
    swapAction_->setVisible( !data_.IsCurrentMaster() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::OnLanguagesEdited
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::OnLanguagesEdited()
{
    const QString previousLanguage = currentAction_ && currentAction_ != masterAction_ ? currentAction_->text() : "";
    BuildMenu();
    UpdateCurrentAction( previousLanguage );
    if( !previousLanguage.isEmpty() && previousLanguage != currentAction_->text() )
        OnLanguageChanged();
    else
        UpdateMenu();
    data_.InitializeLanguages();
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::UpdateCurrentAction
// Created: ABR 2013-10-04
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::UpdateCurrentAction( const QString& name )
{
    currentAction_ = masterAction_;
    for( auto it = actions_.begin(); it != actions_.end() && currentAction_ == masterAction_; ++it )
        if( ( *it )->text() == name )
            currentAction_ = *it;
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
    UpdateCurrentAction( language );
    for( auto it = data_.GetActiveLanguages().begin(); it != data_.GetActiveLanguages().end(); ++it )
        if( it->GetName() == language.toStdString() )
        {
            data_.ChangeLanguage( it->GetCode() );
            UpdateMenu();
            return;
        }
    data_.ChangeLanguage( data_.Master() );
    UpdateMenu();
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::OnMasterChanged
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::OnMasterChanged()
{
    masterAction_->setText( CreateMasterText() );
}

namespace
{
    bool Confirm( const QString& text )
    {
        return QMessageBox::Ok == QMessageBox::warning( 0, tools::translate( "ADN_Languages_GUI", "Warning" ),
                                                        text, QMessageBox::Ok, QMessageBox::Cancel | QMessageBox::Escape );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_GUI::OnSwap
// Created: ABR 2013-10-04
// -----------------------------------------------------------------------------
void ADN_Languages_GUI::OnSwap()
{
    if( data_.IsMasterEmpty() )
        throw MASA_EXCEPTION( "Not supposed to swap with an empty master" );
    const QString& current = data_.GetAllLanguages().Get( tools::Language::Current() ).GetName().c_str();
    const QString& master = data_.GetAllLanguages().Get( data_.Master() ).GetName().c_str();

    if( !Confirm( tr( "Are you sure you want to set the database master as '%1'?" ).arg( current ) ) )
        return;
    if( ADN_Workspace::GetWorkspace().ApplyOnData( boost::bind( &ADN_Data_ABC::ApplyOnTranslations, _1, boost::cref(
                                                   boost::bind( &kernel::LocalizedString::IsUnfinished, _1, tools::Language::Current() ) ) ) )
        && !Confirm( tr( "Some translations are empty or unfinished. If you continue you will have values in both "
                         "%1 and %2 as reference language, and may lost some unfinished translations. "
                         "Proceed anyway?" ).arg( current ).arg( master ) ) )
        return;
    data_.SwapMaster();
    BuildMenu();
    UpdateMenu();
}
