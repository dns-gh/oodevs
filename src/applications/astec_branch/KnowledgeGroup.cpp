// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/KnowledgeGroup.cpp $
// $Author: Ape $
// $Modtime: 24/11/04 10:03 $
// $Revision: 7 $
// $Workfile: KnowledgeGroup.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "KnowledgeGroup.h"
#include "Controller.h"
#include "Team.h"

IDManager KnowledgeGroup::idManager_( 0 );

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( unsigned long nId, Controller& controller, const Team& team )
    : controller_( controller )
    , team_( team )
    , nID_ ( nId )
{
    idManager_.LockIdentifier( nID_ );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    controller_.Delete( *this );
    idManager_.ReleaseIdentifier( nID_ );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::AddAutomat
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void KnowledgeGroup::AddAutomat( unsigned long id, Agent& automat )
{
    Resolver< Agent >::Register( id, automat );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::RemoveAutomat
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void KnowledgeGroup::RemoveAutomat( unsigned long id )
{
    Resolver< Agent >::Remove( id );
    controller_.Update( *this );
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
std::string KnowledgeGroup::GetName() const
{
    std::stringstream stream;
    stream << "Gtia " << nID_;
    return stream.str();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetTeam
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
const Team& KnowledgeGroup::GetTeam() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::IsInTeam
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsInTeam( const Team& team ) const
{
    return team_ == team;
}
