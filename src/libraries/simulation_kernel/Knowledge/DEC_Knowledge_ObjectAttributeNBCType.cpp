// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeNBCType.h"
#include "DEC_Knowledge_Object.h"
#include "Entities/Objects/MIL_NBCType.h"
#include "Entities/Objects/NBCTypeAttribute.h"
#include "protocol/protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeNBCType )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBCType constructor
// Created: RFT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeNBCType::DEC_Knowledge_ObjectAttributeNBCType()
    : attr_              ( 0 )
    , pAgent_            ( 0 )
    , concentration_     ( 0 )
    , sourceLifeDuration_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBCType constructor
// Created: RFT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeNBCType::DEC_Knowledge_ObjectAttributeNBCType( const NBCTypeAttribute& attr )
    : attr_              ( &attr )
    , pAgent_            ( &attr.GetAgentType() )
    , concentration_     ( attr.GetConcentration() )
    , sourceLifeDuration_( attr.GetSourceLifeDuration() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBCType destructor
// Created: RFT 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeNBCType::~DEC_Knowledge_ObjectAttributeNBCType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::load
// Created: RFT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBCType::load( MIL_CheckPointInArchive& file, const uint )
{
    unsigned int nID;
    file >> boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file >> const_cast< NBCTypeAttribute*& >( attr_ );
    file >> nID
         >> concentration_
         >> sourceLifeDuration_;
    pAgent_ = MIL_NBCType::Find( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::save
// Created: RFT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBCType::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned int nID = ( pAgent_ ? pAgent_->GetID() : (uint)-1 );
    file << boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file << const_cast< NBCTypeAttribute*& >( attr_ );
    file << nID
         << concentration_
         << sourceLifeDuration_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBCType::Register
// Created: JCR 2008-09-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBCType::Register( DEC_Knowledge_Object& object )
{
    object.Attach( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBCType::UpdateSpecificAttributes
// Created: RFT 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBCType::UpdateAttributes()
{
    if( !attr_ )
        return;
    pAgent_ = &attr_->GetAgentType();
    concentration_ = attr_->GetConcentration();
    sourceLifeDuration_ = attr_->GetSourceLifeDuration();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBCType::UpdateSpecificAttributes
// Created: RFT 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBCType::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBCType::UpdateSpecificAttributes
// Created: RFT 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBCType::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBCType::UpdateSpecificAttributes
// Created: RFT 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBCType::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBCType::BuildMsgSpecificAttributes
// Created: RFT 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBCType::Send( Common::MsgObjectAttributes& message ) const
{
    message.mutable_nbc_agent()->set_agent_id( pAgent_? pAgent_->GetID() : -1 );
    message.mutable_nbc_agent()->set_concentration( concentration_ );
    message.mutable_nbc_agent()->set_source_life_duration( sourceLifeDuration_ );
}
