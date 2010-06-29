// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectAttributeConstruction.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectAttributeConstruction.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeConstruction.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "DEC_Knowledge_Object.h"
#include "protocol/protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeConstruction )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeConstruction constructor
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeConstruction::DEC_Knowledge_ObjectAttributeConstruction()
    : attr_ ( 0 )
    , rConstructionPercentage_ ( 0. )
    , nNbrDotation_ ( 0 )
    , nDotationType_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeConstruction constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeConstruction::DEC_Knowledge_ObjectAttributeConstruction( const ConstructionAttribute& attr )
    : attr_ ( &attr )
    , rConstructionPercentage_ ( 0. )
    , nNbrDotation_ ( 0 )
    , nDotationType_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeConstruction destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeConstruction::~DEC_Knowledge_ObjectAttributeConstruction()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeConstruction::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectAttributeConstruction::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file & const_cast< ConstructionAttribute*& >( attr_ )
         & rConstructionPercentage_
         & nNbrDotation_
         & nDotationType_;

}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeConstruction::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeConstruction::Register( DEC_Knowledge_Object& knObject )
{
    knObject.AttachExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeConstruction::UpdateAttribute
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeConstruction::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeConstruction::UpdateAttribute
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeConstruction::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeConstruction::UpdateAttribute
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeConstruction::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeConstruction::BuildMsgAttribute
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeConstruction::Send( Common::MsgObjectAttributes& asn ) const
{
    asn.mutable_construction()->set_percentage( int( rConstructionPercentage_ * 100) );
    asn.mutable_construction()->set_dotation_nbr( nNbrDotation_ );
    asn.mutable_construction()->set_dotation_type( nDotationType_ );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeConstruction::IsConstructed
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
bool DEC_Knowledge_ObjectAttributeConstruction::IsConstructed() const
{
    return rConstructionPercentage_ >= 1.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeConstruction::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeConstruction::UpdateAttributes( /*const MIL_Knowledge_Object& knowledge*/ )
{
    if( ! attr_ )
        return;

    nNbrDotation_ = attr_->GetCurrentDotation();
    rConstructionPercentage_ = attr_->GetState();
    nDotationType_ = attr_->GetDotationType();
}
