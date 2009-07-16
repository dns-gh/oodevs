// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectAttributeBypass.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectAttributeBypass.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeBypass.h"
#include "DEC_Knowledge_Object.h"
#include "Entities/Objects/BypassAttribute.h"


#include "MIL.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectAttributeBypass, "DEC_Knowledge_ObjectAttributeBypass" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeBypass constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeBypass::DEC_Knowledge_ObjectAttributeBypass(  )
    : attr_ ( 0 )
    , nBypassPercentage_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeBypass constructor
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeBypass::DEC_Knowledge_ObjectAttributeBypass( const BypassAttribute& attr )
    : attr_ ( &attr )
    , nBypassPercentage_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeBypass destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeBypass::~DEC_Knowledge_ObjectAttributeBypass()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeBypass::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectAttributeBypass::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file & const_cast< BypassAttribute*& >( attr_ )
         & nBypassPercentage_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeBypass::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeBypass::Register( DEC_Knowledge_Object& knObject )
{
    knObject.AttachExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeBypass::IsBypassed
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
bool DEC_Knowledge_ObjectAttributeBypass::IsBypassed() const
{
    return nBypassPercentage_ >= 100;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeBypass::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeBypass::UpdateAttributes()
{
    if( ! attr_ )
        return;
    
    const uint nNewBypassPercentage = (uint)( attr_->GetState() * 100. );
    if( nNewBypassPercentage != nBypassPercentage_ )
    {
        nBypassPercentage_ = nNewBypassPercentage;
        NotifyAttributeUpdated( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeBypass::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeBypass::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeBypass::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeBypass::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeBypass::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeBypass::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeBypass::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeBypass::Send( ASN1T_ObjectAttributes& asn ) const
{    
    if ( NeedUpdate() )
    {
        asn.m.bypassPresent = 1;
        asn.bypass.m.percentagePresent = 1;
        asn.bypass.percentage = nBypassPercentage_;
        Reset();
    }
}
