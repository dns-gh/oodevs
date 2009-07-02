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

#include "simulation_kernel_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_Object.h"
#include "Entities/MIL_Army.h"
#include "Entities/Objects/MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_GUID( DEC_BlackBoard_CanContainKnowledgeObject, "DEC_BlackBoard_CanContainKnowledgeObject" )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject::DEC_BlackBoard_CanContainKnowledgeObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject::~DEC_BlackBoard_CanContainKnowledgeObject()
{
    while( !knowledgeObjectFromIDMap_.empty() )
        DestroyKnowledgeObject( *knowledgeObjectFromIDMap_.begin()->second );            
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
            uint size = map.size();
            file << size;
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
                MIL_Object_ABC* pObject;
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
    file >> objectMap_
         >> knowledgeObjectFromIDMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << objectMap_
         << knowledgeObjectFromIDMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::CreateKnowledgeObject
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& DEC_BlackBoard_CanContainKnowledgeObject::CreateKnowledgeObject( const MIL_Army& teamKnowing, MIL_Object_ABC& objectKnown )
{
    DEC_Knowledge_Object& knowledge = objectKnown.CreateKnowledge( teamKnowing );
    
    bool bOut = objectMap_.insert( std::make_pair( &objectKnown, &knowledge ) ).second;
    assert( bOut );

    bOut = knowledgeObjectFromIDMap_.insert( std::make_pair( knowledge.GetID(), &knowledge ) ).second;
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
        int nOut = objectMap_.erase( knowledge.GetObjectKnown() );
        assert( nOut >= 1 );
    }

    int nOut = knowledgeObjectFromIDMap_.erase( knowledge.GetID() );
    assert( nOut >= 1 );
    delete &knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::NotifyKnowledgeObjectDissociatedFromRealObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::NotifyKnowledgeObjectDissociatedFromRealObject( const MIL_Object_ABC& objectKnown, DEC_Knowledge_Object& knowledge )
{
    assert( knowledge.GetObjectKnown() == 0 );
    int nOut = objectMap_.erase( &objectKnown );
    assert( nOut >= 1 );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObject
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObject( const MIL_Object_ABC& objectKnown ) const
{
    CIT_KnowledgeObjectMap itKnowledge = objectMap_.find( &objectKnown );   
    if( itKnowledge == objectMap_.end() )
        return 0;
    return itKnowledge->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgesObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgesObject( T_KnowledgeObjectVector& outContainer ) const
{
    outContainer.reserve( knowledgeObjectFromIDMap_.size() );
    for( CIT_KnowledgeObjectIDMap itKnowledge = knowledgeObjectFromIDMap_.begin(); itKnowledge != knowledgeObjectFromIDMap_.end(); ++itKnowledge )
        outContainer.push_back( itKnowledge->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeObjectFromID( uint nMosID ) const
{
    CIT_KnowledgeObjectIDMap itKnowledge = knowledgeObjectFromIDMap_.find( nMosID );
    return itKnowledge == knowledgeObjectFromIDMap_.end() ? 0 : itKnowledge->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::HasKnowledgeObject
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool DEC_BlackBoard_CanContainKnowledgeObject::HasKnowledgeObject( const MIL_Object_ABC& objectKnown ) const
{
    return objectMap_.find( &objectKnown ) != objectMap_.end();
}
