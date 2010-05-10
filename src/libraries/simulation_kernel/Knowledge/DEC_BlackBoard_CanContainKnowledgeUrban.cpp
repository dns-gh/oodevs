// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeUrban.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_Urban.h"
#include "MIL_AgentServer.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "simulation_kernel/UrbanModel.h"
#include <urban/Block.h>
#include <urban/BlockModel.h>
#include <urban/Model.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/TerrainObjectVisitor_ABC.h>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgeUrban )

namespace
{
    class UrbanBlockKnowledgeCreator : public urban::TerrainObjectVisitor_ABC
                                     , public boost::noncopyable
    {
    public:
        UrbanBlockKnowledgeCreator( DEC_BlackBoard_CanContainKnowledgeUrban::T_KnowledgeUrbanMap& elements, const MIL_Army_ABC& army )
            : elements_( elements ), army_( army ) {}
        virtual ~UrbanBlockKnowledgeCreator() {}

        virtual void VisitBlock( const urban::TerrainObject_ABC& object )
        {
            elements_[ object.GetId() ] = boost::shared_ptr< DEC_Knowledge_Urban > ( new DEC_Knowledge_Urban( army_, object ) );
        }
        virtual void VisitCity( const urban::TerrainObject_ABC& object )
        {
            //NOTHING
        }
        virtual void VisitDistrict( const urban::TerrainObject_ABC& object )
        {
            //NOTHING
        }

    private:
        DEC_BlackBoard_CanContainKnowledgeUrban::T_KnowledgeUrbanMap& elements_;
        const MIL_Army_ABC& army_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban constructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeUrban::DEC_BlackBoard_CanContainKnowledgeUrban( const MIL_Army_ABC& army )
    : army_( army )
{
    UrbanBlockKnowledgeCreator visitor( urbanMapFromConcrete_, army );
    UrbanModel::GetSingleton().GetModel().Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban destructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeUrban::~DEC_BlackBoard_CanContainKnowledgeUrban()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::load
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrban::load( MIL_CheckPointInArchive& file, const uint )
{
    unsigned int size = 0;
    file >> size;
    for( unsigned int i = 0; i < size; ++i )
    {
        boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge;
        DEC_Knowledge_Urban* knowledge;
        file >> knowledge;
        urbanMapFromConcrete_[ knowledge->GetId() ].reset( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::save
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrban::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned int size = urbanMapFromConcrete_.size();
    file << size;
    for( CIT_KnowledgeUrbanMap it = urbanMapFromConcrete_.begin(); it != urbanMapFromConcrete_.end(); ++it )
    {
        DEC_Knowledge_Urban* knowledge = it->second.get();
        file << knowledge;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban::CreateKnowledgeUrban
// Created: MGD 2009-12-04
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Urban > DEC_BlackBoard_CanContainKnowledgeUrban::CreateKnowledgeUrban( const MIL_Army_ABC& army, const urban::TerrainObject_ABC& object )
{
    boost::shared_ptr< DEC_Knowledge_Urban > knowledge ( new DEC_Knowledge_Urban( army, object ) );
    if( ! urbanMapFromConcrete_.insert( std::make_pair( object.GetId(), knowledge ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban::DestroyKnowledgeUrban
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrban::DestroyKnowledgeUrban( DEC_Knowledge_Urban& knowledge )
{
    if( urbanMapFromConcrete_.erase( knowledge.GetId() ) < 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban::GetKnowledgeUrban
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Urban > DEC_BlackBoard_CanContainKnowledgeUrban::GetKnowledgeUrban( const urban::TerrainObject_ABC& object ) const
{
    CIT_KnowledgeUrbanMap it = urbanMapFromConcrete_.find( object.GetId() );
    if( it == urbanMapFromConcrete_.end() )
    {
        return boost::shared_ptr< DEC_Knowledge_Urban >();
    }  
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeUrbanFromID
// Created: SLG 2010-02-01
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Urban > DEC_BlackBoard_CanContainKnowledgeUrban::GetKnowledgeUrbanFromID( uint nMosID ) const
{
    CIT_KnowledgeUrbanMap itKnowledge = urbanMapFromConcrete_.find( nMosID );
    return itKnowledge == urbanMapFromConcrete_.end() ? boost::shared_ptr< DEC_Knowledge_Urban >( ) : itKnowledge->second;
}


// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban::HasKnowledgeUrban
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
bool DEC_BlackBoard_CanContainKnowledgeUrban::HasKnowledgeUrban( const urban::TerrainObject_ABC& object ) const
{
    return urbanMapFromConcrete_.find( object.GetId() ) != urbanMapFromConcrete_.end();
}