// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "KnowledgeGroup.h"
#include "Side.h"
#include "Model.h"
#include "ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( Model_ABC& model, const MsgsSimToClient::MsgKnowledgeGroupCreation& msg )
    : SimpleEntity< kernel::KnowledgeGroup_ABC >( msg.oid() )
    , team_( model.Sides().Get( msg.oid_camp() ) )
    , parent_( msg.has_oid_parent() ? &model.KnowledgeGroups().Get( msg.oid_parent() ) : 0 )
    , type_( msg.type() ) // LTO
{
    // LTO begin
    if( parent_ == this )
        throw std::runtime_error( __FUNCTION__ ": recursive hierarchy." );
    if( parent_ )
        parent_->Register( *this );
    else
    // LTO end
        team_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    // $$$ RDS : completement invalide si la formation parente a d�ja �t� detruite !!! 
    if( parent_ )
        parent_->Remove( *this );
    else
        team_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void KnowledgeGroup::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::KnowledgeGroupCreation message;
    
    message().set_oid( GetId() );
    message().set_oid_camp( team_.GetId() );
    // LTO begin
    message().set_type( type_ );
    if( parent_ )
        message().set_oid_parent( parent_->GetId() );
    // LTO end
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SendFullUpdate
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void KnowledgeGroup::SendFullUpdate( ClientPublisher_ABC& /*publisher*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void KnowledgeGroup::SendDestruction( ClientPublisher_ABC& ) const
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void KnowledgeGroup::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    knowledgeGroups_.Apply( boost::bind( &KnowledgeGroup_ABC::Accept, _1, boost::ref( visitor ) ) ); // LTO
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Register
// Created: MGD 2009-12-21
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::Register( kernel::KnowledgeGroup_ABC& knowledgeGroup )
{
    knowledgeGroups_.Register( knowledgeGroup.GetId(), knowledgeGroup );
}
// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Register
// Created: MGD 2009-12-21
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::Remove( kernel::KnowledgeGroup_ABC& knowledgeGroup )
{
    knowledgeGroups_.Remove( knowledgeGroup.GetId() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Register
// Created: MGD 2009-12-21
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::Register( kernel::Automat_ABC& automat )
{
    automats_.Register( automat.GetId(), automat );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Register
// Created: MGD 2009-12-21
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::Remove( kernel::Automat_ABC& automat )
{
    automats_.Remove( automat.GetId() );
}
