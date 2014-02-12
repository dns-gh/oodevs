// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChangeHumanFactorsDialog.h"
#include "moc_ChangeHumanFactorsDialog.cpp"
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Enumeration.h"
#include "actions/UnitMagicAction.h"
#include "gaming/HumanFactors.h"
#include "gaming/StaticModel.h"
#include "gaming/Troops.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Profile_ABC.h"

using namespace kernel;
using namespace gui;
using namespace actions;

namespace
{
    template< typename Enum, typename Combo >
    void Populate( Enum size, Combo& combo )
    {
        combo.Clear();
        for( unsigned int i = 0; i < unsigned int( size ); ++i )
            combo.AddItem( tools::ToString( (Enum)i ), (Enum)i );
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog constructor
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
ChangeHumanFactorsDialog::ChangeHumanFactorsDialog( QWidget* pParent, Controllers& controllers, const ::StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const Profile_ABC& profile )
: QDialog( pParent, tr( "Human factors" ) )
    , controllers_ ( controllers )
    , static_      ( staticModel )
    , actionsModel_( actionsModel )
    , simulation_  ( simulation )
    , profile_     ( profile )
    , selected_    ( controllers )
{
    setCaption( tr( "Human factors" ) );
    QGridLayout* pLayout = new QGridLayout( this, 6, 2, 4 );
    pLayout->setRowStretch( 6, 1 );
    pLayout->setRowStretch( 2, 0 );
    pLayout->addWidget( new QLabel( tr( "Tiredness:" ), this ), 1, 0 );
    pTirednessCombo_ = new ValuedComboBox< E_UnitTiredness >( "TirednessCombo", this );
    pLayout->addWidget( pTirednessCombo_, 1, 1 );
    pLayout->addWidget( new QLabel( tr( "Moral:" ), this ), 2, 0 );
    pMoralCombo_ = new ValuedComboBox< E_UnitMorale >( "MoralCombo", this );
    pLayout->addWidget( pMoralCombo_, 2, 1 );
    pLayout->addWidget( new QLabel( tr( "Experience:" ), this ), 3, 0 );
    pExperienceCombo_ = new ValuedComboBox< E_UnitExperience >( "ExperienceCombo", this );
    pLayout->addWidget( pExperienceCombo_, 3, 1 );
    pLayout->addWidget( new QLabel( tr( "Stress:" ), this ), 4, 0 );
    pStressCombo_ = new ValuedComboBox< E_UnitStress >( "StressCombo", this );
    pLayout->addWidget( pStressCombo_, 4, 1 );
    Q3HBox* box = new Q3HBox( this );
    QPushButton* okBtn = new QPushButton( tr( "Ok" ), box );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );
    okBtn->setDefault( true );
    pLayout->addMultiCellWidget( box, 5, 5, 0, 1, Qt::AlignCenter );
    connect( okBtn, SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( hide() ) );
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog destructor
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
ChangeHumanFactorsDialog::~ChangeHumanFactorsDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::Show
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Show()
{
    Populate( eNbrUnitTiredness, *pTirednessCombo_ );
    Populate( eNbrUnitMorale, *pMoralCombo_ );
    Populate( eNbrUnitExperience, *pExperienceCombo_ );
    Populate( eNbrUnitStress, *pStressCombo_ );
    if( const HumanFactors_ABC* factors = selected_->Retrieve< HumanFactors_ABC >() )
        factors->Display( *this );
    show();
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::Call
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Call( const E_UnitTiredness& fatigue )
{
    pTirednessCombo_->SetCurrentItem( fatigue );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::Call
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Call( const E_UnitMorale& morale )
{
    pMoralCombo_->SetCurrentItem( morale );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::Call
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Call( const E_UnitExperience& experience )
{
    pExperienceCombo_->SetCurrentItem( experience );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::Call
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Call( const E_UnitStress& stress )
{
    pStressCombo_->SetCurrentItem( stress );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::Validate
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Validate()
{
    if( ! selected_ )
        return;
    const E_UnitTiredness tiredness = pTirednessCombo_->GetValue();
    const E_UnitMorale moral = pMoralCombo_->GetValue();
    const E_UnitExperience experience = pExperienceCombo_->GetValue();
    const E_UnitStress stress = pStressCombo_->GetValue();
    tools::Iterator< const Entity_ABC& > it = selected_->Get< TacticalHierarchies >().CreateSubordinateIterator();
    SendMessage( *selected_, tiredness, moral, experience, stress );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::SendMessage
// Created: AGE 2006-12-01
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::SendMessage( const kernel::Entity_ABC& entity, E_UnitTiredness tiredness, E_UnitMorale moral, E_UnitExperience experience, E_UnitStress stress )
{
    if( entity.Retrieve< HumanFactors_ABC >() )
        SendAction( entity, tiredness, moral, experience, stress );
    const Hierarchies* h = entity.Retrieve< CommunicationHierarchies >();
    if( !h )
        h = entity.Retrieve< TacticalHierarchies >();
    if( h )
    {
        tools::Iterator< const Entity_ABC& > it = h->CreateSubordinateIterator();
        while( it.HasMoreElements() )
            SendMessage( it.NextElement(), tiredness, moral, experience, stress );
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::SendAction
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::SendAction( const kernel::Entity_ABC& entity, E_UnitTiredness tiredness, E_UnitMorale moral, E_UnitExperience experience, E_UnitStress stress )
{
    // $$$$ _RC_ SBO 2010-05-17: user ActionFactory
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_human_factors" );
    std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Enumeration( it.NextElement(), tiredness ) );
    action->AddParameter( *new parameters::Enumeration( it.NextElement(), moral ) );
    action->AddParameter( *new parameters::Enumeration( it.NextElement(), experience ) );
    action->AddParameter( *new parameters::Enumeration( it.NextElement(), stress ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new ActionTasker( controllers_.controller_, &entity, false ) );
    actionsModel_.Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::sizeHint
// Created: SBO 2006-08-09
// -----------------------------------------------------------------------------
QSize ChangeHumanFactorsDialog::sizeHint() const
{
    return QSize( 240, 120 );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::DoContextMenu
// Created: SBO 2006-08-10
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::DoContextMenu( const kernel::Entity_ABC& entity, ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) && entity.Retrieve< Troops >() )
    {
        selected_ = &entity;
        kernel::ContextMenu* subMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ), false, 1 );
        int id = subMenu->insertItem( tr( "Human factors" ), this, SLOT( Show() ) );
        bool isAutonomous = entity.Get< Troops >().GetTotalHumans() == 0;
        subMenu->setItemEnabled( id, !isAutonomous );
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::NotifyContextMenu
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    DoContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::NotifyContextMenu
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    DoContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::NotifyContextMenu
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    DoContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::NotifyContextMenu
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu )
{
    DoContextMenu( entity, menu );
}
