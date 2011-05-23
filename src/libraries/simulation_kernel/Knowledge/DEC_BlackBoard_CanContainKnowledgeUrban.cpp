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
#include "DEC_Knowledge_Urban.h"
#include "MIL_AgentServer.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "MT_Tools/MT_ScipioException.h"
#include <urban/Model.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/TerrainObjectVisitor_ABC.h>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgeUrban )

namespace
{
    class UrbanBlockKnowledgeCreator : public urban::TerrainObjectVisitor_ABC
    {
    public:
        UrbanBlockKnowledgeCreator( DEC_BlackBoard_CanContainKnowledgeUrban::T_KnowledgeUrbanMap& elements,
            std::map< unsigned int, boost::shared_ptr< DEC_Knowledge_Urban > >& knowledgeElements, const MIL_Army_ABC& army,
            std::vector< UrbanObjectWrapper* >& urbanBlocks )
            : elements_         ( elements )
            , knowledgeElements_( knowledgeElements )
            , army_             ( army )
            , urbanBlocks_      ( urbanBlocks )
        {
            // NOTHING
        }

        virtual ~UrbanBlockKnowledgeCreator()
        {
            // NOTHING
        }

        virtual void VisitBlock( urban::TerrainObject_ABC& object )
        {
            try
            {
                UrbanObjectWrapper& wrapper = MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( object );
                boost::shared_ptr< DEC_Knowledge_Urban > knowledge( new DEC_Knowledge_Urban( army_, wrapper ) );
                elements_[ wrapper.GetID() ] = knowledge;
                knowledgeElements_[ knowledge->GetID() ] = knowledge;
                urbanBlocks_.push_back( &wrapper );
            }
            catch( ... )
            {
                // object not wrapped
            }
        }

    private:
        DEC_BlackBoard_CanContainKnowledgeUrban::T_KnowledgeUrbanMap& elements_;
        std::map< unsigned, boost::shared_ptr< DEC_Knowledge_Urban > >& knowledgeElements_;
        std::vector< UrbanObjectWrapper* >& urbanBlocks_;
        const MIL_Army_ABC& army_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban constructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeUrban::DEC_BlackBoard_CanContainKnowledgeUrban( const MIL_Army_ABC& army, bool fromCheckpoint /*= false*/ )
    : army_( army )
{
    if( !fromCheckpoint && MIL_AgentServer::IsInitialized() )
    {
        UrbanBlockKnowledgeCreator visitor( urbanMapFromConcrete_, urbanKnowledgeMapFromKnowledgeId_, army, urbanBlocks_ );
        MIL_AgentServer::GetWorkspace().GetUrbanModel().Accept( visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban destructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeUrban::~DEC_BlackBoard_CanContainKnowledgeUrban()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::load
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrban::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    std::size_t size = 0;
    file >> size;
    for( unsigned int i = 0; i < size; ++i )
    {
        boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge;
        DEC_Knowledge_Urban* knowledge;
        file >> knowledge;
        urbanMapFromConcrete_[ knowledge->GetObjectKnownId() ].reset( knowledge );
        urbanKnowledgeMapFromKnowledgeId_[ knowledge->GetID() ].reset( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::save
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrban::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    std::size_t size = urbanMapFromConcrete_.size();
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
boost::shared_ptr< DEC_Knowledge_Urban > DEC_BlackBoard_CanContainKnowledgeUrban::CreateKnowledgeUrban( const MIL_Army_ABC& army, const UrbanObjectWrapper& object )
{
    boost::shared_ptr< DEC_Knowledge_Urban > knowledge ( new DEC_Knowledge_Urban( army, object ) );
    if( !urbanMapFromConcrete_.insert( std::make_pair( object.GetID(), knowledge ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    if( !urbanKnowledgeMapFromKnowledgeId_.insert( std::make_pair( knowledge->GetID(), knowledge ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban::DestroyKnowledgeUrban
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeUrban::DestroyKnowledgeUrban( DEC_Knowledge_Urban& knowledge )
{
    if( urbanMapFromConcrete_.erase( knowledge.GetObjectKnownId() ) < 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
    if( urbanKnowledgeMapFromKnowledgeId_.erase( knowledge.GetID() ) < 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban::GetKnowledgeUrban
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Urban > DEC_BlackBoard_CanContainKnowledgeUrban::GetKnowledgeUrban( const UrbanObjectWrapper& object ) const
{
    CIT_KnowledgeUrbanMap it = urbanMapFromConcrete_.find( object.GetID() );
    if( it == urbanMapFromConcrete_.end() )
        return boost::shared_ptr< DEC_Knowledge_Urban >();
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeObject::GetKnowledgeUrbanFromID
// Created: SLG 2010-02-01
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Urban > DEC_BlackBoard_CanContainKnowledgeUrban::GetKnowledgeUrbanFromID( unsigned int nMosID ) const
{
    std::map< unsigned, boost::shared_ptr< DEC_Knowledge_Urban > >::const_iterator itKnowledge = urbanKnowledgeMapFromKnowledgeId_.find( nMosID );
    return itKnowledge == urbanKnowledgeMapFromKnowledgeId_.end() ? boost::shared_ptr< DEC_Knowledge_Urban >( ) : itKnowledge->second;
}
