// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Automat.h"
#include "Agent.h"
#include "AutomatOrder.h"
#include "DotationQuota.h"
#include "Formation.h"
#include "KnowledgeGroup.h"
#include "Model.h"
#include "Report.h"
#include "Side.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/clientsenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::Automat( Model_ABC& model, const MsgsSimToClient::MsgAutomatCreation& msg )
    : Automat_ABC       ( msg.oid(), QString( msg.nom().c_str() ) )
    , model_            ( model )
    , type_             ( msg.type_automate() )
    , name_             ( msg.nom().c_str() )
    , team_             ( model.Sides().Get( msg.oid_camp() ) )
    , parentFormation_  ( msg.oid_parent().has_formation() ? &model.Formations().Get( msg.oid_parent().formation().oid() ) : 0 )
    , parentAutomat_    ( msg.oid_parent().has_automate()  ? &model.Automats().Get( msg.oid_parent().automate().oid() ) : 0 )
    , knowledgeGroup_   ( &model.KnowledgeGroups().Get( msg.oid_groupe_connaissance() ) )
    , pTC2_             ( 0 )
    , pLogMaintenance_  ( 0 )
    , pLogMedical_      ( 0 )
    , pLogSupply_       ( 0 )
    , nAutomatState_    ( Common::debraye )
    , nForceRatioState_ ( MsgsSimToClient::ForceRatio_Value_neutre )
    , nCloseCombatState_( Common::etat_fixe )
    , nOperationalState_( Common::detruit_totalement )
    , nRoe_             ( MsgsSimToClient::RulesOfEngagement_Value_tir_interdit )
    , order_            ( 0 )
{
    if( ! parentFormation_ && ! parentAutomat_ )
        throw std::runtime_error( __FUNCTION__ ": invalid parent for automat " + msg.nom() );

    knowledgeGroup_->Register( *this );
    if( parentFormation_ )
        parentFormation_->Register( *this );
    else if( parentAutomat_ )
        parentAutomat_->Register( *this );
    RegisterSelf( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    quotas_.DeleteAll();
    if( parentFormation_ )
    {
        MoveChildren( *parentFormation_, false );
        parentFormation_->Remove( *this );
    }
    else if( parentAutomat_ )
    {
        MoveChildren( *parentAutomat_, true );
        parentAutomat_->Remove( *this );
    }
    knowledgeGroup_->Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat::MoveChildren
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
template< typename Superior >
void Automat::MoveChildren( Superior& superior, bool agents )
{
    tools::Iterator< const dispatcher::Automat_ABC& > it( automats_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        dispatcher::Automat_ABC& entity = const_cast< dispatcher::Automat_ABC& >( it.NextElement() );
        Remove( entity );
        superior.Register( entity );
    }
    automats_.Clear();
    if( agents )
    {
        tools::Iterator< const dispatcher::Agent_ABC& > it( agents_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            dispatcher::Agent_ABC& entity = const_cast< dispatcher::Agent_ABC& >( it.NextElement() );
            static_cast< Agent& >( entity ).ChangeAutomat( superior.GetId() ); // $$$$ SBO 2010-06-02: add Register/Remove on Agent_ABC
        }
        agents_.Clear();
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const MsgsSimToClient::MsgAutomatCreation& msg )
{
    ChangeKnowledgeGroup( msg.oid_groupe_connaissance() );
    if( parentFormation_ && 
        ( msg.oid_parent().has_automate()  || 
        ( msg.oid_parent().has_formation() && msg.oid_parent().formation().oid() != parentFormation_->GetId() ) ) )
        ChangeSuperior( msg.oid_parent() );
    if( parentAutomat_ && ( msg.oid_parent().has_formation() || ( msg.oid_parent().has_automate()  && msg.oid_parent().automate().oid()  != parentAutomat_  ->GetId() ) ) )
       ChangeSuperior( msg.oid_parent() );
    decisionalInfos_.Clear();
}

// -----------------------------------------------------------------------------
// Name: Automat::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const Common::MsgAutomatChangeLogisticLinks& msg )
{
    if( msg.has_oid_tc2() )
        pTC2_ = msg.oid_tc2() == 0 ? 0 : &model_.Automats().Get( msg.oid_tc2() );
    if( msg.has_oid_maintenance() )
        pLogMaintenance_ = msg.oid_maintenance() == 0 ? 0 : &model_.Automats().Get( msg.oid_maintenance() );
    if( msg.has_oid_sante() )
        pLogMedical_ = msg.oid_sante() == 0 ? 0 : &model_.Automats().Get( msg.oid_sante() );
    if( msg.has_oid_ravitaillement() )
        pLogSupply_ = msg.oid_ravitaillement() == 0 ? 0 : &model_.Automats().Get( msg.oid_ravitaillement() );
}

// -----------------------------------------------------------------------------
// Name: Automat::DoUpdate
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const Common::MsgAutomatChangeSuperior& msg )
{
    ChangeSuperior( msg.oid_superior() );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const Common::MsgAutomatChangeKnowledgeGroup& msg )
{
    ChangeKnowledgeGroup( msg.oid_groupe_connaissance() );
}

// -----------------------------------------------------------------------------
// Name: Automat::ChangeKnowledgeGroup
// Created: SBO 2008-07-10
// -----------------------------------------------------------------------------
void Automat::ChangeKnowledgeGroup( unsigned long id )
{
    if( knowledgeGroup_ && knowledgeGroup_->GetId() == id )
        return;
    if( knowledgeGroup_ )
        knowledgeGroup_->Remove( *this );
    knowledgeGroup_ = &model_.KnowledgeGroups().Get( id );
    knowledgeGroup_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat::ChangeSuperior
// Created: SBO 2008-07-10
// -----------------------------------------------------------------------------
template< typename Superior >
void Automat::ChangeSuperior( const Superior& superior )
{
    if( parentFormation_ )
        parentFormation_->Remove( *this );
    if( parentAutomat_ )
        parentAutomat_->Remove( *this );
    parentFormation_ = 0;
    parentAutomat_   = 0;
    if( superior.has_formation() )
    {
        parentFormation_ = &model_.Formations().Get( superior.formation().oid() );
        parentFormation_->Register( *this );
    }
    else if( superior.has_automate() )
    {
        parentAutomat_ = &model_.Automats().Get( superior.automate().oid() );
        parentAutomat_->Register( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::DoUpdate
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const MsgsSimToClient::MsgAutomatAttributes& msg )
{
    if( msg.has_etat_automate()  )
        nAutomatState_ = msg.etat_automate();
    if( msg.has_rapport_de_force()  )
        nForceRatioState_ = msg.rapport_de_force();
    if( msg.has_combat_de_rencontre()  )
        nCloseCombatState_ = msg.combat_de_rencontre();
    if( msg.has_etat_operationnel()  )
        nOperationalState_ = msg.etat_operationnel();
    if( msg.has_roe()  )
        nRoe_ = msg.roe();
}

// -----------------------------------------------------------------------------
// Name: Automat::DoUpdate
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const MsgsSimToClient::MsgDecisionalState& asnMsg )
{
    decisionalInfos_.Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Automat::DoUpdate
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const MsgsSimToClient::MsgLogSupplyQuotas& msg )
{
    quotas_.DeleteAll();
    for( int i = 0; i < msg.quotas().elem_size(); ++i )
    {
        DotationQuota* quota = new DotationQuota( model_, msg.quotas().elem( i ) );
        quotas_.Register( msg.quotas().elem( i ).ressource_id(), *quota );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::DoUpdate
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const Common::MsgAutomatOrder& msg )
{
    order_.reset();
    if( msg.mission() != 0 )
        order_.reset( new AutomatOrder( model_, *this, msg ) );
}

namespace
{
    void SerializeQuota( ::Common::SeqOfDotationQuota& message, const DotationQuota& quota )
    {
        quota.Send( *message.add_elem() );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Automat::SendCreation( ClientPublisher_ABC& publisher ) const
{
    {
        client::AutomatCreation asn;
        asn().set_oid( GetId() );
        asn().set_type_automate( type_ );
        asn().set_nom( name_ );
        asn().set_oid_camp( team_.GetId() );
        asn().set_oid_groupe_connaissance( knowledgeGroup_->GetId() );

        if( parentFormation_ )
            asn().mutable_oid_parent()->mutable_formation()->set_oid( parentFormation_->GetId() );
        if( parentAutomat_ )
            asn().mutable_oid_parent()->mutable_automate()->set_oid( parentAutomat_->GetId() );
        asn.Send( publisher );
    }
    {
        client::LogSupplyQuotas asn;
        asn().set_oid_automate ( GetId() );
        quotas_.Apply( boost::bind( &::SerializeQuota, boost::ref( *asn().mutable_quotas() ), _1 ) );
        asn.Send( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    {
        client::AutomatAttributes asn;
        asn().set_oid ( GetId() );
        asn().set_etat_automate( nAutomatState_ );
        asn().set_rapport_de_force( nForceRatioState_);
        asn().set_combat_de_rencontre( nCloseCombatState_);
        asn().set_etat_operationnel( nOperationalState_ );
        asn().set_roe( nRoe_ );
        asn.Send( publisher );
    }
    {
        client::AutomatChangeKnowledgeGroup asn;
        asn().set_oid( GetId() );
        asn().set_oid_camp( team_.GetId() );
        asn().set_oid_groupe_connaissance( knowledgeGroup_->GetId() );
        asn.Send( publisher );
    }
    {
        client::AutomatChangeSuperior asn;
        asn().set_oid( GetId() );

        if( parentFormation_ )
            asn().mutable_oid_superior()->mutable_formation()->set_oid( parentFormation_->GetId() );
        if( parentAutomat_ )
            asn().mutable_oid_superior()->mutable_automate()->set_oid( parentAutomat_->GetId() );
        asn.Send( publisher );
    }
    {
        client::AutomatChangeLogisticLinks asn;
        asn().set_oid( GetId() );
        if( pTC2_ )
            asn().set_oid_tc2( pTC2_->GetId() );
        if( pLogMaintenance_ )
            asn().set_oid_maintenance( pLogMaintenance_->GetId() );
        if( pLogMedical_ )
            asn().set_oid_sante( pLogMedical_->GetId() );
        if( pLogSupply_ )
            asn().set_oid_ravitaillement( pLogSupply_->GetId() );
        asn.Send( publisher );
    }
    if( order_.get() )
        order_->Send( publisher );
    else
        AutomatOrder::SendNoMission( *this, publisher );

    decisionalInfos_.Send( GetId(), publisher );
}

// -----------------------------------------------------------------------------
// Name: Automat::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Automat::SendDestruction( ClientPublisher_ABC& ) const
{
    throw std::runtime_error( __FUNCTION__ );
}

namespace
{
    template< typename C >
    void VisitorAdapter( kernel::ModelVisitor_ABC& visitor, kernel::Entity_ABC& entity )
    {
        static_cast< C& >( entity ).Accept( visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Automat::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    automats_.Apply( boost::bind( &dispatcher::Automat_ABC::Accept, _1, boost::ref( visitor ) ) );
    agents_.Apply( boost::bind( &dispatcher::Agent_ABC::Accept, _1, boost::ref( visitor ) ) );
}

// -----------------------------------------------------------------------------
// Name: Automat::GetType
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::AutomatType& Automat::GetType() const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" ); // $$$$ AGE 2008-06-20: 
}

// -----------------------------------------------------------------------------
// Name: Automat::IsEngaged
// Created: SBO 2008-07-10
// -----------------------------------------------------------------------------
bool Automat::IsEngaged() const
{
    return nAutomatState_ == Common::embraye;
}

// -----------------------------------------------------------------------------
// Name: Automat::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Automat::Register( dispatcher::Automat_ABC& automat )
{
    automats_.Register( automat.GetId(), automat );
}
// -----------------------------------------------------------------------------
// Name: Automat::Remove
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Automat::Remove( dispatcher::Automat_ABC& automat )
{
    automats_.Remove( automat.GetId() );
}
// -----------------------------------------------------------------------------
// Name: Automat::GetAutomats
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
const tools::Resolver< dispatcher::Automat_ABC >& Automat::GetAutomats() const
{
    return automats_;
}
// -----------------------------------------------------------------------------
// Name: Automat::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Automat::Register( dispatcher::Agent_ABC& automat )
{
    agents_.Register( automat.GetId(), automat );
}
// -----------------------------------------------------------------------------
// Name: Automat::Remove
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Automat::Remove( dispatcher::Agent_ABC& automat )
{
    agents_.Remove( automat.GetId() );
}
// -----------------------------------------------------------------------------
// Name: Automat::GetAgents
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
const tools::Resolver< dispatcher::Agent_ABC >& Automat::GetAgents() const
{
    return agents_;
}
// -----------------------------------------------------------------------------
// Name: Automat::GetParentAutomat
// Created: MGD 2009-12-23
// -----------------------------------------------------------------------------
kernel::Automat_ABC* Automat::GetParentAutomat() const
{
    return parentAutomat_;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetFormation
// Created: MGD 2009-12-23
// -----------------------------------------------------------------------------
kernel::Formation_ABC* Automat::GetFormation() const
{
    return parentFormation_;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetFormation
// Created: MGD 2009-12-23
// -----------------------------------------------------------------------------
dispatcher::Team_ABC& Automat::GetTeam() const
{
    return team_;
}
// -----------------------------------------------------------------------------
// Name: Automat::GetKnowledgeGroup
// Created: MGD 2009-12-23
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC& Automat::GetKnowledgeGroup() const
{
    if( !knowledgeGroup_ )
        throw std::runtime_error( __FUNCTION__ ": automat without a knowledge group." );
    return *knowledgeGroup_;
}  