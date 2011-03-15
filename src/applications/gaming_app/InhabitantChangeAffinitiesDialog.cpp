// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InhabitantChangeAffinitiesDialog.h"
#include "moc_InhabitantChangeAffinitiesDialog.cpp"

#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/UnitMagicAction.h"
#include "actions/ParameterList.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/tools.h"
#include "gaming/Affinities.h"
#include "gaming/StaticModel.h"
#include "gaming/TeamsModel.h"

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAffinitiesDialog constructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantChangeAffinitiesDialog::InhabitantChangeAffinitiesDialog( QWidget* pParent, kernel::Controllers& controllers, const TeamsModel& teams, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( pParent, tools::translate( "InhabitantChangeAffinitiesDialog", "Change affinities" ) )
    , controllers_   ( controllers )
    , teams_         ( teams )
    , static_        ( staticModel )
    , actionsModel_  ( actionsModel )
    , simulation_    ( simulation )
    , profile_       ( profile )
    , selected_      ( controllers )
    , affinitiesGrid_( 0 )
{
    setCaption( tools::translate( "InhabitantChangeAffinitiesDialog", "Change affinities" ) );
    resize( 320, 150 );
    mainLayout_ = new QVBoxLayout( this );
    buttonLayout_ = new QHBox( this );
    QPushButton* okButton     = new QPushButton( tr( "Ok" )    , buttonLayout_ );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), buttonLayout_ );
    okButton->setDefault( TRUE );
    connect( okButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    selected_ = 0;
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAffinitiesDialog destructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantChangeAffinitiesDialog::~InhabitantChangeAffinitiesDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAffinitiesDialog::Show
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAffinitiesDialog::Show()
{
    if( !selected_ )
        return;
    affinitiesSpinboxs_.clear();
    mainLayout_->remove( affinitiesGrid_ );
    mainLayout_->remove( buttonLayout_ );
    if( affinitiesGrid_ != 0 )
        delete affinitiesGrid_;
    affinitiesGrid_ = new QGrid( 2, this );
    mainLayout_->add( affinitiesGrid_ );
    mainLayout_->add( buttonLayout_ );
    selected_.ConstCast()->Get< Affinities >().CreateAffinitiesSpinBoxs( affinitiesGrid_, affinitiesSpinboxs_ );
    show();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAffinitiesDialog::Validate
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAffinitiesDialog::Validate()
{
    if( ! selected_ )
        return;
    accept();
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "inhabitant_change_affinities" );
    actions::UnitMagicAction* action = new actions::UnitMagicAction( *selected_, actionType, controllers_.controller_, tools::translate( "InhabitantChangeAffinitiesDialog", "Population Change Affinities" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    actions::parameters::ParameterList* affinitiesList = new actions::parameters::ParameterList( it.NextElement() );
    action->AddParameter( *affinitiesList );
    selected_->Get< Affinities >().FillParameterList( affinitiesList );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selected_, false ) );
    action->RegisterAndPublish( actionsModel_ );
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAffinitiesDialog::Reject
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAffinitiesDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAffinitiesDialog::closeEvent
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAffinitiesDialog::closeEvent( QCloseEvent * /* e */ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeAffinitiesDialog::NotifyContextMenu
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeAffinitiesDialog::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        QPopupMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tools::translate( "InhabitantChangeAffinitiesDialog", "Change affinities" ), this, SLOT( Show() ) );
    }
}
