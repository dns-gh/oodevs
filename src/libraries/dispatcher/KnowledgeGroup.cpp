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
#include "ModelVisitor_ABC.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( Model& model, const ASN1T_MsgKnowledgeGroupCreation& msg )
    : SimpleEntity< kernel::KnowledgeGroup_ABC >( msg.oid )
    , team_( model.sides_.Get( msg.oid_camp ) )
    , parent_( msg.m.oid_knowledgegroup_parentePresent ? &model.knowledgeGroups_.Get( msg.oid_knowledgegroup_parente ) : 0 )
{
    if( parent_ )
        parent_->knowledgeGroups_.Register( msg.oid, *this );
    else
        team_.knowledgeGroups_.Register( msg.oid, *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    // $$$ RDS : completement invalide si la formation parente a d�ja �t� detruite !!! 
    if( parent_ )
        parent_->knowledgeGroups_.Remove( GetId() );
    else
        team_.knowledgeGroups_.Remove( GetId() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void KnowledgeGroup::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::KnowledgeGroupCreation asn;
    
    asn().oid      = GetId();
    asn().oid_camp = team_.GetId();
    if( parent_ )
    {
        asn().m.oid_knowledgegroup_parentePresent = 1;
        asn().oid_knowledgegroup_parente = parent_->GetId();
    }
    asn.Send( publisher );
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
void KnowledgeGroup::Accept( ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    knowledgeGroups_.Apply( boost::bind( &KnowledgeGroup::Accept, _1, boost::ref( visitor ) ) );
}
