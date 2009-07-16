// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectAttributeObstacle.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectAttributeObstacle.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeObstacle.h"
#include "Entities/Objects/ObstacleAttribute.h"
#include "DEC_Knowledge_Object.h"

#include "MIL.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectAttributeObstacle, "DEC_Knowledge_ObjectAttributeObstacle" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeObstacle constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeObstacle::DEC_Knowledge_ObjectAttributeObstacle()
    : attr_ ( 0 )
    , obstacle_ ( E_ObstacleType::preliminary )
    , bActivated_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeObstacle constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeObstacle::DEC_Knowledge_ObjectAttributeObstacle( const ObstacleAttribute& attr )
    : attr_ ( &attr )
    , obstacle_ ( ( attr.IsActivable() ) ? E_ObstacleType::reserved : E_ObstacleType::preliminary )
    , bActivated_( attr.IsActivable() ? false : true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeObstacle destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeObstacle::~DEC_Knowledge_ObjectAttributeObstacle()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeObstacle::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectAttributeObstacle::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file & const_cast< ObstacleAttribute*& >( attr_ )
         & obstacle_
         & bActivated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeObstacle::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeObstacle::Register( DEC_Knowledge_Object& knObject )
{
    knObject.AttachExtension( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeObstacle::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeObstacle::UpdateAttributes()
{
    if ( attr_ && bActivated_ != attr_->IsActivated() )
    {
        bActivated_ = attr_->IsActivated();
        NotifyAttributeUpdated( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeObstacle::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeObstacle::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeObstacle::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeObstacle::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeObstacle::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeObstacle::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeObstacle::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeObstacle::Send( ASN1T_ObjectAttributes& asn ) const
{
    if ( NeedUpdate() )
    {
        asn.m.obstaclePresent = 1;
        asn.obstacle.type = obstacle_;
        asn.obstacle.activated = bActivated_;
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeObstacle::BuildMsgSpecificAttributes
// Created: JCR 2008-08-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_ObjectAttributeObstacle::IsActivated() const
{
	return bActivated_;
}