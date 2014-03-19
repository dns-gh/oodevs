// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ExtensionsPanel.h"
#include "moc_ExtensionsPanel.cpp"

#include "actions/ActionsModel.h"
#include "actions/ActionTiming.h"
#include "actions/ActionTasker.h"
#include "actions/ParameterList.h"
#include "actions/UnitMagicAction.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/DiffusionListDialog.h"
#include "clients_gui/RichGroupBox.h"
#include "gaming/AgentsModel.h"
#include "gaming/DictionaryExtensions.h"
#include "gaming/Model.h"
#include "gaming/StaticModel.h"
#include "gaming/TeamsModel.h"

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel constructor
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
ExtensionsPanel::ExtensionsPanel( QMainWindow* parent, kernel::Controllers& controllers, const Model& model, const StaticModel& staticModel, const kernel::Time_ABC& simulation )
    : gui::ExtensionsPanel( parent, controllers, staticModel.extensions_, model.agents_, model.teams_ )
    , actions_        ( model.actions_ )
    , types_          ( staticModel.types_ )
    , simulation_     ( simulation )
    , hasChanged_     ( false )
    , activatedBackup_( false )
{
    QHBoxLayout* buttons = new QHBoxLayout;
    pMainLayout_->addLayout( buttons );
    validateButton_ = new QPushButton( tr( "Validate" ) );
    validateButton_->setObjectName( "ExtensionsPanel_Button_Validate" );
    resetButton_ = new QPushButton( tr( "Reset" ) );
    resetButton_->setObjectName( "ExtensionsPanel_Button_Reset" );
    buttons->addWidget( validateButton_ );
    buttons->addWidget( resetButton_ );
    connect( validateButton_, SIGNAL( clicked() ), SLOT( OnValidate() ) );
    connect( resetButton_, SIGNAL( clicked() ), SLOT( OnReset() ) );
    diffusionDialog_->SetContextMenuEntry( false );
    ChangeButtonsState( false );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel destructor
// Created: ABR 2011-05-12
// -----------------------------------------------------------------------------
ExtensionsPanel::~ExtensionsPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::NotifySelected
// Created: ABR 2011-05-16
// -----------------------------------------------------------------------------
void ExtensionsPanel::NotifySelected( const kernel::Entity_ABC* element )
{
    if( element == selected_ )
        return;
    Reset();
    gui::ExtensionsPanel::NotifySelected( element );
    ChangeButtonsState( false );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::SetReadOnly
// Created: NPT 2013-02-28
// -----------------------------------------------------------------------------
void ExtensionsPanel::SetReadOnly( bool readOnly ) const
{
    validateButton_->setVisible( !readOnly );
    resetButton_->setVisible( !readOnly );
    diffusionDialog_->setEnabled( !readOnly );
    if( pGroupBox_ )
    {
        pGroupBox_->setEnabled( !readOnly );
        pGroupBox_->setCheckable( !readOnly );
    }
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::OnActivationChanged
// Created: ABR 2011-05-16
// -----------------------------------------------------------------------------
void ExtensionsPanel::OnActivationChanged( bool state )
{
    UpdateBackUp();
    gui::ExtensionsPanel::OnActivationChanged( state );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::OnValidate
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
void ExtensionsPanel::OnValidate()
{
    if( !selected_ )
        return;
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( types_ ).Get( "change_extension" );
    std::unique_ptr< actions::Action_ABC > action( new actions::UnitMagicAction( actionType, controllers_.controller_, false ) );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    actions::parameters::ParameterList* extensions = new actions::parameters::ParameterList( it.NextElement() );
    action->AddParameter( *extensions );
    static_cast< const DictionaryExtensions& >( selected_->Get< kernel::DictionaryExtensions >() ).FillParameterList( extensions );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( controllers_.controller_, selected_, false ) );
    actions_.Publish( *action );
    ChangeButtonsState( false );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::Reset
// Created: ABR 2011-05-16
// -----------------------------------------------------------------------------
void ExtensionsPanel::Reset()
{
    if( !selected_ || !hasChanged_ )
        return;
    kernel::DictionaryExtensions* dico = const_cast< kernel::DictionaryExtensions* >( selected_->Retrieve< kernel::DictionaryExtensions >() );
    if( !dico )
        return;
    for( DictionaryExtensions::CIT_Extensions it = backup_.begin(); it != backup_.end(); ++it )
        dico->SetValue( it->first, it->second );
    dico->SetEnabled( activatedBackup_ );
    backup_.clear();
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::OnReset
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
void ExtensionsPanel::OnReset()
{
    Reset();
    UpdateDisplay();
    ChangeButtonsState( false );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::Commit
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
void ExtensionsPanel::Commit()
{
    UpdateBackUp();
    gui::ExtensionsPanel::Commit();
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::ChangeButtonsState
// Created: ABR 2011-05-13
// -----------------------------------------------------------------------------
void ExtensionsPanel::ChangeButtonsState( bool state )
{
    hasChanged_ = state;
    if( IsReadOnly() )
        return;
    validateButton_->setEnabled( state );
    resetButton_->setEnabled( state );
}

// -----------------------------------------------------------------------------
// Name: ExtensionsPanel::UpdateBackUp
// Created: ABR 2011-05-17
// -----------------------------------------------------------------------------
void ExtensionsPanel::UpdateBackUp()
{
    if( selected_ && !hasChanged_ )
    {
        backup_.clear();
        const kernel::DictionaryExtensions* dico = selected_->Retrieve< kernel::DictionaryExtensions >();
        if( !dico )
            return;
        activatedBackup_ = dico->IsEnabled();
        backup_ = dico->GetExtensions();
        ChangeButtonsState( true );
    }
}
