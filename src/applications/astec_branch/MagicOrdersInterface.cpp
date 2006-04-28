// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "MagicOrdersInterface.h"
#include "moc_MagicOrdersInterface.cpp"
#include "Controllers.h"
#include "Agent.h"
#include "MagicOrders.h"
#include "OptionVariant.h"
#include "ASN_Messages.h"

//#include "LogisticSupplyRecompletionDialog.h"
//#include "ChangeHumanFactorsDialog.h"

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface constructor
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
MagicOrdersInterface::MagicOrdersInterface( QWidget* parent, Controllers& controllers )
    : QObject( parent )
    , controllers_( controllers )
    , selectedAgent_( controllers )
    , controller_( false )
{
//    supplyRecompletion_ = new LogisticSupplyRecompletionDialog( parent );
//    changeHumanFactors_ = new ChangeHumanFactorsDialog( parent );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface destructor
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
MagicOrdersInterface::~MagicOrdersInterface()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::NotifyContextMenu
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::NotifyContextMenu( const Agent& agent, QPopupMenu& menu )
{
    if( !controller_ )
        return;

    selectedAgent_ = &agent;
    const MagicOrders& orders = agent.Get< MagicOrders >();
    QPopupMenu* magicMenu = new QPopupMenu( &menu );

    int moveId = AddMagic( tr( "Téléportation" ), SLOT( Move() ), magicMenu );
    magicMenu->setItemEnabled( moveId, orders.CanMagicMove() );
    AddMagic( tr( "Recompletement total" ),      T_MsgUnitMagicAction_action_recompletement_total,      magicMenu );
    AddMagic( tr( "Recompletement personnel" ),  T_MsgUnitMagicAction_action_recompletement_personnel,  magicMenu );
    AddMagic( tr( "Recompletement équipement" ), T_MsgUnitMagicAction_action_recompletement_equipement, magicMenu );
    AddMagic( tr( "Recompletement ressources" ), T_MsgUnitMagicAction_action_recompletement_ressources, magicMenu );
    AddMagic( tr( "Recompletement partiel" ),    SLOT( Recompletion ),      magicMenu );
    AddMagic( tr( "Détruire composante" ),       SLOT( DestroyComponent ),  magicMenu );
    AddMagic( tr( "Destruction totale" ),        T_MsgUnitMagicAction_action_destruction_totale,        magicMenu );

    if( orders.CanSurrender() )
        AddMagic( tr( "Se rendre" ), SLOT( Surrender() ), magicMenu );

    if( orders.CanRetrieveTransporters() )
        AddMagic( tr( "Récupérer transporteurs" ), SLOT( RecoverHumanTransporters() ), magicMenu );

    menu.insertItem( tr( "Ordres magiques" ), magicMenu );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::OptionChanged
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "CurrentTeam" )
        controller_ = value.To< const Team* >() == 0;
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::AddMagic
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::AddMagic( const QString& label, int id, QPopupMenu* menu )
{
    const int nId = menu->insertItem( label, this, SLOT( Magic( int ) ) );
    menu->setItemParameter( nId, id );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::AddMagic
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
int MagicOrdersInterface::AddMagic( const QString& label, const char* slot, QPopupMenu* menu )
{
    return menu->insertItem( label, this, slot );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::Magic
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::Magic( int type )
{
    if( selectedAgent_ )
    {
        ASN_MsgUnitMagicAction asnMsg;
        asnMsg.GetAsnMsg().oid      = selectedAgent_->GetId();
        asnMsg.GetAsnMsg().action.t = type;
        asnMsg.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::Recompletion
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::Recompletion()
{
    if( selectedAgent_ )
    {
//        supplyRecompletion_->SetAgent( *selectedAgent_ );
//        supplyRecompletion_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::DestroyComponent
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::DestroyComponent()
{
    if( selectedAgent_ )
    {
        // $$$$ AGE 2006-04-28: DIN_Message
        App::GetApp().GetNetwork().GetMessageMgr().SendMsgUnitMagicActionDestroyComposante( *selectedAgent_ );
    }
}
    
// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::ChangeHumanFactors
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::ChangeHumanFactors()
{
    if( selectedAgent_ )
    {
//        changeHumanFactors_->SetAgent( *selectedAgent_ );
//        changeHumanFactors_->show();
    }
}
    
// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::Move
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::Move()
{

}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::Surrender
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::Surrender()
{
    if( selectedAgent_ )
    {
        ASN_MsgUnitMagicAction asnMsg;
        asnMsg.GetAsnMsg().oid      = selectedAgent_->GetId();
        asnMsg.GetAsnMsg().action.t = T_MsgUnitMagicAction_action_se_rendre;
        asnMsg.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::RecoverHumanTransporters
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::RecoverHumanTransporters()
{
    if( selectedAgent_ )
    {
        ASN_MsgUnitMagicAction asnMsg;
        asnMsg.GetAsnMsg().oid      = selectedAgent_ ->GetId();
        asnMsg.GetAsnMsg().action.t = T_MsgUnitMagicAction_action_recuperer_transporteurs;
        asnMsg.Send();
    }
}
