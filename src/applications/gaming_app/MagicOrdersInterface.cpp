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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Team_ABC.h"

#include "gaming/StaticModel.h"
#include "gaming/MagicOrders.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/ASN_Messages.h"
#include "gaming/Attributes.h"
#include "ENT/ENT_Tr.h"

using namespace kernel;
using namespace gui;

namespace
{
    class InitializedLineEdit : public QLineEdit
    {
    public:
        InitializedLineEdit( QWidget* parent, const QString& initialValue )
            : QLineEdit( initialValue, parent ), initialValue_( initialValue ) {}

        virtual void keyPressEvent( QKeyEvent* e )
        {
            if( text() == initialValue_ )
            {
                clear();
                setAlignment( Qt::AlignRight );
            }
            QLineEdit::keyPressEvent( e );
        }

        virtual void showEvent( QShowEvent* e )
        {
            if( text() == initialValue_ )
            {
                selectAll();
                setFocus();
            }
            QLineEdit::showEvent( e );
        }

    private:
        QString initialValue_;
    };
}

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
    magicMoveLocation_->Allow( false, false, false, false );
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
        QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        int moveId = AddMagic( tr( "Teleport" ), SLOT( Move() ), magicMenu );
        magicMenu->setItemEnabled( moveId, orders->CanMagicMove() );
        AddSurrenderMenu( magicMenu, agent );
        if( orders->CanRetrieveTransporters() )
            AddMagic( tr( "Recover - Transporters" ), SLOT( RecoverHumanTransporters() ), magicMenu );
        AddMagic( tr( "Destroy - Component" ),  SLOT( DestroyComponent() ),  magicMenu );
        FillCommonOrders( magicMenu );
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
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    int moveId = AddMagic( tr( "Teleport" ), SLOT( Move() ), magicMenu );
    bool bMoveAllowed = false;
    if( const AutomatDecisions* decisions = agent.Retrieve< AutomatDecisions >() )
        bMoveAllowed = decisions->IsEmbraye();
    magicMenu->setItemEnabled( moveId, bMoveAllowed );
    AddSurrenderMenu( magicMenu, agent );
    FillCommonOrders( magicMenu );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::NotifyContextMenu
// Created: SBO 2007-02-27
// -----------------------------------------------------------------------------
void MagicOrdersInterface::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    if( !profile_.CanDoMagic( entity ) )
        return;

    selectedEntity_ = &entity;
    QPopupMenu* magicMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
    AddMagic( tr( "Teleport" ), SLOT( Move() ), magicMenu );
    AddMagic( tr( "Kill all" ), SLOT( KillAllPopulation() ), magicMenu );
    AddValuedMagic( magicMenu, menu, tr( "Kill people:" ), SLOT( KillSomePopulation() ) );
    AddValuedMagic( magicMenu, menu, tr( "Resurect people:" ), SLOT( ResurectSomePopulation() ) );

    QPopupMenu* choiceMenu = new QPopupMenu( magicMenu );
    for( unsigned int i = 0; i < (unsigned int)eNbrPopulationAttitude; ++i )
        choiceMenu->insertItem( ENT_Tr::ConvertFromPopulationAttitude( (E_PopulationAttitude)i ).c_str(), this, SLOT( ChangePopulationAttitude( int ) ), 0, i );
    magicMenu->insertItem( tr( "Change population attitude" ), choiceMenu );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::FillCommonOrders
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void MagicOrdersInterface::FillCommonOrders( QPopupMenu* magicMenu )
{
    AddMagic( tr( "Recover - All" ),        T_MsgUnitMagicAction_action_recompletement_total,      magicMenu );
    AddMagic( tr( "Recover - Troops" ),     T_MsgUnitMagicAction_action_recompletement_personnel,  magicMenu );
    AddMagic( tr( "Recover - Equipments" ), T_MsgUnitMagicAction_action_recompletement_equipement, magicMenu );
    AddMagic( tr( "Recover - Resources" ),  T_MsgUnitMagicAction_action_recompletement_ressources, magicMenu );
    AddMagic( tr( "Destroy - All" ),        T_MsgUnitMagicAction_action_destruction_totale,        magicMenu );
}

namespace
{
    // $$$$ SBO 2007-03-08: temporary, surrendered status should be also present on Automat attributes...
    bool IsSurrendered( const kernel::Entity_ABC& entity )
    {
        if( const Attributes* attr = static_cast< const Attributes* >( entity.Retrieve< kernel::Attributes_ABC >() ) )
            if( attr->surrenderedTo_ )
                return true;
        if( const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
        {
            kernel::Iterator< const kernel::Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
            while( it.HasMoreElements() )
                if( IsSurrendered( it.NextElement() ) )
                    return true;
        }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::AddSurrenderMenu
// Created: SBO 2007-03-08
// -----------------------------------------------------------------------------
void MagicOrdersInterface::AddSurrenderMenu( QPopupMenu* parent, const kernel::Entity_ABC& entity )
{
    if( IsSurrendered( entity ) )
        AddMagic( tr( "Cancel surrender" ), T_MsgUnitMagicAction_action_annuler_reddition, parent );
    else
    {
        QPopupMenu* menu = new QPopupMenu( parent );
        for( CIT_Teams it = teams_.begin(); it != teams_.end(); ++it )
            menu->insertItem( (*it)->GetName(), this, SLOT( SurrenderTo( int ) ), 0, (*it)->GetId() );
        parent->insertItem( tr( "Surrender to" ), menu );
    }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::AddValuedMagic
// Created: SBO 2007-02-27
// -----------------------------------------------------------------------------
void MagicOrdersInterface::AddValuedMagic( QPopupMenu* parent, kernel::ContextMenu& menu, const QString& label, const char* slot )
{
    QPopupMenu* valueMenu = new QPopupMenu( parent );
    QLineEdit* valueEditor = new InitializedLineEdit( valueMenu, tr( "Enter value" ) );
    valueMenu->insertItem( valueEditor );
    parent->insertItem( label, valueMenu );
    QToolTip::add( valueEditor, tr( "Type-in value then press 'Enter'" ) );
    connect( valueEditor, SIGNAL( returnPressed() ), this, slot );
    connect( valueEditor, SIGNAL( returnPressed() ), menu, SLOT( hide() ) );
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
            const Hierarchies* h = entity.Retrieve< CommunicationHierarchies >();
            if( ! h )
                h = entity.Retrieve< TacticalHierarchies >();
            if( h )
            {
                Iterator< const Entity_ABC& > it = h->CreateSubordinateIterator();
                while( it.HasMoreElements() )
                {
                    const Entity_ABC& entity = it.NextElement();
                    operator()( entity ); 
                }
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
// Name: MagicOrdersInterface::SendMagicMove
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
template< typename Message >
void MagicOrdersInterface::SendMagicMove( const geometry::Point2f& point, int action )
{
    ASN1T_CoordUTM utm;
    utm = static_.coordinateConverter_.ConvertToMgrs( point ).c_str();
    Message message;
    message.GetAsnMsg().oid = selectedEntity_->GetId();
    message.GetAsnMsg().action.t = action;
    message.GetAsnMsg().action.u.move_to = &utm;
    message.Send( publisher_, 56 );
    const_cast< kernel::Entity_ABC& >( *selectedEntity_ ).Update( message.GetAsnMsg() );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::VisitPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void MagicOrdersInterface::VisitPoint( const geometry::Point2f& point )
{
    if( selectedEntity_ )
    {
        if( dynamic_cast< const Agent_ABC* >( &*selectedEntity_ ) || dynamic_cast< const Automat_ABC* >( &*selectedEntity_ ) )
            SendMagicMove< ASN_MsgUnitMagicAction >( point, T_MsgUnitMagicAction_action_move_to );
        else if( dynamic_cast< const Population_ABC* >( &*selectedEntity_ ) )
            SendMagicMove< ASN_MsgPopulationMagicAction >( point, T_MsgPopulationMagicAction_action_move_to );
    }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::SurrenderTo
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void MagicOrdersInterface::SurrenderTo( int id )
{
    if( selectedEntity_ )
    {
        ASN_MsgUnitMagicAction asnMsg;
        asnMsg.GetAsnMsg().oid      = selectedEntity_->GetId();
        asnMsg.GetAsnMsg().action.t = T_MsgUnitMagicAction_action_se_rendre;
        asnMsg.GetAsnMsg().action.u.se_rendre = id;
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

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::ChangePopulationAttitude
// Created: SBO 2007-02-27
// -----------------------------------------------------------------------------
void MagicOrdersInterface::ChangePopulationAttitude( int index )
{
    if( selectedEntity_ )
    {
        ASN_MsgPopulationMagicAction asn;
        asn.GetAsnMsg().oid      = selectedEntity_->GetId();
        asn.GetAsnMsg().action.t = T_MsgPopulationMagicAction_action_change_attitude;

        ASN1T_MagicActionPopulationChangeAttitude params;
        params.attitude       = (ASN1T_EnumPopulationAttitude)index;
        params.beneficiaire.t = T_MagicActionPopulationChangeAttitude_beneficiaire_global;

        asn.GetAsnMsg().action.u.change_attitude = &params;
        asn.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::KillAllPopulation
// Created: SBO 2007-02-27
// -----------------------------------------------------------------------------
void MagicOrdersInterface::KillAllPopulation()
{
    if( selectedEntity_ )
    {
        ASN_MsgPopulationMagicAction asn;
        asn.GetAsnMsg().oid      = selectedEntity_->GetId();
        asn.GetAsnMsg().action.t = T_MsgPopulationMagicAction_action_destruction_totale;
        asn.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::KillSomePopulation
// Created: SBO 2007-02-27
// -----------------------------------------------------------------------------
void MagicOrdersInterface::KillSomePopulation()
{
    if( selectedEntity_ )
        if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
        {
            ASN_MsgPopulationMagicAction asn;
            asn.GetAsnMsg().oid           = selectedEntity_->GetId();
            asn.GetAsnMsg().action.t      = T_MsgPopulationMagicAction_action_tuer;
            asn.GetAsnMsg().action.u.tuer = editor->text().toUInt();
            asn.Send( publisher_ );
        }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::ResurectSomePopulation
// Created: SBO 2007-02-27
// -----------------------------------------------------------------------------
void MagicOrdersInterface::ResurectSomePopulation()
{
    if( selectedEntity_ )
        if( const QLineEdit* editor = dynamic_cast< const QLineEdit* >( sender() ) )
        {
            ASN_MsgPopulationMagicAction asn;
            asn.GetAsnMsg().oid                  = selectedEntity_->GetId();
            asn.GetAsnMsg().action.t             = T_MsgPopulationMagicAction_action_ressusciter;
            asn.GetAsnMsg().action.u.ressusciter = editor->text().toUInt();
            asn.Send( publisher_ );
        }
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::NotifyCreated
// Created: SBO 2007-03-08
// -----------------------------------------------------------------------------
void MagicOrdersInterface::NotifyCreated( const kernel::Team_ABC& team )
{
    teams_.push_back( &team );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface::NotifyDeleted
// Created: SBO 2007-03-08
// -----------------------------------------------------------------------------
void MagicOrdersInterface::NotifyDeleted( const kernel::Team_ABC& team )
{
    T_Teams::iterator it = std::find( teams_.begin(), teams_.end(), &team );
    if( it != teams_.end() )
    {
        std::swap( *it, teams_.back() );
        teams_.pop_back();
    }
}
