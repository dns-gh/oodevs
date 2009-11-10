// *****************************************************************************
//
// $Created: RFT 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectAttributeNBCType.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectAttributeNBCType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeNBCType.h"
#include "DEC_Knowledge_Object.h"
#include "Entities/Objects/MIL_NBCType.h"
#include "Entities/Objects/NBCTypeAttribute.h"
#include "MIL.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectAttributeNBCType, "DEC_Knowledge_ObjectAttributeNBCType" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBCType constructor
// Created: RFT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeNBCType::DEC_Knowledge_ObjectAttributeNBCType()
    : attr_         ( 0 )
    , pAgent_       ( 0 )
    , concentration_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBCType constructor
// Created: RFT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeNBCType::DEC_Knowledge_ObjectAttributeNBCType( const NBCTypeAttribute& attr )
    : attr_         ( &attr )
    , pAgent_       ( &attr.GetAgentType() ) 
    , concentration_( attr.GetConcentration() )
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

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::load
// Created: RFT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBCType::load( MIL_CheckPointInArchive& file, const uint )
{
    unsigned int nID;
    int concentration;
    file >> nID
         >> concentration;

    pAgent_        = MIL_NBCType::Find( nID );
    concentration_ = concentration;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::save
// Created: RFT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBCType::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned int nID = ( pAgent_ ? pAgent_->GetID() : (uint)-1 );
    file << nID
         << concentration_;
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
    if( !pAgent_ )
        pAgent_ = & attr_->GetAgentType();
    concentration_ = attr_->GetConcentration();
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
void DEC_Knowledge_ObjectAttributeNBCType::Send( ASN1T_ObjectAttributes& asn ) const
{
    if ( attr_ )
    {
        // asn.m.nbc_agentPresent      = 1;
        // asn.nbc_agent.agent_id      = pAgent_->GetID();    
        // asn.nbc_agent.concentration = concentration_;
    }
}
