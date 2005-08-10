// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Team.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:02 $
// $Revision: 3 $
// $Workfile: Team.cpp $
//
// *****************************************************************************

#include "Tester_Pch.h"
#include "Team.h"
#include "Knowledges/ObjectKnowledge.h"

using namespace TEST;

Team::T_TeamMap Team::teams_;

//-----------------------------------------------------------------------------
// Name: Team::Team
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Team::Team( T_EntityId nSimId, DIN::DIN_Input& input )
    : nId_          ( nSimId )
    , knownObjects_ ()
{
    input >> strName_;
}

//-----------------------------------------------------------------------------
// Name: Team::~Team
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
Team::~Team()
{
    knownObjects_.clear();
    relations_   .clear();
}

//-----------------------------------------------------------------------------
// Name: Team::Initialize
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void Team::Initialize()
{
}

//-----------------------------------------------------------------------------
// Name: Team::Terminate
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void Team::Terminate()
{
    for( CIT_TeamMap it = teams_.begin(); it != teams_.end(); ++it )
        delete it->second;
    teams_.clear();
}

// -----------------------------------------------------------------------------
// Name: Team::IsAFriend
// Created: SBO 2005-05-13
// -----------------------------------------------------------------------------
E_Tristate Team::IsAFriend( const Team& team ) const
{
    E_Diplomacy nRelation = GetRelationWith( team );
    switch( nRelation )
    {
        case eUnknown : return eTristate_DontKnow;
        case eFriend  : return eTristate_True;
        case eEnemy   : return eTristate_False;
        case eNeutral : return eTristate_False;
        default:
            assert( false );
    };
    return eTristate_DontKnow;
}

// -----------------------------------------------------------------------------
// Name: Team::IsAnEnemy
// Created: SBO 2005-05-13
// -----------------------------------------------------------------------------
E_Tristate Team::IsAnEnemy( const Team& team ) const
{
    E_Diplomacy nRelation = GetRelationWith( team );
    switch( nRelation )
    {
        case eUnknown : return eTristate_DontKnow;
        case eFriend  : return eTristate_False;
        case eEnemy   : return eTristate_True;
        case eNeutral : return eTristate_False;
        default:
            assert( false );
    };
    return eTristate_DontKnow;
}

// -----------------------------------------------------------------------------
// Name: Team::IsNeutral
// Created: SBO 2005-05-13
// -----------------------------------------------------------------------------
E_Tristate Team::IsNeutral( const Team& team ) const
{
    E_Diplomacy nRelation = GetRelationWith( team );
    switch( nRelation )
    {
        case eUnknown : return eTristate_DontKnow;
        case eFriend  : return eTristate_False;
        case eEnemy   : return eTristate_False;
        case eNeutral : return eTristate_True;
        default:
            assert( false );
    };
    return eTristate_DontKnow;
}

// -----------------------------------------------------------------------------
// Name: Team::SetRelation
// Created: SBO 2005-05-16
// -----------------------------------------------------------------------------
void Team::SetRelation( Team& otherTeam, ASN1T_EnumDiplomatie diplomacy )
{
    switch( diplomacy )
    {
        case EnumDiplomatie::inconnu:
            relations_[ &otherTeam ] = eUnknown;
            break;
        case EnumDiplomatie::ami:
            relations_[ &otherTeam ] = eFriend;
            break;
        case EnumDiplomatie::ennemi:
            relations_[ &otherTeam ] = eEnemy;
            break;
        case EnumDiplomatie::neutre:
            relations_[ &otherTeam ] = eNeutral;
            break;
        default:
            break;
    }
}

// -----------------------------------------------------------------------------
// Name: Team::OnReceiveMsgObjectKnowledgeCreation
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Team::OnReceiveMsgObjectKnowledgeCreation( const ASN1T_MsgObjectKnowledgeCreation& asnMsg )
{
    if( knownObjects_.find( asnMsg.oid_connaissance ) != knownObjects_.end() )
        return;
    ObjectKnowledge* pKnownObject = new ObjectKnowledge( asnMsg, *this );
    bool bOut = knownObjects_.insert( std::make_pair( pKnownObject->GetId(), pKnownObject ) ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: Team::OnReceiveMsgObjectKnowledgeUpdate
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Team::OnReceiveMsgObjectKnowledgeUpdate( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: Team::OnReceiveMsgObjectKnowledgeDestruction
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Team::OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg )
{
    knownObjects_.erase( asnMsg.oid_connaissance );
}

// -----------------------------------------------------------------------------
// Name: Team::GetTestParam_Objects
// Created: SBO 2005-08-10
// -----------------------------------------------------------------------------
T_IdVector& Team::GetTestParam_Objects() const
{
    return *new T_IdVector();
}
