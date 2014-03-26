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

#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Identifier.h"
#include "actions/UnitMagicAction.h"
#include "gaming/LogisticLinks.h"
#include "gaming/StaticModel.h"
#include "clients_gui/LogisticBase.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Profile_ABC.h"
#include "protocol/SimulationSenders.h"

#pragma warning( push, 0 )
#include <Qt3Support/q3grid.h>
#pragma warning( pop )

using namespace actions;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog constructor
// Created: SBO 2006-06-30
// -----------------------------------------------------------------------------
ChangeLogisticLinksDialog::ChangeLogisticLinksDialog( QWidget* parent, Controllers& controllers, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel, const Time_ABC& simulation, const Profile_ABC& profile )
    : QDialog( parent )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , static_( staticModel )
    , simulation_( simulation )
    , profile_( profile )
    , selected_( controllers )
{
    setCaption( tr( "Logistic links edition" ) );
    Q3VBoxLayout* layout = new Q3VBoxLayout( this );
    Q3Grid* grid = new Q3Grid( 3, Qt::Horizontal, this );
    layout->addWidget( grid );
    grid->setSpacing( 5 );
    grid->setMargin( 5 );

    QLabel* color = new QLabel( grid );
    QPixmap tmp( 10 , 10 );
    tmp.fill( QColor( "yellow" ) );
    color->setPixmap( tmp );
    color->setMaximumWidth( 10 );
    QLabel* label = new QLabel( tr( "Nominal superior" ), grid );
    label->setMaximumWidth( 150 );
    nominalSuperiorCombo_ = new ValuedComboBox< const Entity_ABC* >( "nominalSuperiorCombo", grid );
    nominalSuperiorCombo_->setMinimumWidth( 200 );

    color = new QLabel( grid );
    tmp.fill( QColor( "yellow" ).lighter( 150 ) );
    color->setPixmap( tmp );
    label = new QLabel( tr( "Current superior" ), grid );
    currentSuperiorCombo_ = new ValuedComboBox< const Entity_ABC* >( "currentSuperiorCombo", grid );

    Q3HBox* box = new Q3HBox( this );
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
    nominalSuperiorCombo_->RemoveItem( selected_ );
    currentSuperiorCombo_->RemoveItem( selected_ );
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
    if( !agent.Get< LogisticBase >().IsBase() )
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
    if( !agent.Get< LogisticBase >().IsBase() )
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
    if( !agent.Get< LogisticBase >().IsBase() )
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
    if( !agent.Get< LogisticBase >().IsBase() )
        return;
    nominalSuperiorCombo_->RemoveItem( &agent );
    currentSuperiorCombo_->RemoveItem( &agent );
}

namespace
{
    template < typename T >
    actions::Parameter_ABC& Serialize( ValuedComboBox< const T* >& combo, const OrderParameter& orderParameter )
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
    auto selected = selected_;
    Reject();
    if( selected )
    {
        // $$$$ _RC_ SBO 2010-05-17: use ActionFactory
        MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_logistic_links" );
        std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
        tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();

        action->AddParameter( Serialize( *nominalSuperiorCombo_, it.NextElement() ) );
        action->AddParameter( Serialize( *currentSuperiorCombo_, it.NextElement() ) );

        action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new ActionTasker( controllers_.controller_, selected, false ) );
        actionsModel_.Publish( *action );
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeLogisticLinksDialog::Reject
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void ChangeLogisticLinksDialog::Reject()
{
    nominalSuperiorCombo_->AddItem( selected_->GetName(), selected_ );
    currentSuperiorCombo_->AddItem( selected_->GetName(), selected_ );
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
        menu.InsertItem( "Command", tr( "Change logistic links" ), this, SLOT( Show() ) );
    }
}
