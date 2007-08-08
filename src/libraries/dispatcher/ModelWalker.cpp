// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ModelWalker.h"
#include "ModelDataVisitor_ABC.h"
#include "Model.h"
#include "Agent.h"
#include "AgentKnowledge.h"
#include "KnowledgeGroup.h"
#include "Side.h"
#include "Automat.h"
#include "Formation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ModelWalker constructor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
ModelWalker::ModelWalker( const Model& model )
    : model_ ( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelWalker destructor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
ModelWalker::~ModelWalker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelWalker::Send
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ModelWalker::Send( const ASN1T_MsgsSimToClient& asn )
{
    switch ( asn.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_unit_creation:               WalkTo( *asn.msg.u.msg_unit_creation ); break;
    case T_MsgsSimToClient_msg_msg_automat_creation:           WalkTo( *asn.msg.u.msg_automat_creation ); break;
    case T_MsgsSimToClient_msg_msg_side_creation:               WalkTo( *asn.msg.u.msg_side_creation ); break;
    case T_MsgsSimToClient_msg_msg_limit_creation:              WalkTo( asn.msg.u.msg_limit_creation->tactical_line ); break;
    case T_MsgsSimToClient_msg_msg_lima_creation:               WalkTo( asn.msg.u.msg_limit_creation->tactical_line ); break;
    case T_MsgsSimToClient_msg_msg_formation_creation:          WalkTo( *asn.msg.u.msg_formation_creation ); break;
    case T_MsgsSimToClient_msg_msg_knowledge_group_creation:    WalkTo( *asn.msg.u.msg_knowledge_group_creation ); break;
    case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:     WalkTo( *asn.msg.u.msg_unit_knowledge_creation ); break;
    }
}

// -----------------------------------------------------------------------------
// Name: ModelWalker::Visit
// Created: JCR 2007-06-08
// -----------------------------------------------------------------------------
void ModelWalker::Visit( ModelDataVisitor_ABC& visitor, Entity_ABC& entity )
{
    visitor_ = &visitor;
    visitor_->Visit( entity );
}

// -----------------------------------------------------------------------------
// Name: ModelWalker::WalkTo
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ModelWalker::WalkTo( const ASN1T_MsgUnitCreation& asn )
{
    if( ! visitor_->Accept( asn ) )
        visitor_->Visit( model_.GetAutomats().Get( asn.oid_automate ) );
}

// -----------------------------------------------------------------------------
// Name: ModelWalker::WalkTo
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void ModelWalker::WalkTo( const ASN1T_MsgObjectCreation& asn )
{
    if( ! visitor_->Accept( asn ) )
        visitor_->Visit( const_cast< Model& >( model_ ).GetSides().Get( asn.team ) );
}

// -----------------------------------------------------------------------------
// Name: ModelWalker::WalkTo
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void ModelWalker::WalkTo( const ASN1T_MsgUnitKnowledgeUpdate& asn )
{
    if( ! visitor_->Accept( asn ) )
    {
        visitor_->Visit( model_.GetAgentKnowledges().Get( asn.oid ) );
        visitor_->Visit( model_.GetKnowledgeGroups().Get( asn.oid_groupe_possesseur ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelWalker::WalkTo
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void ModelWalker::WalkTo( const ASN1T_MsgUnitKnowledgeCreation& asn )
{
    if( !visitor_->Accept( asn ) )
        visitor_->Visit( model_.GetKnowledgeGroups().Get( asn.oid_groupe_possesseur ) );
}

// -----------------------------------------------------------------------------
// Name: ModelWalker::WalkTo
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void ModelWalker::WalkTo( const ASN1T_MsgKnowledgeGroupCreation& asn )
{
    if( ! visitor_->Accept( asn ) )
        visitor_->Visit( model_.GetSides().Get( asn.oid_camp ) );
}


// -----------------------------------------------------------------------------
// Name: ModelWalker::WalkTo
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ModelWalker::WalkTo( const ASN1T_MsgAutomatCreation& asn )
{
    if( ! visitor_->Accept( asn ) )
    {
        visitor_->Visit( model_.GetSides().Get( asn.oid_camp ) );
        visitor_->Visit( model_.GetFormations().Get( asn.oid_formation ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelWalker::WalkTo
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void ModelWalker::WalkTo( const ASN1T_MsgTeamCreation& asn )
{
    visitor_->Accept( asn );
}

// -----------------------------------------------------------------------------
// Name: ModelWalker::WalkTo
// Created: JCR 2007-05-16
// -----------------------------------------------------------------------------
void ModelWalker::WalkTo( const ASN1T_TacticalLine& asn )
{
    if( ! visitor_->Accept( asn ) )
    {
        if( asn.diffusion.t == T_TacticalLinesDiffusion_automat )
            visitor_->Visit( model_.GetAutomats().Get( asn.diffusion.u.automat ) );
        else if( asn.diffusion.t == T_TacticalLinesDiffusion_formation )
            visitor_->Visit( model_.GetFormations().Get( asn.diffusion.u.formation ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelWalker::WalkTo
// Created: JCR 2007-05-16
// -----------------------------------------------------------------------------
void ModelWalker::WalkTo( const ASN1T_MsgFormationCreation& asn )
{
    if( ! visitor_->Accept( asn ) )
        visitor_->Visit( model_.GetSides().Get( asn.oid_camp ) );
}
