// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectSiteFranchissement.cpp $
// $Author: Jvt $
// $Modtime: 7/04/05 11:35 $
// $Revision: 8 $
// $Workfile: DEC_Knowledge_ObjectSiteFranchissement.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectSiteFranchissement.h"

#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "Entities/Objects/MIL_SiteFranchissement.h"
#include "Entities/Agents/MIL_AgentPion.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectSiteFranchissement, "DEC_Knowledge_ObjectSiteFranchissement" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectSiteFranchissement constructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectSiteFranchissement::DEC_Knowledge_ObjectSiteFranchissement( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown )
    : DEC_Knowledge_Object     ( armyKnowing, objectKnown )
    , bSpecificAttributesValid_( false )
    , rWidth_                  ( 0. )
    , rDepth_                  ( 0. )
    , rCurrentSpeed_           ( 0. )
    , bBanksToFitOut_          ( false )
{    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectSiteFranchissement constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectSiteFranchissement::DEC_Knowledge_ObjectSiteFranchissement()
    : DEC_Knowledge_Object     ()
    , bSpecificAttributesValid_( false )
    , rWidth_                  ( 0. )
    , rDepth_                  ( 0. )
    , rCurrentSpeed_           ( 0. )
    , bBanksToFitOut_          ( false )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectSiteFranchissement destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectSiteFranchissement::~DEC_Knowledge_ObjectSiteFranchissement()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectSiteFranchissement::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_Knowledge_ObjectSiteFranchissement::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< DEC_Knowledge_Object >( *this );
    file & rWidth_
         & rDepth_
         & rCurrentSpeed_
         & bBanksToFitOut_
         & bSpecificAttributesValid_;
}


// =============================================================================
// OPERATIONS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectSiteFranchissement::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectSiteFranchissement::UpdateSpecificAttributes()
{
    if( !GetObjectKnown() )
        return;

    if( !bSpecificAttributesValid_ )
    {
        bSpecificAttributesValid_ = true;

        const MIL_SiteFranchissement& siteFranchissement = static_cast< const MIL_SiteFranchissement& >( *GetObjectKnown() );
        rWidth_         = siteFranchissement.GetWidth();
        rDepth_         = siteFranchissement.GetDepth();
        rCurrentSpeed_  = siteFranchissement.GetCurrentSpeed();
        bBanksToFitOut_ = siteFranchissement.GetBanksToFitOut();
        NotifyAttributeUpdated( eAttr_Specific );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectSiteFranchissement::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectSiteFranchissement::UpdateSpecificAttributes( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    // NOTHING        
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectSiteFranchissement::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectSiteFranchissement::UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& perception )
{
    if( !IsReconBy( perception.GetAgentPerceiving().GetType() ) )
        return;
    UpdateSpecificAttributes();
}

   
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectSiteFranchissement::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectSiteFranchissement::UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision& collision )
{
    if( !IsReconBy( collision.GetAgentColliding().GetType() ) )
        return;
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectSiteFranchissement::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectSiteFranchissement::BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    if( !IsAttributeUpdated( eAttr_Specific ) || !bSpecificAttributesValid_ )
        return;
    
    asnAttributes_.largeur           = (int)rWidth_;
    asnAttributes_.profondeur        = (int)rDepth_;
    asnAttributes_.vitesse_courant   = (int)rCurrentSpeed_;
    asnAttributes_.berges_a_amenager = bBanksToFitOut_;

    asnMsg.m.attributs_specifiquesPresent = 1;
    asnMsg.attributs_specifiques.t                     = T_ObjectAttributesSpecific_site_franchissement;
    asnMsg.attributs_specifiques.u.site_franchissement = &asnAttributes_;
}


    

 