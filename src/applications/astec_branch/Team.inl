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
// $Archive: /MVW_v10/Build/SDK/Light2/src/Team.inl $
// $Author: Ape $
// $Modtime: 27/10/04 12:00 $
// $Revision: 7 $
// $Workfile: Team.inl $
//
// *****************************************************************************


#include "AgentKnowledge.h"
#include "ObjectKnowledge.h"
#include "KnowledgeGroup.h"


// -----------------------------------------------------------------------------
// Name: Team::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
uint Team::GetID() const
{
    return nID_;
}


// -----------------------------------------------------------------------------
// Name: Team::GetName
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
const std::string& Team::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: Team::SetIdx
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
void Team::SetIdx( uint nIdx )
{
    nIdx_ = nIdx;    
}


// -----------------------------------------------------------------------------
// Name: Team::GetIdx
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
uint Team::GetIdx() const
{
    return nIdx_;    
}


// -----------------------------------------------------------------------------
// Name: Team::GetObjectKnowledges
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
const Team::T_ObjectKnowledgeMap& Team::GetObjectKnowledges() const
{
    return objectKnowledges_;    
}


// -----------------------------------------------------------------------------
// Name: Team::FindObjectKnowledge
/** @param  nId 
    @return 
*/
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
inline
ObjectKnowledge* Team::FindObjectKnowledge( int nId )
{
    IT_ObjectKnowledgeMap it = objectKnowledges_.find( nId );    
    if( it != objectKnowledges_.end() )
        return it->second;
    return 0;
}


// -----------------------------------------------------------------------------
// Name: Team::GetAgentKnowledges
/** @return 
*/
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
inline
const Team::T_AgentKnowledgeMap& Team::GetAgentKnowledges() const
{
    return agentKnowledges_;
}


// -----------------------------------------------------------------------------
// Name: Team::RegisterAgentKnowledge
/** @param  oid 
    @param  knowledge 
*/
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
inline
void Team::RegisterAgentKnowledge( AgentKnowledge& knowledge )
{
    assert( agentKnowledges_.find( knowledge.GetID() ) == agentKnowledges_.end() );
    agentKnowledges_.insert( std::make_pair( knowledge.GetID(), &knowledge ) );
}


// -----------------------------------------------------------------------------
// Name: Team::RemoveAgentKnowledge
/** @param  oid 
*/
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
inline
void Team::RemoveAgentKnowledge( AgentKnowledge& knowledge )
{
    IT_AgentKnowledgeMap itAgentKnowledge = agentKnowledges_.find( knowledge.GetID() );
    assert( itAgentKnowledge != agentKnowledges_.end() );
    agentKnowledges_.erase( itAgentKnowledge );
}

// -----------------------------------------------------------------------------
// Name: Team::RegisterKnowledgeGroup
/** @param  gtia 
*/
// Created: APE 2004-10-26
// -----------------------------------------------------------------------------
inline
void Team::RegisterKnowledgeGroup( KnowledgeGroup& gtia )
{
    gtia.SetTeam( *this );
    if( ! gtias_.insert( std::make_pair( gtia.GetID(), &gtia ) ).second )
        RUNTIME_ERROR;
}


// -----------------------------------------------------------------------------
// Name: Team::RemoveKnowledgeGroup
/** @param  gtia 
*/
// Created: APE 2004-10-26
// -----------------------------------------------------------------------------
inline
void Team::RemoveKnowledgeGroup( KnowledgeGroup& gtia )
{
    IT_KnowledgeGroupMap it = gtias_.find( gtia.GetID() );
    assert( it != gtias_.end() );
    gtias_.erase( it );
}


// -----------------------------------------------------------------------------
// Name: Team::DeleteAllKnowledgeGroups
// Created: APE 2004-10-26
// -----------------------------------------------------------------------------
inline
void Team::DeleteAllKnowledgeGroups()
{
    for( IT_KnowledgeGroupMap itKnowledgeGroup = gtias_.begin(); itKnowledgeGroup != gtias_.end(); ++itKnowledgeGroup )
        delete itKnowledgeGroup->second;
    gtias_.clear();
}


// -----------------------------------------------------------------------------
// Name: Team::FindKnowledgeGroup
/** @param  nID 
    @return 
    */
// Created: APE 2004-10-26
// -----------------------------------------------------------------------------
inline
KnowledgeGroup* Team::FindKnowledgeGroup( uint nID )
{
    IT_KnowledgeGroupMap itKnowledgeGroup = gtias_.find( nID );    
    if( itKnowledgeGroup != gtias_.end() )
        return itKnowledgeGroup->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Team::GetKnowledgeGroups
/** @return 
*/
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
inline
const Team::T_KnowledgeGroupMap& Team::GetKnowledgeGroups() const
{
    return gtias_;
}
