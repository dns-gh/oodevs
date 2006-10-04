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
#include "gaming/ASN_Messages.h"
#include "gaming/HumanFactors.h"
#include "gaming/MagicOrders.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"

#include "ENT/ENT_Tr.h"

using namespace kernel;
using namespace gui;

namespace 
{
    template< typename Enum, typename Combo, typename Converter >
    void Populate( Enum size, Combo& combo, Converter converter )
    {
        combo.Clear();
        for( unsigned int i = 0; i < (unsigned int)size; ++i )
        {
            const std::string& value = converter( (Enum)i, ENT_Tr_ABC::eToSim );
            combo.AddItem( value.c_str(), (Enum)i );
        }
    }
}
// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog constructor
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
ChangeHumanFactorsDialog::ChangeHumanFactorsDialog( QWidget* pParent, Controllers& controllers, Publisher_ABC& publisher )
    : QDialog( pParent, "Facteurs Humains" )
    , controllers_( controllers )
    , publisher_( publisher )
    , selected_( controllers )
{
    setCaption( "Facteurs humains" );
    QGridLayout* pLayout = new QGridLayout( this, 5, 2, 4 );
    pLayout->setRowStretch( 6, 1 );
    pLayout->setRowStretch( 2, 0 );
    
    pLayout->addWidget( new QLabel( tr( "Fatigue:" ), this ), 1, 0 );
    pTirednessCombo_ = new ValuedComboBox< E_UnitFatigue >( this );
    pLayout->addWidget( pTirednessCombo_, 1, 1 );

    pLayout->addWidget( new QLabel( tr( "Moral:" ), this ), 2, 0 );
    pMoralCombo_ = new ValuedComboBox< E_UnitMoral >( this );
    pLayout->addWidget( pMoralCombo_, 2, 1 );

    pLayout->addWidget( new QLabel( tr( "Experience:" ), this ), 3, 0 );
    pExperienceCombo_ = new ValuedComboBox< E_UnitExperience >( this );
    pLayout->addWidget( pExperienceCombo_, 3, 1 );

    pAllUnitsCheckBox_ = new QCheckBox( tr( "Toutes les unités" ), this );
    pLayout->addMultiCellWidget( pAllUnitsCheckBox_, 4, 4, 0, 1 );

    QHBox* box = new QHBox( this );
    QPushButton* okBtn = new QPushButton( tr( "OK" ), box );
    QPushButton* cancelBtn = new QPushButton( tr( "Annuler" ), box );
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
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::Show
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Show()
{
    Populate( eNbrUnitFatigue, *pTirednessCombo_, &ENT_Tr::ConvertFromUnitFatigue );
    Populate( eNbrUnitMoral, *pMoralCombo_, &ENT_Tr::ConvertFromUnitMoral );
    Populate( eNbrUnitExperience, *pExperienceCombo_, &ENT_Tr::ConvertFromUnitExperience );

    if( const HumanFactors_ABC* factors = selected_->Retrieve< HumanFactors_ABC >() )
    {
        factors->Display( *this );
        if ( selected_->GetAutomatType() )
            pAllUnitsCheckBox_->show();
        else
            pAllUnitsCheckBox_->hide();
        show();
    }
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::Call
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Call( const E_UnitFatigue& fatigue )
{
    pTirednessCombo_->SetCurrentItem( fatigue );
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::Call
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::Call( const E_UnitMoral& morale )
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
    const ASN1T_EnumUnitFatigue tiredness = (ASN1T_EnumUnitFatigue)pTirednessCombo_->GetValue();
    const ASN1T_EnumUnitMoral moral = (ASN1T_EnumUnitMoral)pMoralCombo_->GetValue();
    const ASN1T_EnumUnitExperience experience = (ASN1T_EnumUnitExperience)pExperienceCombo_->GetValue();
    if ( pAllUnitsCheckBox_->isChecked() )
    {
        Iterator< const Entity_ABC& > it = selected_->Get< CommunicationHierarchies >().CreateSubordinateIterator();
        while( it.HasMoreElements() )
            SendMessage( it.NextElement().GetId(), tiredness, moral, experience );
    }
    SendMessage( selected_->GetId(), tiredness, moral, experience );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ChangeHumanFactorsDialog::SendMessage
// Created: AGE 2005-09-22
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::SendMessage( uint id, ASN1T_EnumUnitFatigue tiredness, ASN1T_EnumUnitMoral moral, ASN1T_EnumUnitExperience experience )
{
    ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid = id;

    ASN1T_MagicActionChangeFacteursHumains asnMagicAction;

    asnMsg.GetAsnMsg().action.t                         = T_MsgUnitMagicAction_action_change_facteurs_humains;
    asnMsg.GetAsnMsg().action.u.change_facteurs_humains = &asnMagicAction;

    asnMagicAction.m.fatiguePresent = asnMagicAction.m.moralPresent = asnMagicAction.m.experiencePresent = 1;
    asnMagicAction.fatigue    = tiredness;
    asnMagicAction.moral      = moral;
    asnMagicAction.experience = experience;
    asnMsg.Send( publisher_ );
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
// Name: ChangeHumanFactorsDialog::NotifyContextMenu
// Created: SBO 2006-08-10
// -----------------------------------------------------------------------------
void ChangeHumanFactorsDialog::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    if( !agent.Retrieve< MagicOrders >() )
        return;
    selected_ = &agent;
    QPopupMenu* subMenu = menu.SubMenu( "Ordre", tr( "Ordres magiques" ) );
    subMenu->insertItem( tr( "Facteurs humains" ), this, SLOT( Show() ) );
}
