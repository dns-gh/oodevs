// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:38 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeObject.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"

#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_Object.h"
#include "DEC_Knowledge_ObjectSiteFranchissement.h"
#include "DEC_Knowledge_ObjectNuageNBC.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"

BOOST_CLASS_EXPORT_GUID( DEC_BlackBoard_CanContainKnowledgeObject, "DEC_BlackBoard_CanContainKnowledgeObject" )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject::DEC_BlackBoard_CanContainKnowledgeObject()
{
	
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject::~DEC_BlackBoard_CanContainKnowledgeObject()
{
    while( !knowledgeObjectFromDiaIDMap_.empty() )
        DestroyKnowledgeObject( *knowledgeObjectFromDiaIDMap_.begin()->second );        	
}

// =============================================================================
// CHECKPOINT
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, DEC_BlackBoard_CanContainKnowledgeObject::T_KnowledgeObjectMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const DEC_BlackBoard_CanContainKnowledgeObject::T_KnowledgeObjectMap& map, const uint )
        {
            file << map.size();
            for ( DEC_BlackBoard_CanContainKnowledgeObject::CIT_KnowledgeObjectMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;                
            }
        }
        
        template< typename Archive >
        void load( Archive& file, DEC_BlackBoard_CanContainKnowledgeObject::T_KnowledgeObjectMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_RealObject_ABC* pObject;
                file >> pObject;
                file >> map[ pObject ];
            }
        }
    }
}
// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> knowledgeObjectMap_
         >> knowledgeObjectFromMosIDMap_
         >> knowledgeObjectFromDiaIDMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << knowledgeObjectMap_
         << knowledgeObjectFromMosIDMap_
         << knowledgeObjectFromDiaIDMap_;
}


// =============================================================================
// OPERATIONS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::CreateKnowledgeObject
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& DEC_BlackBoard_CanContainKnowledgeObject::CreateKnowledgeObject( const MIL_Army& teamKnowing, MIL_RealObject_ABC& objectKnown )
{
    DEC_Knowledge_Object& knowledge = objectKnown.CreateKnowledge( teamKnowing );
    
    bool bOut = knowledgeObjectMap_.insert( std::make_pair( &objectKnown, &knowledge ) ).second;
    assert( bOut );

    bOut = knowledgeObjectFromMosIDMap_.insert( std::make_pair( knowledge.GetMosID(), &knowledge ) ).second;
    assert( bOut );

    bOut = knowledgeObjectFromDiaIDMap_.insert( std::make_pair( knowledge.GetDiaID(), &knowledge ) ).second;
    assert( bOut );

    return knowledge;
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::DestroyKnowledgeObject
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::DestroyKnowledgeObject( DEC_Knowledge_Object& knowledge )
{
    if( knowledge.GetObjectKnown() )
    {
        int nOut = knowledgeObjectMap_.erase( knowledge.GetObjectKnown() );
        assert( nOut >= 1 );
    }

    int nOut = knowledgeObjectFromMosIDMap_.erase( knowledge.GetMosID() );
    assert( nOut >= 1 );

    nOut = knowledgeObjectFromDiaIDMap_.erase( knowledge.GetDiaID() );
    assert( nOut >= 1 );
    delete &knowledge;
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::NotifyKnowledgeObjectDissociatedFromRealObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::NotifyKnowledgeObjectDissociatedFromRealObject( const MIL_RealObject_ABC& objectKnown, DEC_Knowledge_Object& knowledge )
{
    assert( knowledge.GetObjectKnown() == 0 );
    int nOut = knowledgeObjectMap_.erase( &objectKnown );
    assert( nOut >= 1 );
}