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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Team.inl $
// $Author: Ape $
// $Modtime: 27/10/04 12:00 $
// $Revision: 7 $
// $Workfile: MOS_Team.inl $
//
// *****************************************************************************


#include "MOS_AgentKnowledge.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_Gtia.h"


// -----------------------------------------------------------------------------
// Name: MOS_Team::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_Team::GetID() const
{
    return nID_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::GetName
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
MOS_INLINE
const std::string& MOS_Team::GetName() const
{
    return strName_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::SetIdx
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Team::SetIdx( uint nIdx )
{
    nIdx_ = nIdx;    
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::GetIdx
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_Team::GetIdx() const
{
    return nIdx_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::GetObjectKnowledges
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_Team::T_ObjectKnowledgeMap& MOS_Team::GetObjectKnowledges() const
{
    return objectKnowledges_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::FindObjectKnowledge
/** @param  nId 
    @return 
*/
// Created: APE 2004-09-10
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_ObjectKnowledge* MOS_Team::FindObjectKnowledge( int nId )
{
    IT_ObjectKnowledgeMap it = objectKnowledges_.find( nId );    
    if( it != objectKnowledges_.end() )
        return it->second;
    return 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::FindKnowledgeOnObject
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_ObjectKnowledge* MOS_Team::FindKnowledgeOnObject( const MOS_Object_ABC& object )
{
    for( IT_ObjectKnowledgeMap it = objectKnowledges_.begin(); it != objectKnowledges_.end(); ++it )
        if( it->second->GetRealObject() == &object )
            return it->second;

    return 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::GetAgentKnowledges
/** @return 
*/
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_Team::T_AgentKnowledgeMap& MOS_Team::GetAgentKnowledges() const
{
    return agentKnowledges_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::RegisterAgentKnowledge
/** @param  oid 
    @param  knowledge 
*/
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Team::RegisterAgentKnowledge( MOS_AgentKnowledge& knowledge )
{
    assert( agentKnowledges_.find( knowledge.GetID() ) == agentKnowledges_.end() );
    agentKnowledges_.insert( std::make_pair( knowledge.GetID(), &knowledge ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::RemoveAgentKnowledge
/** @param  oid 
*/
// Created: APE 2004-05-28
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Team::RemoveAgentKnowledge( MOS_AgentKnowledge& knowledge )
{
    IT_AgentKnowledgeMap itAgentKnowledge = agentKnowledges_.find( knowledge.GetID() );
    assert( itAgentKnowledge != agentKnowledges_.end() );
    agentKnowledges_.erase( itAgentKnowledge );
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::RegisterGtia
/** @param  gtia 
*/
// Created: APE 2004-10-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Team::RegisterGtia( MOS_Gtia& gtia )
{
    gtia.SetTeam( *this );
    bool bOut = gtias_.insert( std::make_pair( gtia.GetID(), &gtia ) ).second;
    assert( bOut );
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::RemoveGtia
/** @param  gtia 
*/
// Created: APE 2004-10-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Team::RemoveGtia( MOS_Gtia& gtia )
{
    IT_GtiaMap it = gtias_.find( gtia.GetID() );
    assert( it != gtias_.end() );
    gtias_.erase( it );
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::DeleteAllGtias
// Created: APE 2004-10-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Team::DeleteAllGtias()
{
    for( IT_GtiaMap itGtia = gtias_.begin(); itGtia != gtias_.end(); ++itGtia )
        delete itGtia->second;
    gtias_.clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_Team::FindGtia
/** @param  nID 
    @return 
    */
// Created: APE 2004-10-26
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Gtia* MOS_Team::FindGtia( uint nID )
{
    IT_GtiaMap itGtia = gtias_.find( nID );    
    if( itGtia != gtias_.end() )
        return itGtia->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_Team::GetGtias
/** @return 
*/
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_Team::T_GtiaMap& MOS_Team::GetGtias() const
{
    return gtias_;
}
