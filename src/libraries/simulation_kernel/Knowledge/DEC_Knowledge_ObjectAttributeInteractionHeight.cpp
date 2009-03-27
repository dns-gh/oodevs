// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeInteractionHeight.h"

#include "Entities/Objects/InteractionHeightAttribute.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include <boost/serialization/void_cast.hpp>


BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectAttributeInteractionHeight, "DEC_Knowledge_ObjectAttributeInteractionHeight" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInteractionHeight constructor
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeInteractionHeight::DEC_Knowledge_ObjectAttributeInteractionHeight()
: height_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInteractionHeight constructor
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeInteractionHeight::DEC_Knowledge_ObjectAttributeInteractionHeight( const InteractionHeightAttribute& attribute )
: height_( attribute.Get() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInteractionHeight destructor
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeInteractionHeight::~DEC_Knowledge_ObjectAttributeInteractionHeight()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInteractionHeight::Get
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
float DEC_Knowledge_ObjectAttributeInteractionHeight::Get() const
{
    return height_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInteractionHeight::UpdateOnPerceptionLevel
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeInteractionHeight::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInteractionHeight::UpdateOnPerception
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeInteractionHeight::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInteractionHeight::UpdateOnCollision
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeInteractionHeight::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& /*collision*/  )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInteractionHeight::Send
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeInteractionHeight::Send( ASN1T_ObjectAttributes& asn ) const
{
    asn.m.interaction_heightPresent = 1;
    asn.interaction_height.height = height_;
}
    
// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_Knowledge_ObjectAttributeInteractionHeight::serialize
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectAttributeInteractionHeight::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this )
            & height_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeInteractionHeight::Register
// Created: LDC 2009-03-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeInteractionHeight::Register( DEC_Knowledge_Object& /*knObject*/ )
{

}
