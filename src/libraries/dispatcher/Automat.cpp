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
#include "Side.h"
#include "Formation.h"
#include "Model.h"
#include "KnowledgeGroup.h"
#include "ClientPublisher_ABC.h"
#include "Agent.h"
#include "DotationQuota.h"
#include "ModelVisitor_ABC.h"
#include "AutomatOrder.h"
#include "Report.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::Automat( Model& model, const ASN1T_MsgAutomatCreation& msg )
    : SimpleEntity< kernel::Automat_ABC >( msg.oid, msg.nom )
    , model_            ( model )
    , type_             ( msg.type_automate )
    , name_             ( msg.nom )
    , team_             ( model.sides_.Get( msg.oid_camp ) )
    , parentFormation_  ( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_formation ? static_cast< Formation* >( &model.formations_.Get( msg.oid_parent.u.formation ) ) : 0 )
    , parentAutomat_    ( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_automate  ? static_cast< Automat* >( &model.automats_.Get( msg.oid_parent.u.automate ) ) : 0 )
    , knowledgeGroup_   ( static_cast< KnowledgeGroup* >( &model.knowledgeGroups_.Get( msg.oid_groupe_connaissance ) ) )
    , pTC2_             ( 0 )
    , pLogMaintenance_  ( 0 )
    , pLogMedical_      ( 0 )
    , pLogSupply_       ( 0 )
    , nAutomatState_    ( EnumAutomatMode::debraye )
    , nForceRatioState_ ( EnumForceRatioStatus::neutre )
    , nCloseCombatState_( EnumMeetingEngagementStatus::etat_fixe )
    , nOperationalState_( EnumOperationalStatus::detruit_totalement )
    , nRoe_             ( EnumRoe::tir_interdit )
    , order_            ( 0 )
{
    assert( parentFormation_ || parentAutomat_ );

    knowledgeGroup_->automats_.Register( msg.oid, *this );
    if( parentFormation_ )
        parentFormation_->automats_.Register( msg.oid, *this );
    else if( parentAutomat_ )
        parentAutomat_->automats_.Register( msg.oid, *this );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    quotas_.DeleteAll();
    if( parentFormation_ )
        parentFormation_->automats_.Remove( GetId() );
    else if( parentAutomat_ )
        parentAutomat_->automats_.Remove( GetId() );
    knowledgeGroup_->automats_.Remove( GetId() );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatCreation& msg )
{
    ChangeKnowledgeGroup( msg.oid_groupe_connaissance );
    if(   parentFormation_ && ( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_automate  || ( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_formation && msg.oid_parent.u.formation != parentFormation_->GetId() ) )
       || parentAutomat_   && ( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_formation || ( msg.oid_parent.t == T_MsgAutomatCreation_oid_parent_automate  && msg.oid_parent.u.automate  != parentAutomat_  ->GetId() ) ) )
       ChangeSuperior( msg.oid_parent );
    decisionalInfos_.Clear();
    ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatChangeLogisticLinks& msg )
{
    if( msg.m.oid_tc2Present )
        pTC2_ = msg.oid_tc2 == 0 ? 0 : &model_.automats_.Get( msg.oid_tc2 );
    if( msg.m.oid_maintenancePresent )
        pLogMaintenance_ = msg.oid_maintenance == 0 ? 0 : &model_.automats_.Get( msg.oid_maintenance );
    if( msg.m.oid_santePresent )
        pLogMedical_ = msg.oid_sante == 0 ? 0 : &model_.automats_.Get( msg.oid_sante );
    if( msg.m.oid_ravitaillementPresent )
        pLogSupply_ = msg.oid_ravitaillement == 0 ? 0 : &model_.automats_.Get( msg.oid_ravitaillement );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatChangeSuperior& msg )
{
    ChangeSuperior( msg.oid_superior );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatChangeKnowledgeGroup& msg )
{
    ChangeKnowledgeGroup( msg.oid_groupe_connaissance );
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
        knowledgeGroup_->automats_.Remove( GetId() );
    knowledgeGroup_ = &model_.knowledgeGroups_.Get( id );
    knowledgeGroup_->automats_.Register( GetId(), *this );
}

// -----------------------------------------------------------------------------
// Name: Automat::ChangeSuperior
// Created: SBO 2008-07-10
// -----------------------------------------------------------------------------
template< typename Superior >
void Automat::ChangeSuperior( const Superior& superior )
{
    if( parentFormation_ )
        parentFormation_->automats_.Remove( GetId() );
    if( parentAutomat_ )
        parentAutomat_->automats_.Remove( GetId() );
    parentFormation_ = 0;
    parentAutomat_   = 0;
    if( superior.t == T_MsgAutomatChangeSuperior_oid_superior_formation )
    {
        parentFormation_ = &model_.formations_.Get( superior.u.formation );
        parentFormation_->automats_.Register( GetId(), *this );
    }
    else if( superior.t == T_MsgAutomatChangeSuperior_oid_superior_automate )
    {
        parentAutomat_ = &model_.automats_.Get( superior.u.automate );
        parentAutomat_->automats_.Register( GetId(), *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatAttributes& msg )
{
    if( msg.m.etat_automatePresent )
        nAutomatState_ = msg.etat_automate;
    if( msg.m.rapport_de_forcePresent )
        nForceRatioState_ = msg.rapport_de_force;
    if( msg.m.combat_de_rencontrePresent )
        nCloseCombatState_ = msg.combat_de_rencontre;
    if( msg.m.etat_operationnelPresent )
        nOperationalState_ = msg.etat_operationnel;
    if( msg.m.roePresent )
        nRoe_ = msg.roe;
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgDecisionalState& asnMsg )
{
    decisionalInfos_.Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgLogSupplyQuotas& msg )
{
    quotas_.DeleteAll();
    for( unsigned i = 0; i < msg.quotas.n; ++i )
    {
        DotationQuota* quota = new DotationQuota( model_, msg.quotas.elem[ i ] );
        quotas_.Register( msg.quotas.elem[ i ].ressource_id, *quota );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Automat::Update( const ASN1T_MsgAutomatOrder& msg )
{
    order_.reset();
    if( msg.mission != 0 )
        order_.reset( new AutomatOrder( model_, *this, msg ) );
    ApplyUpdate( msg );
}

namespace
{
    struct QuotaSerializer
    {
        explicit QuotaSerializer( ASN1T_DotationQuota* asn ) : asn_( asn ), index_( 0 ) {}
        void SerializeQuota( const DotationQuota& quota )
        {
            quota.Send( asn_[ index_ ] );
            ++index_;
        }
        ASN1T_DotationQuota* asn_;
        int index_;
    };
}

// -----------------------------------------------------------------------------
// Name: Automat::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Automat::SendCreation( ClientPublisher_ABC& publisher ) const
{
    {
        client::AutomatCreation asn;
        asn().oid                     = GetId();
        asn().type_automate           = type_;
        asn().nom                     = name_.c_str();
        asn().oid_camp                = team_.GetId();
        asn().oid_groupe_connaissance = knowledgeGroup_->GetId();
        asn().oid_parent.t = parentFormation_ ? T_MsgAutomatCreation_oid_parent_formation : T_MsgAutomatCreation_oid_parent_automate;
        if( parentFormation_ )
            asn().oid_parent.u.formation = parentFormation_->GetId();
        if( parentAutomat_ )
            asn().oid_parent.u.automate = parentAutomat_->GetId();
        asn.Send( publisher );
    }
    {
        client::LogSupplyQuotas asn;
        asn().oid_automate = GetId();
        asn().quotas.n = quotas_.Count();
        asn().quotas.elem = asn().quotas.n > 0 ? new ASN1T_DotationQuota[ asn().quotas.n ] : 0;
        QuotaSerializer serializer( asn().quotas.elem );
        quotas_.Apply( boost::bind( &QuotaSerializer::SerializeQuota, boost::ref( serializer ), _1 ) );
        asn.Send( publisher );
        if( asn().quotas.n > 0 )
            delete [] asn().quotas.elem;
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
        asn().m.etat_automatePresent = 1;
        asn().m.rapport_de_forcePresent = 1;
        asn().m.combat_de_rencontrePresent = 1;
        asn().m.etat_operationnelPresent = 1;
        asn().m.roePresent = 1;
        asn().oid = GetId();
        asn().etat_automate = nAutomatState_;
        asn().rapport_de_force = nForceRatioState_;
        asn().combat_de_rencontre = nCloseCombatState_;
        asn().etat_operationnel = nOperationalState_;
        asn().roe = nRoe_;
        asn.Send( publisher );
    }
    {
        client::AutomatChangeKnowledgeGroup asn;
        asn().oid = GetId();
        asn().oid_camp = team_.GetId();
        asn().oid_groupe_connaissance = knowledgeGroup_->GetId();
        asn.Send( publisher );
    }
    {
        client::AutomatChangeSuperior asn;
        asn().oid = GetId();
        asn().oid_superior.t = parentFormation_ ? T_MsgAutomatCreation_oid_parent_formation : T_MsgAutomatCreation_oid_parent_automate;
        if( parentFormation_ )
            asn().oid_superior.u.formation = parentFormation_->GetId();
        if( parentAutomat_ )
            asn().oid_superior.u.automate = parentAutomat_->GetId();
        asn.Send( publisher );
    }
    {
        client::AutomatChangeLogisticLinks asn;
        asn().oid  = GetId();
        if( pTC2_ )
        {
            asn().m.oid_tc2Present = 1;
            asn().oid_tc2 = pTC2_->GetId();
        }
        if( pLogMaintenance_ )
        {
            asn().m.oid_maintenancePresent = 1;
            asn().oid_maintenance = pLogMaintenance_->GetId();
        }
        if( pLogMedical_ )
        {
            asn().m.oid_santePresent = 1;
            asn().oid_sante = pLogMedical_->GetId();
        }
        if( pLogSupply_ )
        {
            asn().m.oid_ravitaillementPresent = 1;
            asn().oid_ravitaillement = pLogSupply_->GetId();
        }
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
    void VisitorAdapter( ModelVisitor_ABC& visitor, kernel::Entity_ABC& entity )
    {
        static_cast< C& >( entity ).Accept( visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Automat::Accept( ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    automats_.Apply( boost::bind( &Automat::Accept, _1, boost::ref( visitor ) ) );
    agents_.Apply( boost::bind( &Agent::Accept, _1, boost::ref( visitor ) ) );
}

// -----------------------------------------------------------------------------
// Name: Automat::GetType
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::AutomatType& Automat::GetType() const
{
    throw std::runtime_error( "Not implemented" ); // $$$$ AGE 2008-06-20: 
}

// -----------------------------------------------------------------------------
// Name: Automat::IsEngaged
// Created: SBO 2008-07-10
// -----------------------------------------------------------------------------
bool Automat::IsEngaged() const
{
    return nAutomatState_ == EnumAutomatMode::embraye;
}