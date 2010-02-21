// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeObjectPerception.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 19:05 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeObjectPerception.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeObjectPerception.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "Entities/Objects/MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgeObjectPerception )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObjectPerception::DEC_BlackBoard_CanContainKnowledgeObjectPerception()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObjectPerception::~DEC_BlackBoard_CanContainKnowledgeObjectPerception()
{
    while( !knowledgeObjectPerceptionMap_.empty() )
        DestroyKnowledgeObjectPerception( *knowledgeObjectPerceptionMap_.begin()->second );        
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectPerception::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int nNbr;
    file >> nNbr;
    while ( nNbr-- )
    {
        MIL_Object_ABC* pObject;
        file >> pObject;
        file >> knowledgeObjectPerceptionMap_[ pObject ];
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectPerception::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    const T_KnowledgeObjectPerceptionMap::size_type size = knowledgeObjectPerceptionMap_.size();
    file << size;
    for ( CIT_KnowledgeObjectPerceptionMap it = knowledgeObjectPerceptionMap_.begin(); it != knowledgeObjectPerceptionMap_.end(); ++it )
    {
        file << it->first
             << it->second;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception::CreateKnowledgeObjectPerception
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectPerception& DEC_BlackBoard_CanContainKnowledgeObjectPerception::CreateKnowledgeObjectPerception( const MIL_AgentPion& agentPerceiving, MIL_Object_ABC& objectPerceived )
{
    DEC_Knowledge_ObjectPerception* pKnowledge = new DEC_Knowledge_ObjectPerception( agentPerceiving, objectPerceived );//$$ RAM   
    bool bOut = knowledgeObjectPerceptionMap_.insert( std::make_pair( &objectPerceived, pKnowledge ) ).second;
    assert( bOut );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception::DestroyKnowledgeObjectPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObjectPerception::DestroyKnowledgeObjectPerception( DEC_Knowledge_ObjectPerception& knowledge )
{
    int nOut = knowledgeObjectPerceptionMap_.erase( &knowledge.GetObjectPerceived() );
    assert( nOut == 1 );
    delete &knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObjectPerception::GetKnowledgesObjectPerception
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectPerception* DEC_BlackBoard_CanContainKnowledgeObjectPerception::GetKnowledgeObjectPerception( const MIL_Object_ABC& associatedObject ) const
{
    CIT_KnowledgeObjectPerceptionMap itKnowledge = knowledgeObjectPerceptionMap_.find( &associatedObject );
    if( itKnowledge != knowledgeObjectPerceptionMap_.end() )
        return itKnowledge->second;
    else
        return 0;
}