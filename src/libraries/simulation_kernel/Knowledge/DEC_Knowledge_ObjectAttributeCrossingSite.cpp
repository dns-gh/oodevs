// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectAttributeCrossingSite.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectAttributeCrossingSite.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeCrossingSite.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "Entities/Objects/CrossingSiteAttribute.h"
#include "DEC_Knowledge_Object.h"

#include "MIL.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectAttributeCrossingSite, "DEC_Knowledge_ObjectAttributeCrossingSite" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeCrossingSite constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeCrossingSite::DEC_Knowledge_ObjectAttributeCrossingSite()
    : attr_ ( 0 )
    , rWidth_ ( 0. )
    , rDepth_ ( 0. )
    , rCurrentSpeed_ ( 0. )
    , bBanksToFitOut_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeCrossingSite constructor
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeCrossingSite::DEC_Knowledge_ObjectAttributeCrossingSite( const CrossingSiteAttribute& attr )
    : attr_ ( &attr )
    , rWidth_ ( 0. )
    , rDepth_ ( 0. )
    , rCurrentSpeed_ ( 0. )
    , bBanksToFitOut_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeCrossingSite destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeCrossingSite::~DEC_Knowledge_ObjectAttributeCrossingSite()
{    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeCrossingSite::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectAttributeCrossingSite::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file & const_cast< CrossingSiteAttribute*& >( attr_ )
         & rWidth_
         & rDepth_
         & rCurrentSpeed_
         & bBanksToFitOut_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeCrossingSite::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeCrossingSite::Register( DEC_Knowledge_Object& knObject )
{
    knObject.Attach( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeCrossingSite::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeCrossingSite::UpdateAttributes()
{
    if ( ! attr_ )
        return;    

    rWidth_         = attr_->GetWidth();
    rDepth_         = attr_->GetDepth();
    rCurrentSpeed_  = attr_->GetSpeed();
    bBanksToFitOut_ = attr_->IsBanksToFitOut();
    NotifyAttributeUpdated( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeCrossingSite::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeCrossingSite::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeCrossingSite::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeCrossingSite::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception )
{
//    if( !IsReconBy( perception.GetAgentPerceiving().GetType() ) )
//        return;
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeCrossingSite::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeCrossingSite::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision )
{
//    if( !IsReconBy( perception.GetAgentPerceiving().GetType() ) )
//        return;
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeCrossingSite::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeCrossingSite::Send( ASN1T_ObjectAttributes& asn ) const
{
//    if( !IsAttributeUpdated( eAttr_Specific ) || !bSpecificAttributesValid_ )
//        return;
//    
    if ( NeedUpdate() )
    {
        asn.m.crossing_sitePresent = 1;
        asn.crossing_site.width                 = (int)rWidth_;
        asn.crossing_site.depth                 = (int)rDepth_;
        asn.crossing_site.flow_rate             = (int)rCurrentSpeed_;
        asn.crossing_site.banks_require_fitting = bBanksToFitOut_;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeSupplyRoute::IsBanksToFitOut
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
bool DEC_Knowledge_ObjectAttributeCrossingSite::IsBanksToFitOut() const
{
    return bBanksToFitOut_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeCrossingSite::GetWidth
// Created: JCR 2008-06-04
// -----------------------------------------------------------------------------
float DEC_Knowledge_ObjectAttributeCrossingSite::GetWidth() const
{
    return rWidth_;
}
