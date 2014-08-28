// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Formation.h"
#include "Model_ABC.h"
#include "Automat_ABC.h"
#include "Team_ABC.h"
#include "LogisticEntity.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "protocol/SimulationSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Formation::Formation( const Model_ABC& model, const sword::FormationCreation& msg )
    : Formation_ABC        ( msg.formation().id(), QString::fromStdString( msg.name() ) )
    , model_               ( model )
    , team_                ( model.Sides().Get( msg.party().id() ) )
    , level_               ( static_cast< E_NatureLevel >( msg.level() ) )
    , app6symbol_          ( msg.app6symbol() )
    , parent_              ( msg.has_parent() ? &model.Formations().Get( msg.parent().id() ) : 0 )
    , logMaintenanceManual_( msg.log_maintenance_manual() )
    , logSupplyManual_     ( msg.log_supply_manual() )
{
    if( parent_ )
        parent_->Register( *this );
    else
        team_.Register( *this );
    if( msg.has_color() )
        color_.reset( new sword::RgbColor( msg.color() ) );
    if( msg.has_extension() )
        for( int i = 0; i < msg.extension().entries_size(); ++i )
            extensions_[ msg.extension().entries( i ).name() ] = msg.extension().entries( i ).value();
    if( msg.logistic_level() != sword::none )
    {
        logisticEntity_.reset( new LogisticEntity( *this, model.Formations(), model.Automats() ) );
        AddExtension( *logisticEntity_ );
        AddExtension( logisticEntity_->GetLogisticHierarchy() );
    }
    AddExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    if( parent_ )
    {
        MoveChildren( *parent_, formations_ );
        MoveChildren( *parent_, automats_ );
        parent_->Remove( *this );
    }
    else
    {
        MoveChildren( team_, formations_ );
        // at this point, automats should be already deleted
        team_.Remove( *this );
    }
}

// -----------------------------------------------------------------------------
// Name: Formation::MoveChildren
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
template< typename Superior, typename Entity >
void Formation::MoveChildren( Superior& superior, tools::Resolver< Entity >& entities )
{
    tools::Iterator< const Entity& > it( entities.CreateIterator() );
    while( it.HasMoreElements() )
    {
        Entity& entity = const_cast< Entity& >( it.NextElement() );
        entity.SetSuperior( superior );
    }
    entities.Clear();
}

// -----------------------------------------------------------------------------
// Name: Formation::SetSuperior
// Created: SBO 2010-06-14
// -----------------------------------------------------------------------------
void Formation::SetSuperior( dispatcher::Formation_ABC& superior )
{
    if( parent_ )
        parent_->Remove( *this );
    else
        team_.Remove( *this );
    parent_ = &superior;
    parent_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation::SetSuperior
// Created: SBO 2010-06-14
// -----------------------------------------------------------------------------
void Formation::SetSuperior( dispatcher::Team_ABC& /*superior*/ )
{
    if( parent_ )
    {
        parent_->Remove( *this );
        parent_ = 0;
    }
    team_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Formation::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::FormationCreation message;
    message().mutable_formation()->set_id( GetId() );
    message().mutable_party()->set_id( team_.GetId() );
    message().set_level( sword::EnumNatureLevel( level_ ) );
    message().set_name( GetName().toStdString() );
    message().set_app6symbol( app6symbol_ );
    message().set_symbol( app6symbol_ );
    if( logisticEntity_.get() )
        logisticEntity_->Send( message() );
    else
        message().set_logistic_level( sword::none );
    if( color_ )
        *message().mutable_color() = *color_;
    if( parent_ )
        message().mutable_parent()->set_id( parent_->GetId() );
    for( auto it = extensions_.begin(); it !=  extensions_.end(); ++it )
    {
        sword::Extension_Entry* entry = message().mutable_extension()->add_entries();
        entry->set_name( it->first );
        entry->set_value( it->second );
    }
    message().set_log_maintenance_manual( logMaintenanceManual_ );
    message().set_log_supply_manual( logSupplyManual_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Formation::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Formation::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    {
        client::FormationUpdate asn;
        asn().mutable_formation()->set_id( GetId() );
        asn().set_log_maintenance_manual( logMaintenanceManual_ );
        asn().set_log_supply_manual( logSupplyManual_ );
        for( auto it = extensions_.begin(); it !=  extensions_.end(); ++it )
        {
            sword::Extension_Entry* entry = asn().mutable_extension()->add_entries();
            entry->set_name( it->first );
            entry->set_value( it->second );
        }
        asn().set_name( GetName().toStdString() );
        asn.Send( publisher );
    }
    if( logisticEntity_.get() )
        logisticEntity_->SendFullUpdate( publisher );
}

// -----------------------------------------------------------------------------
// Name: Formation::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Formation::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::FormationDestruction asn;
    asn().mutable_formation()->set_id( GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Formation::Send
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
void Formation::Send( sword::ParentEntity& msg ) const
{
    msg.mutable_formation()->set_id( GetId() );
}

// -----------------------------------------------------------------------------
// Name: Formation::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Formation::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    formations_.Apply( boost::bind( &dispatcher::Formation_ABC::Accept, _1, boost::ref( visitor ) ) );
    automats_.Apply( boost::bind( &dispatcher::Automat_ABC::Accept, _1, boost::ref( visitor ) ) );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLevel
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
E_NatureLevel Formation::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
dispatcher::Formation_ABC* Formation::GetParent() const
{
    return parent_;
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
dispatcher::Team_ABC& Formation::GetTeam() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Formation::Register( dispatcher::Formation_ABC& formation )
{
    formations_.Register( formation.GetId(), formation );
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Formation::Remove( dispatcher::Formation_ABC& formation )
{
    formations_.Remove( formation.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetFormations
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
const tools::Resolver< dispatcher::Formation_ABC >& Formation::GetFormations() const
{
    return formations_;
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Register( dispatcher::Automat_ABC& automat )
{
    automats_.Register( automat.GetId(), automat );
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Remove( dispatcher::Automat_ABC& automat )
{
    automats_.Remove( automat.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetAutomates
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
const tools::Resolver< dispatcher::Automat_ABC >& Formation::GetAutomates() const
{
    return automats_;
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLogisticEntity
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
LogisticEntity_ABC* Formation::GetLogisticEntity() const
{
    return logisticEntity_.get();
}

// -----------------------------------------------------------------------------
// Name: Formation::DoUpdate
// Created: ABR 2011-05-11
// -----------------------------------------------------------------------------
void Formation::DoUpdate( const sword::FormationUpdate& msg )
{
    if( msg.has_extension() )
        for( int i = 0; i < msg.extension().entries_size(); ++i )
            extensions_[ msg.extension().entries( i ).name() ] = msg.extension().entries( i ).value();
    if( msg.has_log_maintenance_manual() )
        logMaintenanceManual_ = msg.log_maintenance_manual();
    if( msg.has_log_supply_manual() )
        logSupplyManual_ = msg.log_supply_manual();
    if( msg.has_name() )
        SetName( QString::fromStdString( msg.name() ) );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetExtension
// Created: MMC 2012-08-10
// -----------------------------------------------------------------------------
bool Formation::GetExtension( const std::string& key, std::string& result ) const
{
    auto it = extensions_.find( key );
    if( it == extensions_.end() )
        return false;
    result = it->second;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Formation::GetApp6Symbol
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
const std::string& Formation::GetApp6Symbol() const
{
    return app6symbol_;
}

// -----------------------------------------------------------------------------
// Name: Formation::DoUpdate
// Created: AHC 2013-01-14
// -----------------------------------------------------------------------------
void Formation::DoUpdate( const sword::FormationChangeSuperior&  msg )
{
    if( msg.superior().has_formation() )
        SetSuperior( model_.Formations().Get( msg.superior().formation().id() ) );
    else
        SetSuperior( model_.Sides().Get( msg.superior().party().id() ) );
}
