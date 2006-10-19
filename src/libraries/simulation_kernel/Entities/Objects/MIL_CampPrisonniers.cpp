//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_CampPrisonniers.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 6 $
// $Workfile: MIL_CampPrisonniers.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_CampPrisonniers.h"
#include "MIL_RealObjectType.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Knowledge/DEC_Knowledge_ObjectCampPrisonniers.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( MIL_CampPrisonniers, "MIL_CampPrisonniers" )

//-----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_CampPrisonniers::MIL_CampPrisonniers()
    : MIL_RealObject_ABC( MIL_RealObjectType::campPrisonniers_ )
    , pTC2_             ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_CampPrisonniers::~MIL_CampPrisonniers()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_CampPrisonniers::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this )
         & pTC2_;

}

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::WriteSpecificAttributes
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_CampPrisonniers::WriteSpecificAttributes( MT_XXmlOutputArchive& archive ) const
{
    assert( pTC2_ );
    archive.WriteField( "TC2", pTC2_->GetID() );
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::Initialize
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool MIL_CampPrisonniers::Initialize( const MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( army, diaParameters, nCurrentParamIdx ) )
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
// Name: MIL_CampPrisonniers::Initialize
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
void MIL_CampPrisonniers::Initialize( uint nID, MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( nID, archive );

    uint nTC2_;
    archive.ReadField( "TC2", nTC2_ );

    MIL_Automate* pTC2Tmp = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nTC2_ );
    if( !pTC2Tmp )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automate TC2 specified is invalid", archive.GetContext() );
    if( !pTC2Tmp->GetType().IsLogistic() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automate TC2 specified is not a logistic automate", archive.GetContext() );
    pTC2_ = static_cast< MIL_AutomateLOG* >( pTC2Tmp );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::Initialize
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_CampPrisonniers::Initialize( uint nID, const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    if( !asnCreateObject.m.attributs_specifiquesPresent || asnCreateObject.attributs_specifiques.t != T_AttrObjectSpecific_camp_prisonniers )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    MIL_Automate* pTC2Tmp = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( asnCreateObject.attributs_specifiques.u.camp_prisonniers->tc2 );
    if( !pTC2Tmp || !pTC2Tmp->GetType().IsLogistic() )
        return EnumObjectErrorCode::error_invalid_specific_attributes;
    pTC2_ = static_cast< MIL_AutomateLOG* >( pTC2Tmp );

    return MIL_RealObject_ABC::Initialize( nID, asnCreateObject );
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::ProcessAgentInside
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void MIL_CampPrisonniers::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentInside( agent );
    if( pTC2_ && CanInteractWith( agent ) )
        agent.GetRole< PHY_RoleInterface_Surrender >().NotifyInsidePrisonerCamp( *this );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::ProcessAgentEntering
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void MIL_CampPrisonniers::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentEntering( agent );
    if( pTC2_ && CanInteractWith( agent ) )
        agent.GetRole< PHY_RoleInterface_Surrender >().NotifyInsidePrisonerCamp( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::ProcessAgentExiting
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void MIL_CampPrisonniers::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentExiting( agent );
    if( pTC2_ )
        agent.GetRole< PHY_RoleInterface_Surrender >().NotifyOutsidePrisonerCamp( *this );
}

// =============================================================================
// KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::CreateKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& MIL_CampPrisonniers::CreateKnowledge( const MIL_Army& teamKnowing )
{
    return *new DEC_Knowledge_ObjectCampPrisonniers( teamKnowing, *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
void MIL_CampPrisonniers::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg )
{
    assert( pTC2_ );

    asnAttributes_.tc2 = pTC2_->GetID();

    asnMsg.GetAsnMsg().m.attributs_specifiquesPresent           = 1;
    asnMsg.GetAsnMsg().attributs_specifiques.t                  = T_AttrObjectSpecific_camp_prisonniers;
    asnMsg.GetAsnMsg().attributs_specifiques.u.camp_prisonniers = &asnAttributes_;
}
