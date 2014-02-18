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

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectCollision )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision::DEC_Knowledge_ObjectCollision( const MIL_Agent_ABC& agentColliding, MIL_Object_ABC& object )
    : DEC_Knowledge_ABC()
    , pAgentColliding_ ( &agentColliding )
    , pObject_         ( &object )
    , bIsValid_        ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision constructor
// Created: LDC 2014-02-18
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision::DEC_Knowledge_ObjectCollision( MIL_Object_ABC& object )
    : DEC_Knowledge_ABC()
    , pAgentColliding_ ( 0 )
    , pObject_         ( &object )
    , bIsValid_        ( false )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision constructor
// Created: JVT 2005-03-16
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision::DEC_Knowledge_ObjectCollision()
    : DEC_Knowledge_ABC()
    , pAgentColliding_( 0 )
    , pObject_        ( 0 )
    , bIsValid_       ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectCollision::~DEC_Knowledge_ObjectCollision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectCollision::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         & const_cast< MIL_Agent_ABC*& >( pAgentColliding_ )
         & pObject_
         & vPosition_
         & bIsValid_;
}

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

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::IsValid
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
bool DEC_Knowledge_ObjectCollision::IsValid() const
{
    return bIsValid_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::GetObject
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
MIL_Object_ABC& DEC_Knowledge_ObjectCollision::GetObject() const
{
    assert( pObject_ );
    return *pObject_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::GetAgentColliding
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
const MIL_Agent_ABC* DEC_Knowledge_ObjectCollision::GetAgentColliding() const
{
    return pAgentColliding_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::GetPosition
// Created: NLD 2004-11-03
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Knowledge_ObjectCollision::GetPosition() const
{
    return vPosition_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_ObjectCollision::Clean()
{
    return !IsValid();
}
