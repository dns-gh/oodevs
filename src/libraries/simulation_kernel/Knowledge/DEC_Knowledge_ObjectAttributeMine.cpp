// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectAttributeMine.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectAttributeMine.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeMine.h"
#include "Entities/Objects/MineAttribute.h"
#include "Entities/MIL_Army.h"
#include "DEC_Knowledge_Object.h"
#include "protocol/protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeMine )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMine constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeMine::DEC_Knowledge_ObjectAttributeMine()
    : attr_ ( 0 )
    , nNbrDotationForMining_ ( 0 ) 
    , rMiningPercentage_ ( 0. )
    , nMinesActivityTime_ ( 0 )
    , rMinesDensity_ ( 0. )
    , nDotationType_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMine constructor
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeMine::DEC_Knowledge_ObjectAttributeMine( const MineAttribute& attr )
    : attr_ ( &attr )
    , nNbrDotationForMining_ ( 0 ) 
    , rMiningPercentage_ ( 0. )
    , nMinesActivityTime_ ( 0 )
    , rMinesDensity_ ( 0. )
    , nDotationType_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMine destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeMine::~DEC_Knowledge_ObjectAttributeMine()
{    
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMine::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectAttributeMine::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file & const_cast< MineAttribute*& >( attr_ )
         & nNbrDotationForMining_
         & rMiningPercentage_
         & nMinesActivityTime_
         & rMinesDensity_
         & nDotationType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMine::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMine::Register( DEC_Knowledge_Object& knObject )
{
    knObject.AttachExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMine::UpdateOnPerceptionLevel
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMine::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateAttributes();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMine::UpdateOnPerception
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMine::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateAttributes();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMine::UpdateOnCollision
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMine::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    UpdateAttributes();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMine::MsgObjectAttributes
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMine::Send( Common::MsgObjectAttributes& asn ) const
{
    asn.mutable_mine()->set_percentage( (unsigned int)( rMiningPercentage_ * 100. ) ); 
    asn.mutable_mine()->set_dotation_nbr( nNbrDotationForMining_ );
    asn.mutable_mine()->set_dotation_type( nDotationType_ );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeMine::UpdateAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeMine::UpdateAttributes()
{
    if ( !attr_ )
        return;

    nNbrDotationForMining_ = attr_->GetCurrentDotations();
    rMiningPercentage_ = attr_->GetState();
    nDotationType_ = attr_->GetDotationType();
}
