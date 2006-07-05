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
#include "Model.h"
#include "Agent.h"
#include "MagicOrders.h"
#include "OptionVariant.h"
#include "ASN_Messages.h"
#include "CoordinateConverter.h"
#include "LocationCreator.h"
#include "ParametersLayer.h"
#include "LogisticSupplyRecompletionDialog.h"
#include "ChangeHumanFactorsDialog.h"
#include "KnowledgeGroup.h"
#include "Team.h"

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface constructor
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
MagicOrdersInterface::MagicOrdersInterface( QWidget* parent, Controllers& controllers, const Model& model, ParametersLayer& layer )
    : QObject( parent )
    , controllers_( controllers )
    , model_( model )
    , selectedAgent_( controllers )
    , selectedGroup_( controllers )
    , selectedTeam_( controllers )
    , controller_( true )
    , magicMove_( false )
{
    supplyRecompletion_ = new LogisticSupplyRecompletionDialog( parent, controllers_, model_ );
    changeHumanFactors_ = new ChangeHumanFactorsDialog( parent, controllers_ );
    
    magicMoveLocation_ = new LocationCreator( 0, layer, *this );
    magicMoveLocation_->AddLocationType( tr( "point" ), EnumTypeLocalisation::point );
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
    selectedGroup_ = 0;
    const MagicOrders& orders = agent.Get< MagicOrders >();
    QPopupMenu* magicMenu = new QPopupMenu( &menu );

    int moveId = AddMagic( tr( "Téléportation" ), SLOT( Move() ), magicMenu );
    magicMenu->setItemEnabled( moveId, orders.CanMagicMove() );
    AddMagic( tr( "Recompletement total" ),      T_MsgUnitMagicAction_action_recompletement_total,      magicMenu );
    AddMagic( tr( "Recompletement personnel" ),  T_MsgUnitMagicAction_action_recompletement_personnel,  magicMenu );
    AddMagic( tr( "Recompletement équipement" ), T_MsgUnitMagicAction_action_recompletement_equipement, magicMenu );
    AddMagic( tr( "Recompletement ressources" ), T_MsgUnitMagicAction_action_recompletement_ressources, magicMenu );
    AddMagic( tr( "Recompletement partiel" ),    SLOT( Recompletion() ),      magicMenu );
    AddMagic( tr( "Détruire composante" ),       SLOT( DestroyComponent() ),  magicMenu );
    AddMagic( tr( "Destruction totale" ),        T_MsgUnitMagicAction_action_destruction_totale,        magicMenu );
    AddMagic( tr( "Facteurs humains" ),          SLOT( ChangeHumanFactors() ), magicMenu );

    if( orders.CanSurrender() )
        AddMagic( tr( "Se rendre" ), SLOT( Surrender() ), magicMenu );

    if( orders.CanRetrieveTransporters() )
        AddMagic( tr( "Récupérer transporteurs" ), SLOT( RecoverHumanTransporters() ), magicMenu );

    menu.insertItem( tr( "Ordres magiques" ), magicMenu );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::NotifyContextMenu
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void MagicOrdersInterface::NotifyContextMenu( const KnowledgeGroup& group, QPopupMenu& menu )
{   
    if( !controller_ )
        return;
    selectedAgent_ = 0;
    selectedGroup_ = &group;
    selectedTeam_ = 0;
    QPopupMenu* magicMenu = new QPopupMenu( &menu );
    FillCommonOrders( magicMenu );
    menu.insertItem( tr( "Ordres magiques" ), magicMenu );
} 

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::NotifyContextMenu
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void MagicOrdersInterface::NotifyContextMenu( const Team& team, QPopupMenu& menu )
{
    if( !controller_ )
        return;
    selectedAgent_ = 0;
    selectedGroup_ = 0;
    selectedTeam_ = &team;
    QPopupMenu* magicMenu = new QPopupMenu( &menu );
    FillCommonOrders( magicMenu );
    menu.insertItem( tr( "Ordres magiques" ), magicMenu );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::FillCommonOrders
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void MagicOrdersInterface::FillCommonOrders( QPopupMenu* magicMenu )
{
    AddMagic( tr( "Recompletement total" ),      T_MsgUnitMagicAction_action_recompletement_total,      magicMenu );
    AddMagic( tr( "Recompletement personnel" ),  T_MsgUnitMagicAction_action_recompletement_personnel,  magicMenu );
    AddMagic( tr( "Recompletement équipement" ), T_MsgUnitMagicAction_action_recompletement_equipement, magicMenu );
    AddMagic( tr( "Recompletement ressources" ), T_MsgUnitMagicAction_action_recompletement_ressources, magicMenu );
    AddMagic( tr( "Destruction totale" ),        T_MsgUnitMagicAction_action_destruction_totale,        magicMenu );
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


namespace
{
    struct MagicFunctor
    {
        MagicFunctor( int id ) : id_( id ) {};
        void operator()( const Agent& agent ) const
        {
            ASN_MsgUnitMagicAction asnMsg;
            asnMsg.GetAsnMsg().oid      = agent.GetId();
            asnMsg.GetAsnMsg().action.t = id_;
            asnMsg.Send();
        }
        int id_;
    };

    struct RecursiveMagicFunctor : public MagicFunctor
    {
        RecursiveMagicFunctor( int id ) : MagicFunctor( id ) {};
        void operator()( const Agent& agent ) const
        {
            MagicFunctor::operator()( agent );
            agent.Resolver< Agent >::Apply( *this );
        }
        void operator()( const KnowledgeGroup& group ) const
        {
            group.Resolver< Agent >::Apply( *this );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::Magic
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::Magic( int type )
{
    if( selectedAgent_ )
    {
        MagicFunctor functor( type );
        functor( *selectedAgent_ );
        selectedAgent_ = 0;
    } 
    else if( selectedGroup_ )
    {
        ApplyOnHierarchy( *selectedGroup_, type );
        selectedGroup_ = 0;
    }
    else if( selectedTeam_ )
    {
        ApplyOnHierarchy( *selectedTeam_, type );
        selectedTeam_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::ApplyOnHierarchy
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void MagicOrdersInterface::ApplyOnHierarchy( const KnowledgeGroup& group, int id )
{
    group.Resolver< Agent >::Apply( RecursiveMagicFunctor( id ) );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::ApplyOnHierarchy
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void MagicOrdersInterface::ApplyOnHierarchy( const Team& team, int id )
{
    team.Resolver< KnowledgeGroup >::Apply( RecursiveMagicFunctor( id ) );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::Recompletion
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::Recompletion()
{
    if( selectedAgent_ )
        supplyRecompletion_->Show( *selectedAgent_ );
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
        AgentServerMsgMgr::Instance().SendMsgUnitMagicActionDestroyComposante( *selectedAgent_ );
    }
}
    
// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::ChangeHumanFactors
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::ChangeHumanFactors()
{
    if( selectedAgent_ )
        changeHumanFactors_->Show( *selectedAgent_ );
}
    
// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::Move
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::Move()
{
    if( magicMove_ )
        return;
    controllers_.Register( *magicMoveLocation_ );
    magicMove_ = true;
    magicMoveLocation_->StartPoint();
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::Handle
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void MagicOrdersInterface::Handle( const T_PointVector& points )
{
    if( magicMove_ && points.size() == 1 )
    {
        ASN_MsgUnitMagicAction message;
        message.GetAsnMsg().oid = selectedAgent_->GetId();
        message.GetAsnMsg().action.t = T_MsgUnitMagicAction_action_move_to;

        ASN1T_CoordUTM utm;
        utm = model_.coordinateConverter_.ConvertToMgrs( points[0] ).c_str();
        message.GetAsnMsg().action.u.move_to = &utm;
        message.Send( 56 );
    }
    controllers_.Remove( *magicMoveLocation_ );
    magicMove_ = false;
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
