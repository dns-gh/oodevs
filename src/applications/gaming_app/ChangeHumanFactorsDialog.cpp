// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChangeHumanFactorsDialog.h"
#include "moc_ChangeHumanFactorsDialog.cpp"
#include "gaming/HumanFactors.h"
#include "gaming/tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Profile_ABC.h"
#include "protocol/ServerPublisher_ABC.h"
#include "protocol/simulationsenders.h"

using namespace kernel;
using namespace gui;

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
ChangeHumanFactorsDialog::ChangeHumanFactorsDialog( QWidget* pParent, Controllers& controllers, Publisher_ABC& publisher, const Profile_ABC& profile )
    : QDialog( pParent, tr( "Human factors" ) )
    , controllers_( controllers )
    , publisher_( publisher )
    , profile_( profile )
    , selected_( controllers )
{
    setCaption( tr( "Human factors" ) );
    QGridLayout* pLayout = new QGridLayout( this, 5, 2, 4 );
    pLayout->setRowStretch( 6, 1 );
    pLayout->setRowStretch( 2, 0 );
    
    pLayout->addWidget( new QLabel( tr( "Tiredness:" ), this ), 1, 0 );
    pTirednessCombo_ = new ValuedComboBox< E_UnitTiredness >( this );
    pLayout->addWidget( pTirednessCombo_, 1, 1 );

    pLayout->addWidget( new QLabel( tr( "Moral:" ), this ), 2, 0 );
    pMoralCombo_ = new ValuedComboBox< E_UnitMorale >( this );
    pLayout->addWidget( pMoralCombo_, 2, 1 );

    pLayout->addWidget( new QLabel( tr( "Experience:" ), this ), 3, 0 );
    pExperienceCombo_ = new ValuedComboBox< E_UnitExperience >( this );
    pLayout->addWidget( pExperienceCombo_, 3, 1 );

    QHBox* box = new QHBox( this );
    QPushButton* okBtn = new QPushButton( tr( "Ok" ), box );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );
    okBtn->setDefault( true );
    pLayout->addMultiCellWidget( box, 4, 4, 0, 1, Qt::AlignCenter );

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
    Populate( eNbrUnitTiredness,  *pTirednessCombo_ );
    Populate( eNbrUnitMorale,     *pMoralCombo_ );
    Populate( eNbrUnitExperience, *pExperienceCombo_ );

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
// Name: ChangeHumanFactorsDialog::Validate
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Validate()
{
    if( ! selected_ )
        return;
    const EnumUnitTiredness tiredness = (EnumUnitTiredness)pTirednessCombo_->GetValue();
    const EnumUnitMorale moral = (EnumUnitMorale)pMoralCombo_->GetValue();
    const EnumUnitExperience experience = (EnumUnitExperience)pExperienceCombo_->GetValue();
    tools::Iterator< const Entity_ABC& > it = selected_->Get< CommunicationHierarchies >().CreateSubordinateIterator();
    SendMessage( *selected_, tiredness, moral, experience );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::SendMessage
// Created: AGE 2006-12-01
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::SendMessage( const kernel::Entity_ABC& entity, EnumUnitTiredness tiredness, EnumUnitMorale moral, EnumUnitExperience experience )
{
    if( entity.Retrieve< HumanFactors_ABC >() )
        SendMessage( entity.GetId(), tiredness, moral, experience );
    const Hierarchies* h = entity.Retrieve< CommunicationHierarchies >();
    if( !h )
        h = entity.Retrieve< TacticalHierarchies >();
    if( h )
    {
        tools::Iterator< const Entity_ABC& > it = h->CreateSubordinateIterator();
        while( it.HasMoreElements() )
            SendMessage( it.NextElement(), tiredness, moral, experience );
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::SendMessage
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::SendMessage( unsigned int id, EnumUnitTiredness tiredness, EnumUnitMorale moral, EnumUnitExperience experience )
{
    simulation::UnitMagicAction message;
    message().set_oid( id );

    Common::MsgMagicActionChangeHumanFactors magicAction;
    *message().mutable_action()->mutable_change_facteurs_humains() = magicAction;     // $$$$ _RC_ FDS 2010-01-27: Pas tr�s clair pour moi ???
 
    magicAction.set_fatigue    ( tiredness );
    magicAction.set_moral      ( moral );
    magicAction.set_experience ( experience );
    message.Send( publisher_ );
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
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        QPopupMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tr( "Human factors" ), this, SLOT( Show() ) );
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
