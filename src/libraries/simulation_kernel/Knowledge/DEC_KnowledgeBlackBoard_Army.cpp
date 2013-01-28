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
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "DEC_BlackBoard_CanContainKnowledgeUrban.h"
#include "DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "DEC_KS_UrbanKnowledgeSynthetizer.h"
#include "DEC_Knowledge_Object.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "DEC_Knowledge_Urban.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/MIL_Army_ABC.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/MIL_UrbanCache.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "protocol/Protocol.h"
#include <boost/noncopyable.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KnowledgeBlackBoard_Army )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army::DEC_KnowledgeBlackBoard_Army( MIL_Army_ABC& army )
    : DEC_KnowledgeBlackBoard_ABC()
    , pArmy_                        ( &army )
    , pKnowledgeObjectContainer_    ( new DEC_BlackBoard_CanContainKnowledgeObject() )
    , pKnowledgeUrbanContainer_     ( new DEC_BlackBoard_CanContainKnowledgeUrban( army ) )
    , pKsObjectKnowledgeSynthetizer_( new DEC_KS_ObjectKnowledgeSynthetizer( *this ) )
    , pKsUrbanKnowledgeSynthetizer_ ( new DEC_KS_UrbanKnowledgeSynthetizer( *this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army::DEC_KnowledgeBlackBoard_Army()
    : DEC_KnowledgeBlackBoard_ABC()
    , pArmy_                        ( 0 )
    , pKnowledgeObjectContainer_    ( 0 )
    , pKnowledgeUrbanContainer_     ( 0 )
    , pKsObjectKnowledgeSynthetizer_( 0 )
    , pKsUrbanKnowledgeSynthetizer_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_Army::~DEC_KnowledgeBlackBoard_Army()
{
    delete pKnowledgeObjectContainer_;
    delete pKnowledgeUrbanContainer_;
    delete pKsObjectKnowledgeSynthetizer_;
    delete pKsUrbanKnowledgeSynthetizer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KnowledgeBlackBoard_Army::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeBlackBoard_ABC >( *this )
            & pArmy_
            & pKnowledgeObjectContainer_
            & pKnowledgeUrbanContainer_
            & pKsObjectKnowledgeSynthetizer_;
            & pKsUrbanKnowledgeSynthetizer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::SendFullState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::SendFullState( unsigned int ) const
{
    std::mem_fun_ref_t< void, DEC_Knowledge_Object > functorObject = std::mem_fun_ref( &DEC_Knowledge_Object::SendStateToNewClient );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObjectRef( functorObject );
    std::mem_fun_ref_t< void, DEC_Knowledge_Urban > functorUrban = std::mem_fun_ref( &DEC_Knowledge_Urban::SendStateToNewClient );
    pKnowledgeUrbanContainer_->ApplyOnKnowledgesUrbanRef( functorUrban );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::SendChangedState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::SendChangedState() const
{
    std::mem_fun_ref_t< void, DEC_Knowledge_Object > functorObject = std::mem_fun_ref( &DEC_Knowledge_Object::UpdateOnNetwork );
    pKnowledgeObjectContainer_->ApplyOnKnowledgesObjectRef( functorObject );
    std::mem_fun_ref_t< void, DEC_Knowledge_Urban > functorUrban = std::mem_fun_ref( &DEC_Knowledge_Urban::UpdateOnNetwork );
    pKnowledgeUrbanContainer_->ApplyOnKnowledgesUrbanRef( functorUrban );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::SendChangedState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::GetObjectsInZone( T_KnowledgeObjectDiaIDVector& container, const MIL_ObjectFilter& filter, const TER_Localisation& zone )
{
    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->GetObjectsInZone( container, filter, zone );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::SendChangedState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::GetObjects( T_KnowledgeObjectVector& container ) const
{
    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->GetObjects( container );
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgesObject
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::GetKnowledgesObject( T_KnowledgeObjectVector& container ) const
{
    assert( pKnowledgeObjectContainer_ );
    pKnowledgeObjectContainer_->GetKnowledgesObject( container );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectFromID
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectFromID( unsigned int nID ) const
{
    assert( pKnowledgeObjectContainer_ );
    return pKnowledgeObjectContainer_->GetKnowledgeObjectFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectFromObjectID
// Created: JSR 2011-09-29
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectFromObjectID( unsigned int nID ) const
{
    assert( pKnowledgeObjectContainer_ );
    return pKnowledgeObjectContainer_->GetKnowledgeObjectFromObjectID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeObject
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::GetKnowledgeObject( const MIL_Object_ABC& object ) const
{
    assert( pKnowledgeObjectContainer_ );
    return pKnowledgeObjectContainer_->GetKnowledgeObject( object );
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
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObject( const sword::ObjectKnowledgeId& /*asn*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObject
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObject( const MIL_Object_ABC& /*object*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObject( unsigned int /*nID*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObjectByObjectID
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_Army::ResolveKnowledgeObjectByObjectID( unsigned int /*nID*/ ) const
{
    return boost::shared_ptr< DEC_Knowledge_Object >();
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
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectContainer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject& DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectContainer() const
{
    assert( pKnowledgeObjectContainer_ );
    return *pKnowledgeObjectContainer_;
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

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetObjects
// Created: MGD 2010-02-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::GetUrbanObjects( T_UrbanObjectVector& container ) const
{
    container = MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlocks();
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
// Name: DEC_KnowledgeBlackBoard_Army::GetKSObjectKnowledgeSynthetizer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KS_ObjectKnowledgeSynthetizer& DEC_KnowledgeBlackBoard_Army::GetKsObjectKnowledgeSynthetizer() const
{
    assert( pKsObjectKnowledgeSynthetizer_ );
    return *pKsObjectKnowledgeSynthetizer_;
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

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::Accept
// Created: LGY 2012-01-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::Accept( KnowledgesVisitor_ABC& visitor ) const
{
    pKnowledgeObjectContainer_->Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::Update
// Created: LDC 2012-02-02
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_Army::Update( int currentTimeStep )
{
    if( pArmy_ && pKnowledgeObjectContainer_ )
        pKnowledgeObjectContainer_->UpdateUniversalObjects( *pArmy_ );
    DEC_KnowledgeBlackBoard_ABC::Update( currentTimeStep );
    if( pKnowledgeObjectContainer_ )
        pKnowledgeObjectContainer_->Prepare();
}