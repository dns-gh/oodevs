//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_CampRefugies.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 6 $
// $Workfile: MIL_CampRefugies.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_CampRefugies.h"
#include "MIL_RealObjectType.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Knowledge/DEC_Knowledge_ObjectCampRefugies.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( MIL_CampRefugies, "MIL_CampRefugies" )

//-----------------------------------------------------------------------------
// Name: MIL_CampRefugies constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_CampRefugies::MIL_CampRefugies( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
    , pTC2_             ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_CampRefugies::MIL_CampRefugies()
    : MIL_RealObject_ABC()
    , pTC2_             ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_CampRefugies destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_CampRefugies::~MIL_CampRefugies()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_CampRefugies::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this )
         & pTC2_;

}

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::WriteSpecificAttributes
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_CampRefugies::WriteSpecificAttributes( MT_XXmlOutputArchive& archive ) const
{
    assert( pTC2_ );
    archive.Section( "specific-attributes" );
    archive.Section( "tc2" );
    archive.WriteAttribute( "id", pTC2_->GetID() );
    archive.EndSection(); // tc2
    archive.EndSection(); // specific-attributes
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::Initialize
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool MIL_CampRefugies::Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( obstacleType, diaParameters, nCurrentParamIdx ) )
        return false;

    DEC_AutomateDecision* pDecTC2 = diaParameters[ nCurrentParamIdx ].ToUserObject( pDecTC2 );
    if( !pDecTC2 )
        return false;

    if( !pDecTC2->GetAutomate().GetType().IsLogistic() )
        return false;

    pTC2_ = static_cast< MIL_AutomateLOG* >( &pDecTC2->GetAutomate() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::Initialize
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
void MIL_CampRefugies::Initialize( MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( archive );

    archive.Section( "specific-attributes" );
    archive.Section( "tc2" );
    uint nTC2;
    archive.ReadAttribute( "id", nTC2 );
    MIL_Automate* pTC2Tmp = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nTC2 );
    if( !pTC2Tmp )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automate TC2 specified is invalid", archive.GetContext() );
    if( !pTC2Tmp->GetType().IsLogistic() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automate TC2 specified is not a logistic automate", archive.GetContext() );
    pTC2_ = static_cast< MIL_AutomateLOG* >( pTC2Tmp );
    archive.EndSection(); // tc2
    archive.EndSection(); // specific-attributes
}
    
// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::Initialize
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_CampRefugies::Initialize( const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    if( !asnCreateObject.m.specific_attributesPresent || asnCreateObject.specific_attributes.t != T_ObjectAttributesSpecific_refugee_camp )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    MIL_Automate* pTC2Tmp = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( asnCreateObject.specific_attributes.u.refugee_camp->tc2 );
    if( !pTC2Tmp || !pTC2Tmp->GetType().IsLogistic() )
        return EnumObjectErrorCode::error_invalid_specific_attributes;
    pTC2_ = static_cast< MIL_AutomateLOG* >( pTC2Tmp );

    return MIL_RealObject_ABC::Initialize( asnCreateObject );
}

// =============================================================================
// KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::CreateKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& MIL_CampRefugies::CreateKnowledge( const MIL_Army& teamKnowing )
{
    return *new DEC_Knowledge_ObjectCampRefugies( teamKnowing, *this );
}
   
// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
void MIL_CampRefugies::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg )
{
    assert( pTC2_ );

    asnAttributes_.tc2 = pTC2_->GetID();

    asnMsg().m.specific_attributesPresent       = 1;
    asnMsg().specific_attributes.t              = T_ObjectAttributesSpecific_refugee_camp;
    asnMsg().specific_attributes.u.refugee_camp = &asnAttributes_;
}