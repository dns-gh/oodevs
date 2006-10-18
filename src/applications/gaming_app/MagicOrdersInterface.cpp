// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MagicOrdersInterface.h"
#include "moc_MagicOrdersInterface.cpp"

#include "clients_gui/LocationCreator.h"
#include "clients_gui/ParametersLayer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Profile_ABC.h"

#include "gaming/StaticModel.h"
#include "gaming/MagicOrders.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/ASN_Messages.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface constructor
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
MagicOrdersInterface::MagicOrdersInterface( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const StaticModel& staticModel, ParametersLayer& layer, const kernel::Profile_ABC& profile )
    : QObject( parent )
    , controllers_( controllers )
    , publisher_( publisher )
    , static_( staticModel )
    , profile_( profile )
    , selectedEntity_( controllers )
    , magicMove_( false )
{
    magicMoveLocation_ = new LocationCreator( 0, layer, *this );
    magicMoveLocation_->Allow( true, false, false, false );
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
void MagicOrdersInterface::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    if( !profile_.CanDoMagic( agent ) )
        return;

    selectedEntity_ = &agent;
    if( const MagicOrders* orders = agent.Retrieve< MagicOrders >() )
    {
        QPopupMenu* magicMenu = menu.SubMenu( "Ordre", tr( "Ordres magiques" ) );

        int moveId = AddMagic( tr( "Téléportation" ), SLOT( Move() ), magicMenu );
        magicMenu->setItemEnabled( moveId, orders->CanMagicMove() );
        AddMagic( tr( "Recompletement total" ),      T_MsgUnitMagicAction_action_recompletement_total,      magicMenu );
        AddMagic( tr( "Recompletement personnel" ),  T_MsgUnitMagicAction_action_recompletement_personnel,  magicMenu );
        AddMagic( tr( "Recompletement équipement" ), T_MsgUnitMagicAction_action_recompletement_equipement, magicMenu );
        AddMagic( tr( "Recompletement ressources" ), T_MsgUnitMagicAction_action_recompletement_ressources, magicMenu );
        AddMagic( tr( "Détruire composante" ),       SLOT( DestroyComponent() ),  magicMenu );
        AddMagic( tr( "Destruction totale" ),        T_MsgUnitMagicAction_action_destruction_totale,        magicMenu );

        if( orders->CanRetrieveTransporters() )
            AddMagic( tr( "Récupérer transporteurs" ), SLOT( RecoverHumanTransporters() ), magicMenu );
    }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::NotifyContextMenu
// Created: AGE 2006-10-13
// -----------------------------------------------------------------------------
void MagicOrdersInterface::NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu )
{
    if( !profile_.CanDoMagic( agent ) )
        return;

    selectedEntity_ = &agent;
    QPopupMenu* magicMenu = menu.SubMenu( "Ordre", tr( "Ordres magiques" ) );
    AddMagic( tr( "Se rendre" ), SLOT( Surrender() ), magicMenu );
    int moveId = AddMagic( tr( "Téléportation" ), SLOT( Move() ), magicMenu );
    bool bMoveAllowed = false;
    if( const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >() )
        bMoveAllowed = decisions->IsEmbraye();
    magicMenu->setItemEnabled( moveId, bMoveAllowed );
    FillCommonOrders( magicMenu );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::NotifyContextMenu
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void MagicOrdersInterface::NotifyContextMenu( const KnowledgeGroup_ABC& group, ContextMenu& menu )
{   
    if( !profile_.CanDoMagic( group ) )
        return;

    selectedEntity_ = &group;
    QPopupMenu* magicMenu = menu.SubMenu( "Ordre", tr( "Ordres magiques" ) );
    FillCommonOrders( magicMenu );
} 

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::NotifyContextMenu
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void MagicOrdersInterface::NotifyContextMenu( const Team_ABC& team, ContextMenu& menu )
{
    if( !profile_.CanDoMagic( team ) )
        return;

    selectedEntity_ = &team;
    QPopupMenu* magicMenu = menu.SubMenu( "Ordre", tr( "Ordres magiques" ) );
    FillCommonOrders( magicMenu );
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
        MagicFunctor( Publisher_ABC& publisher, int id ) : publisher_( publisher ), id_( id ) {};
        void operator()( const Agent_ABC& agent ) const
        {
            ASN_MsgUnitMagicAction asnMsg;
            asnMsg.GetAsnMsg().oid      = agent.GetId();
            asnMsg.GetAsnMsg().action.t = id_;
            asnMsg.Send( publisher_ );
        }
    private:
        MagicFunctor& operator=( const MagicFunctor& );
        Publisher_ABC& publisher_;
        int id_;
    };

    struct RecursiveMagicFunctor : public MagicFunctor
    {
        RecursiveMagicFunctor( Publisher_ABC& publisher, int id ) : MagicFunctor( publisher, id ) {};
        void operator()( const Entity_ABC& entity ) const
        {
            if( const Agent_ABC* agent = dynamic_cast< const Agent_ABC* >( &entity ) )
                MagicFunctor::operator()( *agent );
            Iterator< const Entity_ABC& > it = entity.Get< CommunicationHierarchies >().CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const Entity_ABC& entity = it.NextElement();
                operator()( entity ); 
            }
        }
    };
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::Magic
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::Magic( int type )
{
    if( selectedEntity_ )
        ApplyOnHierarchy( *selectedEntity_, type );
    selectedEntity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::ApplyOnHierarchy
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void MagicOrdersInterface::ApplyOnHierarchy( const kernel::Entity_ABC& entity, int id )
{
    RecursiveMagicFunctor functor( publisher_, id );
    functor( entity );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::DestroyComponent
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::DestroyComponent()
{
    if( selectedEntity_ )
        publisher_.SendMagicDestruction( *selectedEntity_ ); // $$$$ SBO 2006-07-06: 
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
void MagicOrdersInterface::Handle( Location_ABC& location )
{
    if( magicMove_ && location.IsValid() )
        location.Accept( *this );
    controllers_.Remove( *magicMoveLocation_ );
    magicMove_ = false;
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::VisitPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void MagicOrdersInterface::VisitPoint( const geometry::Point2f& point )
{
    if( selectedEntity_ )
    {
        ASN_MsgUnitMagicAction message;
        message.GetAsnMsg().oid = selectedEntity_->GetId();
        message.GetAsnMsg().action.t = T_MsgUnitMagicAction_action_move_to;

        ASN1T_CoordUTM utm;
        utm = static_.coordinateConverter_.ConvertToMgrs( point ).c_str();
        message.GetAsnMsg().action.u.move_to = &utm;
        message.Send( publisher_, 56 );
    }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::Surrender
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::Surrender()
{
    if( selectedEntity_ )
    {
        ASN_MsgUnitMagicAction asnMsg;
        asnMsg.GetAsnMsg().oid      = selectedEntity_->GetId();
        asnMsg.GetAsnMsg().action.t = T_MsgUnitMagicAction_action_se_rendre;
        asnMsg.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::RecoverHumanTransporters
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::RecoverHumanTransporters()
{
    if( selectedEntity_ )
    {
        ASN_MsgUnitMagicAction asnMsg;
        asnMsg.GetAsnMsg().oid      = selectedEntity_ ->GetId();
        asnMsg.GetAsnMsg().action.t = T_MsgUnitMagicAction_action_recuperer_transporteurs;
        asnMsg.Send( publisher_ );
    }
}
