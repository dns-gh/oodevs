// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectAttributeNBC.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectAttributeNBC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeNBC.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/Objects/NBCAttribute.h"
#include "protocol/protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeNBC )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeNBC::DEC_Knowledge_ObjectAttributeNBC()
    : attr_ ( 0 )
    , pNbcAgentType_ ( 0 )    
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeNBC::DEC_Knowledge_ObjectAttributeNBC( const NBCAttribute& attr )
    : attr_ ( &attr )
    , pNbcAgentType_ ( 0 )    
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeNBC::~DEC_Knowledge_ObjectAttributeNBC()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::load
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
//    unsigned int nID;
//    file >> nID;
//    pNbcAgentType_ = MIL_NbcAgentType::Find( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::save
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
//    unsigned nbc = ( pNbcAgentType_ ? pNbcAgentType_->GetID() : (unsigned int)-1 );
//    file << nbc;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::Register( DEC_Knowledge_Object& /*knObject*/ )
{
//    knObject.AttachExtension( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::UpdateAttributes()
{
//    pNbcAgentType_ = attr_->GetNbcAgentType();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::Send( Common::MsgObjectAttributes& /*message*/ ) const
{
    if ( attr_ )
    {
        // $$$$ SBO 2009-10-27: huu?
//    asn.set_nbcPresent( 1 );
//    asn.nbc.nbc_agent = pNbcAgentType_->GetID();    
    }
}
