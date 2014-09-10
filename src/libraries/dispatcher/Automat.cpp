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
#include "Formation.h"
#include "KnowledgeGroup.h"
#include "Model.h"
#include "Report.h"
#include "Side.h"
#include "LogisticEntity.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/DecisionalModel.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::Automat( Model_ABC& model, const sword::AutomatCreation& msg, const tools::Resolver_ABC< kernel::AutomatType >& types )
    : Automat_ABC( msg.automat().id(), QString::fromStdString( msg.name() ) )
    , model_               ( model )
    , decisionalInfos_     ( model )
    , type_                ( types.Get( msg.type().id() ) )
    , team_                ( model.Sides().Get( msg.party().id() ) )
    , parentFormation_     ( msg.parent().has_formation() ? &model.Formations().Get( msg.parent().formation().id() ) : 0 )
    , parentAutomat_       ( msg.parent().has_automat() ? &model.Automats().Get( msg.parent().automat().id() ) : 0 )
    , knowledgeGroup_      ( &model.KnowledgeGroups().Get( msg.knowledge_group().id() ) )
    , logisticHierarchy_   ( *this, model.Formations(), model.Automats() )
    , nAutomatState_       ( sword::disengaged )
    , nForceRatioState_    ( sword::ForceRatio::neutral )
    , nCloseCombatState_   ( sword::pinned_down )
    , nOperationalState_   ( sword::totally_destroyed )
    , nRoe_                ( sword::RulesOfEngagement::fire_upon_order )
    , app6symbol_          ( msg.app6symbol() )
    , decisionalModel_     ( type_.GetDecisionalModel().GetName() )
    , brainDebug_          ( false )
    , logMaintenanceManual_( msg.log_maintenance_manual() )
    , logSupplyManual_     ( msg.log_supply_manual() )
{
    if( ! parentFormation_ && ! parentAutomat_ )
        throw MASA_EXCEPTION( "invalid parent for automat " + msg.name() );
    knowledgeGroup_->Register( *this );
    if( msg.has_color() )
        color_.reset( new sword::RgbColor( msg.color() ) );
    if( parentFormation_ )
        parentFormation_->Register( *this );
    else if( parentAutomat_ )
        parentAutomat_->Register( *this );
    if( msg.has_extension() )
        for( int i = 0; i < msg.extension().entries_size(); ++i )
            extensions_[ msg.extension().entries( i ).name() ] = msg.extension().entries( i ).value();
    if( msg.logistic_level() != sword::none )
    {
        logisticEntity_.reset( new LogisticEntity( *this, model.Formations(), model.Automats() ) );
        AddExtension( *logisticEntity_ );
        AddExtension( logisticEntity_->GetLogisticHierarchy() );
    }
    else
        AddExtension( logisticHierarchy_ );
    AddExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    if( parentFormation_ )
    {
        MoveChildren( *parentFormation_ );
        parentFormation_->Remove( *this );
    }
    else if( parentAutomat_ )
    {
        MoveChildren( *parentAutomat_ );
        MoveAgents( *parentAutomat_ );
        parentAutomat_->Remove( *this );
    }
    knowledgeGroup_->Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat::MoveChildren
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
template< typename Superior >
void Automat::MoveChildren( Superior& superior )
{
    tools::Iterator< const dispatcher::Automat_ABC& > it( automats_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        dispatcher::Automat_ABC& entity = const_cast< dispatcher::Automat_ABC& >( it.NextElement() );
        entity.SetSuperior( superior );
    }
    automats_.Clear();
}

// -----------------------------------------------------------------------------
// Name: Automat::MoveAgents
// Created: SBO 2010-06-14
// -----------------------------------------------------------------------------
void Automat::MoveAgents( dispatcher::Automat_ABC& superior )
{
    tools::Iterator< const dispatcher::Agent_ABC& > it( agents_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        dispatcher::Agent_ABC& entity = const_cast< dispatcher::Agent_ABC& >( it.NextElement() );
        entity.SetSuperior( superior );
    }
    agents_.Clear();
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const sword::AutomatCreation& msg )
{
    ChangeKnowledgeGroup( msg.knowledge_group().id() );
    if( parentFormation_ &&
        ( msg.parent().has_automat()  ||
        ( msg.parent().has_formation() && msg.parent().formation().id() != parentFormation_->GetId() ) ) )
        ChangeSuperior( msg.parent() );
    if( parentAutomat_ && ( msg.parent().has_formation() || ( msg.parent().has_automat()  && msg.parent().automat().id()  != parentAutomat_->GetId() ) ) )
       ChangeSuperior( msg.parent() );
    decisionalInfos_.Clear();
}

// -----------------------------------------------------------------------------
// Name: Automat::DoUpdate
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const sword::AutomatChangeSuperior& msg )
{
    ChangeSuperior( msg.superior() );
}

// -----------------------------------------------------------------------------
// Name: Automat::Update
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const sword::AutomatChangeKnowledgeGroup& msg )
{
    ChangeKnowledgeGroup( msg.knowledge_group().id() );
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
template< typename Message >
void Automat::ChangeSuperior( const Message& superior )
{
    if( superior.has_formation() )
        SetSuperior( model_.Formations().Get( superior.formation().id() ) );
    else if( superior.has_automat() )
        SetSuperior( model_.Automats().Get( superior.automat().id() ) );
}

// -----------------------------------------------------------------------------
// Name: Automat::SetSuperior
// Created: SBO 2010-06-14
// -----------------------------------------------------------------------------
void Automat::SetSuperior( dispatcher::Formation_ABC& superior )
{
    ResetSuperior();
    superior.Register( *this );
    parentFormation_ = &superior;
}

// -----------------------------------------------------------------------------
// Name: Automat::SetSuperior
// Created: SBO 2010-06-14
// -----------------------------------------------------------------------------
void Automat::SetSuperior( dispatcher::Automat_ABC& superior )
{
    ResetSuperior();
    superior.Register( *this );
    parentAutomat_ = &superior;
}

// -----------------------------------------------------------------------------
// Name: Automat::ResetSuperior
// Created: SBO 2010-06-14
// -----------------------------------------------------------------------------
void Automat::ResetSuperior()
{
    if( parentFormation_ )
    {
        parentFormation_->Remove( *this );
        parentFormation_ = 0;
    }
    else if( parentAutomat_ )
    {
        parentAutomat_->Remove( *this );
        parentAutomat_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::DoUpdate
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const sword::AutomatAttributes& msg )
{
    if( msg.has_mode()  )
        nAutomatState_ = msg.mode();
    if( msg.has_force_ratio()  )
        nForceRatioState_ = msg.force_ratio();
    if( msg.has_meeting_engagement()  )
        nCloseCombatState_ = msg.meeting_engagement();
    if( msg.has_operational_state()  )
        nOperationalState_ = msg.operational_state();
    if( msg.has_roe()  )
        nRoe_ = msg.roe();
    if( msg.has_extension() )
        for( int i = 0; i < msg.extension().entries_size(); ++i )
            extensions_[ msg.extension().entries( i ).name() ] = msg.extension().entries( i ).value();
    if( msg.has_decisional_model() )
        decisionalModel_ = msg.decisional_model();
    if( msg.has_brain_debug() )
        brainDebug_ = msg.brain_debug();
    if( msg.has_log_maintenance_manual() )
        logMaintenanceManual_ = msg.log_maintenance_manual();
    if( msg.has_log_supply_manual() )
        logSupplyManual_ = msg.log_supply_manual();
    if( msg.has_name() )
        SetName( QString::fromStdString( msg.name().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: Automat::DoUpdate
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const sword::DecisionalState& asnMsg )
{
    decisionalInfos_.Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Automat::DoUpdate
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Automat::DoUpdate( const sword::AutomatOrder& message )
{
    order_.reset();
    if( message.type().id() != 0 )
        order_.reset( new AutomatOrder( message ) );
}

// -----------------------------------------------------------------------------
// Name: Automat::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Automat::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::AutomatCreation msg;
    msg().mutable_automat()->set_id( GetId() );
    msg().mutable_type()->set_id( type_.GetId() );
    msg().set_name( GetName().toStdString() );
    msg().mutable_party()->set_id( team_.GetId() );
    msg().mutable_knowledge_group()->set_id( knowledgeGroup_->GetId() );
    msg().set_app6symbol( app6symbol_ );
    msg().set_symbol( app6symbol_ );
    if( parentFormation_ )
        msg().mutable_parent()->mutable_formation()->set_id( parentFormation_->GetId() );
    if( parentAutomat_ )
        msg().mutable_parent()->mutable_automat()->set_id( parentAutomat_->GetId() );
    if( logisticEntity_.get() )
        logisticEntity_->Send( msg() );
    else
        msg().set_logistic_level( sword::none );
    if( color_ )
        *msg().mutable_color() = *color_;
    for( auto it = extensions_.begin(); it !=  extensions_.end(); ++it )
    {
        sword::Extension_Entry* entry = msg().mutable_extension()->add_entries();
        entry->set_name( it->first );
        entry->set_value( it->second );
    }
    msg().set_log_maintenance_manual( logMaintenanceManual_ );
    msg().set_log_supply_manual( logSupplyManual_ );
    msg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Automat::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Automat::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    {
        client::AutomatAttributes msg;
        msg().mutable_automat()->set_id( GetId() );
        msg().set_mode( nAutomatState_ );
        msg().set_force_ratio( nForceRatioState_);
        msg().set_meeting_engagement( nCloseCombatState_);
        msg().set_operational_state( nOperationalState_ );
        msg().set_roe( nRoe_ );
        for( std::map< std::string, std::string >::const_iterator it = extensions_.begin(); it !=  extensions_.end(); ++it )
        {
            sword::Extension_Entry* entry = msg().mutable_extension()->add_entries();
            entry->set_name( it->first );
            entry->set_value( it->second );
        }
        msg().set_decisional_model( decisionalModel_ );
        msg().set_brain_debug( brainDebug_ );
        msg().set_log_maintenance_manual( logMaintenanceManual_ );
        msg().set_log_supply_manual( logSupplyManual_ );
        msg().set_name( GetName().toStdString() );
        msg.Send( publisher );
    }
    if( order_.get() )
        order_->Send( publisher );
    else
        AutomatOrder::SendNoMission( *this, publisher );

    decisionalInfos_.Send( GetId(), publisher );

    if( logisticEntity_.get() )
        logisticEntity_->SendFullUpdate( publisher );
    else
        logisticHierarchy_.SendFullUpdate( publisher );
}

// -----------------------------------------------------------------------------
// Name: Automat::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Automat::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::AutomatDestruction asn;
    asn().mutable_automat()->set_id( GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Automat::Send
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
void Automat::Send( sword::ParentEntity& msg ) const
{
    msg.mutable_automat()->set_id( GetId() );
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
// Name: Automat::IsEngaged
// Created: SBO 2008-07-10
// -----------------------------------------------------------------------------
bool Automat::IsEngaged() const
{
    return nAutomatState_ == sword::engaged;
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
dispatcher::Automat_ABC* Automat::GetParentAutomat() const
{
    return parentAutomat_;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetFormation
// Created: MGD 2009-12-23
// -----------------------------------------------------------------------------
dispatcher::Formation_ABC* Automat::GetFormation() const
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
        throw MASA_EXCEPTION( "automat without a knowledge group." );
    return *knowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetLogisticEntity
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
LogisticEntity_ABC* Automat::GetLogisticEntity() const
{
    return logisticEntity_.get();
}

// -----------------------------------------------------------------------------
// Name: Automat::GetExtension
// Created: MMC 2012-08-10
// -----------------------------------------------------------------------------
bool Automat::GetExtension( const std::string& key, std::string& result ) const
{
    auto it = extensions_.find( key );
    if( it == extensions_.end() )
        return false;
    result = it->second;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetApp6Symbol
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
const std::string& Automat::GetApp6Symbol() const
{
    return app6symbol_;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetType
// Created: AHC 2012-10-11
// -----------------------------------------------------------------------------
const kernel::AutomatType& Automat::GetType() const
{
    return type_;
}
