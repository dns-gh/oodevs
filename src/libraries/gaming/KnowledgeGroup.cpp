// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "KnowledgeGroup.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/ActionController.h"

using namespace kernel;

IDManager KnowledgeGroup::idManager_( 0 );

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( unsigned long nId, Controller& controller, const Team_ABC& team )
    : controller_( controller )
    , team_( team )
    , nID_ ( nId )
{
    idManager_.LockIdentifier( nID_ );
    controller_.Create( *(KnowledgeGroup_ABC*)this );
    name_ = QString( "Gtia %1" ).arg( nID_ ); // $$$$ AGE 2006-08-23: 
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    controller_.Delete( *(KnowledgeGroup_ABC*)this );
    idManager_.ReleaseIdentifier( nID_ );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::AddAutomat
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void KnowledgeGroup::AddAutomat( unsigned long id, Agent_ABC& automat )
{
    Resolver< Agent_ABC >::Register( id, automat );
    controller_.Update( *(KnowledgeGroup_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::RemoveAutomat
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void KnowledgeGroup::RemoveAutomat( unsigned long id )
{
    Resolver< Agent_ABC >::Remove( id );
    controller_.Update( *(KnowledgeGroup_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetId
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
unsigned long KnowledgeGroup::GetId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetName
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
QString KnowledgeGroup::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetTeam
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
const Team_ABC& KnowledgeGroup::GetTeam() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::IsInTeam
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsInTeam( const Team_ABC& team ) const
{
    return & team_ == & team;
}
