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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/EntityManager.cpp $
// $Author: Sbo $
// $Modtime: 7/07/05 15:09 $
// $Revision: 17 $
// $Workfile: EntityManager.cpp $
//
// *****************************************************************************

#include "Tester_pch.h"
#include "Types.h"

#include "Entities/EntityManager.h"
#include "Entities/KnowledgeGroup.h"
#include "Entities/Team.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"
#include "Entities/Object.h"
#include "Entities/Testable_Entity.h"
#include "Actions/Scheduler.h"

using namespace TEST;

//-----------------------------------------------------------------------------
// Name: EntityManager::EntityManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
EntityManager::EntityManager()
    : automats_        ()
    , pawns_           ()
    , teams_           ()
    , knowledgeGroups_ ()
    , objects_         ()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: EntityManager::~EntityManager
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
EntityManager::~EntityManager()
{
    for( CIT_AutomatMap it = automats_.begin(); it != automats_.end(); ++it )
        delete it->second;
    automats_.clear();
    for( CIT_PawnMap it = pawns_.begin(); it != pawns_.end(); ++it )
        delete it->second;
    pawns_.clear();

    for( CIT_TeamMap it = teams_.begin(); it != teams_.end(); ++it )
        delete it->second;
    teams_.clear();

    for( CIT_KnowledgeGroupMap it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        delete it->second;
    knowledgeGroups_.clear();

    for( CIT_ObjectMap it = objects_.begin(); it != objects_.end(); ++it )
        delete it->second;
    objects_.clear();
}

//-----------------------------------------------------------------------------
// Name: EntityManager::FindAutomat
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
Automat* EntityManager::FindAutomat( T_EntityId nId ) const
{
	CIT_AutomatMap it;
	it = automats_.find( nId );
	if ( it != automats_.end() )
		return ( *it ).second;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: EntityManager::Register
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void EntityManager::Register( Automat& automat )
{
    bool bOut = automats_.insert( std::make_pair( automat.GetId(), &automat ) ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: EntityManager::FindPawn
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
Pawn* EntityManager::FindPawn( T_EntityId nId ) const
{
	CIT_PawnMap it;
	it = pawns_.find( nId );
	if ( it != pawns_.end() )
		return ( *it ).second;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: EntityManager::Register
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void EntityManager::Register( Pawn& pawn )
{
    bool bOut = pawns_.insert( std::make_pair( pawn.GetId(), &pawn ) ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: EntityManager::FindTeam
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
Team* EntityManager::FindTeam( T_EntityId nId ) const
{
	CIT_TeamMap it;
	it = teams_.find( nId );
	if ( it != teams_.end() )
		return ( *it ).second;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: EntityManager::Register
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void EntityManager::Register( Team& team )
{
    bool bOut = teams_.insert( std::make_pair( team.GetId(), &team ) ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: EntityManager::Find
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
KnowledgeGroup* EntityManager::FindKnowledgeGroup( T_EntityId nId ) const
{
	CIT_KnowledgeGroupMap it;
	it = knowledgeGroups_.find( nId );
	if ( it != knowledgeGroups_.end() )
		return ( *it ).second;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: EntityManager::Register
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void EntityManager::Register( KnowledgeGroup& knowledgeGroup )
{
    bool bOut = knowledgeGroups_.insert( std::make_pair( knowledgeGroup.GetId(), &knowledgeGroup ) ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: EntityManager::Find
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
Object* EntityManager::FindObject( T_EntityId nId ) const
{
	CIT_ObjectMap it;
	it = objects_.find( nId );
	if ( it != objects_.end() )
		return ( *it ).second;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: EntityManager::Register
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void EntityManager::Register( Object& object )
{
    bool bOut = objects_.insert( std::make_pair( object.GetId(), &object ) ).second;
    assert( bOut );
}

//-----------------------------------------------------------------------------
// Name: EntityManager::Unregister
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
void EntityManager::Unregister( Object& object )
{
    int nOut = objects_.erase( object.GetId() );
    assert( nOut == 1 );
}

// -----------------------------------------------------------------------------
// Name: EntityManager::ScheduleAllPawnMissions
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void EntityManager::ScheduleAllPawnMissions( Scheduler& scheduler ) const
{
    for( CIT_PawnMap it = pawns_.begin(); it != pawns_.end(); ++it )
        it->second->ScheduleAllMissions( scheduler );
}

// -----------------------------------------------------------------------------
// Name: EntityManager::ScheduleAllAutomatMissions
// Created: SBO 2005-08-12
// -----------------------------------------------------------------------------
void EntityManager::ScheduleAllAutomatMissions( Scheduler& scheduler ) const
{
    for( CIT_AutomatMap it = automats_.begin(); it != automats_.end(); ++it )
        it->second->ScheduleAllMissions( scheduler );
}

// -----------------------------------------------------------------------------
// Name: EntityManager::GetTestParam_AgentList
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
T_IdVector& EntityManager::GetTestParam_AgentList( uint nNbr, const Testable_Entity& target ) const
{
    T_IdVector& agents = *new T_IdVector();
    for( CIT_PawnMap it = pawns_.begin(); nNbr > 0 && it != pawns_.end(); ++it )
        if( it->first != target.GetId() && 
            it->second->GetAutomat().GetKnowledgeGroup().GetId() == target.GetAutomat().GetKnowledgeGroup().GetId() )
        {
            agents.push_back( it->first );
            --nNbr;
        }
    return agents;
}

// -----------------------------------------------------------------------------
// Name: EntityManager::GetTestParam_AutomateList
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
T_IdVector& EntityManager::GetTestParam_AutomateList( uint nNbr, const Testable_Entity& target ) const
{
    T_IdVector& automats = *new T_IdVector();
    for( CIT_AutomatMap it = automats_.begin(); nNbr > 0 && it != automats_.end(); ++it )
        if( it->first != target.GetId() &&
            it->second->GetAutomat().GetKnowledgeGroup().GetId() == target.GetAutomat().GetKnowledgeGroup().GetId() )
        {
            automats.push_back( it->first );
            --nNbr;
        }
    return automats;
}
