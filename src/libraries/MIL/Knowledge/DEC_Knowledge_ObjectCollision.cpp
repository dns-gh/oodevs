// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectCollision.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:32 $
// $Revision: 5 $
// $Workfile: DEC_Knowledge_ObjectCollision.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Knowledge_ObjectCollision.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"

#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectCollision, "DEC_Knowledge_ObjectCollision" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision::DEC_Knowledge_ObjectCollision( const MIL_AgentPion& agentColliding, MIL_RealObject_ABC& object )
    : DEC_Knowledge_ABC()
    , pAgentColliding_ ( &agentColliding )
    , pObject_         ( &object )
    , vPosition_       ()
    , bIsValid_        ( false )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision constructor
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision::DEC_Knowledge_ObjectCollision()
    : DEC_Knowledge_ABC()
    , pAgentColliding_ ( 0 )
    , pObject_         ( 0 )
    , vPosition_       ()
    , bIsValid_        ( false )
{
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision::~DEC_Knowledge_ObjectCollision()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_Knowledge_ObjectCollision::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         & const_cast< MIL_AgentPion*& >( pAgentColliding_ )
         & pObject_
         & vPosition_
         & bIsValid_;    
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectCollision::Prepare()
{
    bIsValid_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::Update
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectCollision::Update( const MT_Vector2D& vPosition )
{
    vPosition_ = vPosition;
    bIsValid_  = true;
}