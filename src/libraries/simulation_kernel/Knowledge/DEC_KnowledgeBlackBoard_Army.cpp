// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard_Army.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:48 $
// $Revision: 2 $
// $Workfile: DEC_KnowledgeBlackBoard_Army.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeBlackBoard_Army.h"
#include "DEC_BlackBoard_CanContainKnowledgeUrban.h"
#include "DEC_KS_UrbanKnowledgeSynthetizer.h"
#include "DEC_Knowledge_Urban.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KnowledgeBlackBoard_Army )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army::DEC_KnowledgeBlackBoard_Army( MIL_Army_ABC& army )
    : DEC_KnowledgeBlackBoardObjects_ABC( 0 )
    , pArmy_( &army )
    , pKnowledgeUrbanContainer_( new DEC_BlackBoard_CanContainKnowledgeUrban( army ) )
    , pKsUrbanKnowledgeSynthetizer_( new DEC_KS_UrbanKnowledgeSynthetizer( *this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army::DEC_KnowledgeBlackBoard_Army()
    : DEC_KnowledgeBlackBoardObjects_ABC( 0 )
    , pArmy_( 0 )
    , pKnowledgeUrbanContainer_( 0 )
    , pKsUrbanKnowledgeSynthetizer_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army::~DEC_KnowledgeBlackBoard_Army()
{
    delete pKnowledgeUrbanContainer_;
    delete pKsUrbanKnowledgeSynthetizer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KnowledgeBlackBoard_Army::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeBlackBoardObjects_ABC >( *this )
            & pArmy_
            & pKnowledgeUrbanContainer_
            & pKsUrbanKnowledgeSynthetizer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::SendFullState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::SendFullState( unsigned int nCtx ) const
{
    DEC_KnowledgeBlackBoardObjects_ABC::SendFullState( nCtx );
    std::mem_fun_ref_t< void, DEC_Knowledge_Urban > functorUrban = std::mem_fun_ref( &DEC_Knowledge_Urban::SendStateToNewClient );
    pKnowledgeUrbanContainer_->ApplyOnKnowledgesUrbanRef( functorUrban );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::SendChangedState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::SendChangedState() const
{
    SendObjectChangedState();
    std::mem_fun_ref_t< void, DEC_Knowledge_Urban > functorUrban = std::mem_fun_ref( &DEC_Knowledge_Urban::UpdateOnNetwork );
    pKnowledgeUrbanContainer_->ApplyOnKnowledgesUrbanRef( functorUrban );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::Finalize
// Created: LMT 2011-07-21
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::Finalize()
{
    pKnowledgeUrbanContainer_->Finalize();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeAgent( const sword::UnitKnowledgeId& /*asn*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeAgent
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeAgent( const MIL_Agent_ABC& /*agent*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeAgent( unsigned int /*nID*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Population > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgePopulation( const sword::CrowdKnowledgeId& /*asn*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Population >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgePopulation
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Population > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgePopulation( const MIL_Population& /*population*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Population >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Population > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgePopulation( unsigned int /*nID*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Population >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeUrbanContainer
// Created: MGD 2009-12-02
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeUrban& DEC_KnowledgeBlackBoard_Army::GetKnowledgeUrbanContainer() const
{
    assert( pKnowledgeUrbanContainer_ );
    return *pKnowledgeUrbanContainer_;
}

namespace
{
    class sUrbanKnowledgesInserter
    {
    public:
        sUrbanKnowledgesInserter( T_UrbanObjectVector& container )
        : pContainer_( &container )
        {
        }

        void operator() ( UrbanObjectWrapper* pUrbanObject )
        {
            pContainer_->push_back( pUrbanObject );
        }

    private:
        T_UrbanObjectVector* pContainer_;
    };
}
// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetObjects
// Created: MGD 2010-02-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::GetUrbanObjects( T_UrbanObjectVector& container ) const
{
    sUrbanKnowledgesInserter functor( container );

    assert( pKnowledgeUrbanContainer_ );
    pKnowledgeUrbanContainer_->ApplyOnUrbanBlocks( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetArmy
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
MIL_Army_ABC& DEC_KnowledgeBlackBoard_Army::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKSUrbanKnowledgeSynthetizer
// Created: MGD 2009-12-02
// -----------------------------------------------------------------------------
DEC_KS_UrbanKnowledgeSynthetizer& DEC_KnowledgeBlackBoard_Army::GetKsUrbanKnowledgeSynthetizer() const
{
    assert( pKsUrbanKnowledgeSynthetizer_ );
    return *pKsUrbanKnowledgeSynthetizer_;
}
