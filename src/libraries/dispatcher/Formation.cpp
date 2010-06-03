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

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "Model_ABC.h"

#include <boost/bind.hpp>
#include "protocol/simulationsenders.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;
////using namespace Common;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Formation::Formation( const Model_ABC& model, const Common::MsgFormationCreation& msg, const tools::Resolver_ABC< kernel::HierarchyLevel_ABC >& levels )
    : SimpleEntity< kernel::Formation_ABC >( msg.oid(), QString(msg.nom().c_str()) )
    , model_ ( model )
    , name_  ( msg.nom() )
    , team_  ( model.Sides().Get( msg.oid_camp() ) )
    , level_ ( levels.Get( msg.niveau() ) )
    , parent_( msg.has_oid_formation_parente() ? &model.Formations().Get( msg.oid_formation_parente() ) : 0 )
{
    if( parent_ )
        parent_->Register( *this );
    else
        team_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    if( parent_ )
        parent_->Remove( *this );
    else
        team_.Remove( *this );
    NotifyDestructionToChildFormations();
}

// -----------------------------------------------------------------------------
// Name: Formation::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Formation::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::FormationCreation message;

    message().set_oid( GetId() );
    message().set_oid_camp( team_.GetId() );
    message().set_nom( name_ );
    message().set_niveau( Common::EnumNatureLevel( level_.GetId() ) );

    if( parent_ )
        message().set_oid_formation_parente( parent_->GetId() );

    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Formation::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Formation::SendFullUpdate( ClientPublisher_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Formation::SendDestruction( ClientPublisher_ABC& ) const
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: Formation::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Formation::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    childFormations_.Apply( boost::bind( &kernel::Formation_ABC::Accept, _1, boost::ref( visitor ) ) );
    automats_.Apply( boost::bind( &kernel::Automat_ABC::Accept, _1, boost::ref( visitor ) ) );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLevel
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::HierarchyLevel_ABC& Formation::GetLevel() const
{
    return level_; 
}

// -----------------------------------------------------------------------------
// Name: Formation::GetParent
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
kernel::Formation_ABC* Formation::GetParent() const
{
    return parent_;
}

// -----------------------------------------------------------------------------
// Name: Formation::GetTeam
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
kernel::Team_ABC& Formation::GetTeam() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Formation::Register( kernel::Formation_ABC& formation )
{
    childFormations_.Register( formation.GetId(), formation );
}

// -----------------------------------------------------------------------------
// Name: Formation::Remove
// Created: MGD 2009-12-18
// -----------------------------------------------------------------------------
void Formation::Remove( kernel::Formation_ABC& formation )
{
    childFormations_.Remove( formation.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetFormations
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
const tools::Resolver< kernel::Formation_ABC >& Formation::GetFormations() const
{
    return childFormations_;
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Register( kernel::Automat_ABC& automat )
{
    automats_.Register( automat.GetId(), automat );
}

// -----------------------------------------------------------------------------
// Name: Formation::Remove
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Remove( kernel::Automat_ABC& automat )
{
    automats_.Remove( automat.GetId() );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetAutomates
// Created: MGD 2009-12-22
// -----------------------------------------------------------------------------
const tools::Resolver< kernel::Automat_ABC >& Formation::GetAutomates() const
{
    return automats_;
}

// -----------------------------------------------------------------------------
// Name: Formation::NotifyDestructionToChildFormations
// Created: RPD 2010-05-31
// -----------------------------------------------------------------------------
void Formation::NotifyDestructionToChildFormations()
{
    childFormations_.Apply( boost::bind( &kernel::Formation_ABC::NotifyParentDestroyed, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: Formation::NotifyParentDestroyed()
// Created: RPD 2010-05-31
// -----------------------------------------------------------------------------
void Formation::NotifyParentDestroyed()
{
    parent_ = 0;
}
