// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChangeLogisticLinksDialog.h"
#include "moc_ChangeLogisticLinksDialog.cpp"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Identifier.h"
#include "actions/UnitMagicAction.h"
#include "gaming/LogisticLinks.h"
#include "gaming/StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Profile_ABC.h"
#include "protocol/SimulationSenders.h"
#include <qgrid.h>

using namespace actions;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog constructor
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
ChangeLogisticLinksDialog::ChangeLogisticLinksDialog( QWidget* parent, Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( parent )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation)
    , profile_( profile )
    , selected_( controllers )
    , selectedLevel_( &kernel::LogisticLevel::none_ )
{
    setCaption( tr( "Logistic links edition" ) );
    QVBoxLayout* layout = new QVBoxLayout( this );
    QGrid* grid = new QGrid( 3, Qt::Horizontal, this );
    layout->addWidget( grid );
    grid->setSpacing( 5 );
    grid->setMargin( 5 );

    QLabel* color = new QLabel( grid );
    color->setPixmap( QPixmap( 10, 10 ) );
    color->pixmap()->fill( QColor( "yellow" ).light( 150 ) );
    color->setMaximumWidth( 10 );
    QLabel* label = new QLabel( tr( "Nominal superior" ), grid );
    label->setMaximumWidth( 150 );
    nominalSuperiorCombo_ = new ValuedComboBox< const Entity_ABC* >( grid );
    nominalSuperiorCombo_->setMinimumWidth( 200 );

    color = new QLabel( grid );
    color->setPixmap( QPixmap( 10, 10 ) );
    color->pixmap()->fill( QColor( "yellow" ) );
    label = new QLabel( tr( "Current superior" ), grid );
    currentSuperiorCombo_ = new ValuedComboBox< const Entity_ABC* >( grid );

    QHBox* box = new QHBox( this );
    layout->addWidget( box );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), box );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), box );
    okButton->setDefault( true );

    connect( okButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );

    nominalSuperiorCombo_->AddItem( tr( "None" ), 0 );
    currentSuperiorCombo_->AddItem( tr( "None" ), 0 );

    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog destructor
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
ChangeLogisticLinksDialog::~ChangeLogisticLinksDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::Show
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::Show()
{
    if( !selected_ )
        return;
    const Entity_ABC& agent = *(selected_);

    const LogisticLinks* log = agent.Retrieve< LogisticLinks >();
    if( !log )
        return;

    nominalSuperiorCombo_->SetCurrentItem( log->GetNominalSuperior() );
    currentSuperiorCombo_->SetCurrentItem( log->GetCurrentSuperior() );
    show();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::NotifyCreated
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::NotifyCreated( const Automat_ABC& agent )
{
    if( agent.GetLogisticLevel() == kernel::LogisticLevel::none_ )
        return;
    nominalSuperiorCombo_->AddItem( agent.GetName(), &agent );
    currentSuperiorCombo_->AddItem( agent.GetName(), &agent );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::NotifyDeleted
// Created: SBO 2006-06-307
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::NotifyDeleted( const Automat_ABC& agent )
{
    if( agent.GetLogisticLevel() == kernel::LogisticLevel::none_ )
        return;
    nominalSuperiorCombo_->RemoveItem( &agent );
    currentSuperiorCombo_->RemoveItem( &agent );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::NotifyCreated
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::NotifyCreated( const Formation_ABC& agent )
{
    if( agent.GetLogisticLevel() == kernel::LogisticLevel::none_ )
        return;
    nominalSuperiorCombo_->AddItem( agent.GetName(), &agent );
    currentSuperiorCombo_->AddItem( agent.GetName(), &agent );
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::NotifyDeleted
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::NotifyDeleted( const Formation_ABC& agent )
{
    if( agent.GetLogisticLevel() == kernel::LogisticLevel::none_ )
        return;
    nominalSuperiorCombo_->RemoveItem( &agent );
    currentSuperiorCombo_->RemoveItem( &agent );
}

namespace
{
    template < typename T >
    actions::Parameter_ABC& Serialize( ValuedComboBox< const T* >& combo, const kernel::OrderParameter& orderParameter )
    {
        const T* tmp = combo.GetValue();
        if( tmp )
            return *new parameters::Identifier( orderParameter, tmp->GetId() );
        else
        {
            parameters::Identifier* newParam = new parameters::Identifier( orderParameter, 0 );
            newParam->Set( false );
            return *newParam;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::Validate()
{
    if( selected_ )
    {
        // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_logistic_links" );
        UnitMagicAction* action = new UnitMagicAction( *selected_, actionType, controllers_.controller_, tr( "Change Logistic Links"), true );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();

        if( nominalSuperiorCombo_->GetValue() )
            action->AddParameter( Serialize( *nominalSuperiorCombo_, it.NextElement() ) );
        if( currentSuperiorCombo_->GetValue() && currentSuperiorCombo_->GetValue() != nominalSuperiorCombo_->GetValue() )
            action->AddParameter( Serialize( *currentSuperiorCombo_, it.NextElement() ) );

        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( selected_, false ) );
        action->RegisterAndPublish( actionsModel_ );
    }
    Reject();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::Reject
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::Reject()
{
    selected_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::NotifyContextMenu
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::NotifyContextMenu( const Automat_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) && agent.Retrieve< LogisticLinks >() )
    {
        selected_ = (Entity_ABC*)&agent;
        selectedLevel_ = &agent.GetLogisticLevel();
        menu.InsertItem( "Command", tr( "Change logistic links" ), this, SLOT( Show() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::NotifyContextMenu
// Created: AHC 2010-10-12
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::NotifyContextMenu( const Formation_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) && agent.Retrieve< LogisticLinks >() )
    {
        selected_ = (Entity_ABC*)&agent;
        selectedLevel_ = &agent.GetLogisticLevel();
        menu.InsertItem( "Command", tr( "Change logistic links" ), this, SLOT( Show() ) );
    }
}
