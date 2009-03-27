// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectAttributeLogistic.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectAttributeLogistic.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeLogistic.h"
#include "Entities/Objects/LogisticAttribute.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "DEC_Knowledge_Object.h"

#include "MIL.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectAttributeLogistic, "DEC_Knowledge_ObjectAttributeLogistic" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeLogistic constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeLogistic::DEC_Knowledge_ObjectAttributeLogistic()
    : attr_ ( 0 )
    , pTC2_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeLogistic constructor
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeLogistic::DEC_Knowledge_ObjectAttributeLogistic( const LogisticAttribute& attr )
    : attr_ ( &attr )
    , pTC2_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeLogistic destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeLogistic::~DEC_Knowledge_ObjectAttributeLogistic()
{   
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeLogistic::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectAttributeLogistic::serialize( Archive& file, const uint )
{   
    file & boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file & const_cast< LogisticAttribute*& >( attr_ )
         & pTC2_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeLogistic::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeLogistic::Register( DEC_Knowledge_Object& knObject )
{
    knObject.Attach( *this );
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeLogistic::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeLogistic::UpdateAttributes()
{
    if ( attr_ )
    {
        pTC2_ = & attr_->GetTC2();
        NotifyAttributeUpdated( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeLogistic::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeLogistic::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeLogistic::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeLogistic::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeLogistic::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeLogistic::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeLogistic::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeLogistic::Send( ASN1T_ObjectAttributes& asn ) const
{
    if ( NeedUpdate() )
    {
        asn.m.logisticPresent = 1;
        asn.logistic.tc2 = pTC2_->GetID();
        Reset();
    }
}
